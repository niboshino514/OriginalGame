#include "TitleScreen.h"
#include <DxLib.h>
#include "Game.h"
#include "SceneTitle.h"
#include "Controller.h"
#include "EvoLib.h"
#include "Sound.h"
#include "GameData.h"


namespace GraphData
{

	// 選択されていないグラフィックの透明度
	constexpr int kAlpha = 100;

	// 選択されているグラフィックを左にずらす
	const float kSelectMoveX = -10;

	// 選択中のグラフィックデータ
	static const EvoLib::Load::GraphicInfo kSelectGraphInfo =
	{
		// ファイルパス
		"Data/Graphic/Title/Select/Select.png",

		// 中心座標
		Vec2(Game::kWindowCenterX_F, Game::kWindowCenterY_F),

		// グラフィックスケール
		0.6,

		// グラフィックの分割数
		EvoLib::Load::DivNum(1,3),
	};

	// 選択中のグラフィックの距離
	const Vec2 kSelectGraphDistance = Vec2(0.0f, 10.0f);


}
namespace SelectTriangleGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Selection/SelectTriangle.png";

	// 拡大率
	const float kScale = 0.4f;

	// フレーム速度
	const int kFrameSpeed = 1;

	// 距離
	const Vec2 kSelectDistanceValue = Vec2(-200, 0);

}

namespace
{
	// サウンドファイル名
	static std::vector<std::string> kSoundFileName =
	{
		"Bgm_0",		// BGM

		"Determination",// 決定音
		"Cancel",		// キャンセル音
		"Select",		// 選択音
	};

	// サウンド名
	enum class SoundName
	{
		BGM,			// BGM

		Decision,		// 決定音
		Cancel,			// キャンセル音
		Select,			// 選択音
	};

}



TitleScreen::TitleScreen():
	m_selectGraphInfo(),
	m_selectTriangleGraph(0),
	m_select(Select()),
	m_isContinue(false),
	m_pStateMachine(),
	m_pSceneTitle(nullptr)
{
}

TitleScreen::~TitleScreen()
{
	// グラフィックの解放
	for(auto& handle : m_selectGraphInfo.handle)
	{
		DeleteGraph(handle);
	}
}

void TitleScreen::Init()
{
	// グラフィックの読み込み
	Load();

	// セーブデータ確認
	CheckSaveData();

	// スコアデータ確認
	CheckScoreData();

	// ステートの初期化
	StateInit();

	// BGM再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::BGM)]);
}

void TitleScreen::Update()
{
	// ステートマシンの更新
	m_pStateMachine.Update();
}

void TitleScreen::Draw()
{
	// 背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xa9a9a9, true);

	// ステートマシンの描画
	m_pStateMachine.Draw();



	if (m_scoreData.isClear)
	{
		// 時間描画
		DrawFormatString(0, 15 * 0, GetColor(255, 255, 255), "%d:%d:%d:%d",
			m_scoreData.clearTime.hour,
			m_scoreData.clearTime.minute,
			m_scoreData.clearTime.second,
			m_scoreData.clearTime.millisecond);

		// 死亡回数描画
		DrawFormatString(0, 15 * 1, GetColor(255, 255, 255), "DeathCount:%d",
			m_scoreData.deathCount);

	}
	
}

void TitleScreen::Load()
{
	// サウンドの読み込み
	{
		Sound::GetInstance()->Load(kSoundFileName);	
	}


	// 選択中のグラフィック読み込み
	{
		// グラフィックの読み込み
		m_selectGraphInfo.handle = 
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(
				GraphData::kSelectGraphInfo.filePath,
				GraphData::kSelectGraphInfo.div);

		// グラフィックのサイズを取得
		const Vec2 size = 
			EvoLib::Calculation::GetGraphSize_EvoLib(
				m_selectGraphInfo.handle,
				GraphData::kSelectGraphInfo.scale);

		// サイズを代入
		for(int i = 0; i < m_selectGraphInfo.handle.size(); i++)
		{
			m_selectGraphInfo.scale.push_back(GraphData::kSelectGraphInfo.scale);
		}

		// グラフィックを均等に配置する
		m_selectGraphInfo.pos =
			EvoLib::Calculation::GraphEqualization(
				size,
				GraphData::kSelectGraphInfo.pos,
				GraphData::kSelectGraphInfo.div.y,
				GraphData::kSelectGraphDistance,
				false);
	}


	// セレクト三角形グラフィック
	{
		// グラフィックロード
		m_selectTriangleGraph = LoadGraph(SelectTriangleGraph::kFilePath);
	}
}

void TitleScreen::StateInit()
{
	// ステートマシンの初期化
	auto dummy = []() {};

	// 入力待ちステート
	{
		auto enter = [this]() {this->StateWaitEnter(); };
		auto update = [this]() {this->StateWaitUpdate(); };
		auto draw = [this]() {this->StateWaitDraw(); };
		auto exit = [this]() {this->StateWaitExit(); };

		m_pStateMachine.AddState(State::WaitInput, enter, update, draw, exit);
	}

	// 選択中ステート
	{
		auto enter = [this]() {this->StateSelectEnter(); };
		auto update = [this]() {this->StateSelectUpdate(); };
		auto draw = [this]() {this->StateSelectDraw(); };
		auto exit = [this]() {this->StateSelectExit(); };

		m_pStateMachine.AddState(State::Selecting, enter, update, draw, exit);
	}

	// 初期ステート設定
	m_pStateMachine.SetState(State::Selecting);
}

void TitleScreen::StateWaitEnter()
{
}

void TitleScreen::StateWaitUpdate()
{
	// 決定ボタンが押されたら、選択中ステートに遷移する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		// 選択中ステートに遷移
		m_pStateMachine.SetState(State::Selecting);
	}
}

void TitleScreen::StateWaitDraw()
{
}

void TitleScreen::StateWaitExit()
{
}

void TitleScreen::StateSelectEnter()
{
}

void TitleScreen::StateSelectUpdate()
{
	// 選択処理
	SelectProcess();

	// 決定処理
	DecideProcess();
}

void TitleScreen::StateSelectDraw()
{
	// 選択されていないグラフィック描画
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, GraphData::kAlpha);

		// グラフィック描画
		for (int i = 0; i < m_selectGraphInfo.handle.size(); i++)
		{
			// 選択中ならば、continueする
			if (i == static_cast<int>(m_select))
			{
				continue;
			}

			if (!m_isContinue &&
				i == static_cast<int>(Select::Continue))
			{
				continue;
			}


			DrawRotaGraphF(
				m_selectGraphInfo.pos[i].x,
				m_selectGraphInfo.pos[i].y,
				m_selectGraphInfo.scale[i],
				0.0f,
				m_selectGraphInfo.handle[i],
				true);
		}
		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 選ばれている選択肢は赤色で描画
	SetDrawBright(255, 0, 0);

	// 選択中のグラフィック描画
	{
		// グラフィック描画
		DrawRotaGraphF(
			m_selectGraphInfo.pos[static_cast<int>(m_select)].x + GraphData::kSelectMoveX,
			m_selectGraphInfo.pos[static_cast<int>(m_select)].y,
			m_selectGraphInfo.scale[static_cast<int>(m_select)],
			0.0f,
			m_selectGraphInfo.handle[static_cast<int>(m_select)],
			true);
	}


	// 選択されている三角形描画
	{
		Vec2 pos = Vec2();
		pos.x = (m_selectGraphInfo.pos[static_cast<int>(m_select)].x + GraphData::kSelectMoveX);
		pos.y = m_selectGraphInfo.pos[static_cast<int>(m_select)].y;

		pos += SelectTriangleGraph::kSelectDistanceValue;

		// 回転描画
		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			pos,
			SelectTriangleGraph::kFrameSpeed,
			false,
			SelectTriangleGraph::kScale);
	}

	// 色を元に戻す
	SetDrawBright(255, 255, 255);
}

void TitleScreen::StateSelectExit()
{
}

void TitleScreen::CheckSaveData()
{
	// セーブデータが初期化されているかどうかを取得
	const bool isSaveDataInit = GameData::GetInstance()->IsSaveDataInit();

	// セーブデータが初期化されているならば、続きからプレイできない
	if(isSaveDataInit)
	{
		m_isContinue = false;

		// セレクトの初期をNewGameに設定
		m_select = Select::NewGame;
	}
	else
	{
		m_isContinue = true;
	}

}

void TitleScreen::CheckScoreData()
{
	// スコアデータの読み込み
	m_scoreData = GameData::GetInstance()->GetScore();
}

void TitleScreen::SelectProcess()
{
	// セレクト数
	const int selectNum = static_cast<int>(Select::SelectNum);




	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		m_select = static_cast<Select>(((static_cast<int>(m_select) - 1 + selectNum) % selectNum));

		if (!m_isContinue &&
			m_select == Select::Continue)
		{
			m_select = static_cast<Select>((static_cast<int>(m_select) - 1 + selectNum));
		}

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		m_select = static_cast<Select>(((static_cast<int>(m_select) + 1) % selectNum));

		if(!m_isContinue&&
			m_select == Select::Continue)
		{
			m_select = static_cast<Select>(((static_cast<int>(m_select) + 1)));
		}

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void TitleScreen::DecideProcess()
{
	// フェードインが終了していなければ、return
	if (!m_pSceneTitle->IsFadeInEnd())
	{
		return;
	}

	// 決定ボタンが押されていなければ、return
	if (!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}



	// 選択されたものによって処理を分岐
	switch (m_select)
	{
	case TitleScreen::Select::Continue:
		// シーンを変更する
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::GameMain);

		break;
	case TitleScreen::Select::NewGame:
		// セーブデータを初期化
		GameData::GetInstance()->InitSaveData();
		// シーンを変更する
		m_pSceneTitle->ChangeScene(SceneTitle::Scene::GameMain);

	
		break;
	case TitleScreen::Select::GameEnd:
		
		GameData::GetInstance()->WriteSaveData();	// セーブデータを書き込む
		GameData::GetInstance()->WriteScoreData();	// スコアデータの書き込み

		// ゲーム終了
		DxLib_End();

		break;
	default:
		break;
	}

	// 決定音を再生
	Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Decision)]);
}

void TitleScreen::DrawScore()
{

}