#include "Pause.h"
#include "game.h"
#include <DxLib.h>
#include "Controller.h"
#include "ObjectManager.h"
#include "EvoLib.h"
#include "ControllerOption.h"
#include "Sound.h"
#include "SoundOption.h"

namespace Window
{
	// ポーズウィンドウの横幅
	const int kWidth = 700;

	// ポーズウィンドウの縦幅
	const int kHeight = 450;

	// 透明度
	const int kAlpha = 200;
}

namespace WindowNameGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Pause/WindowName.png";
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// 拡大率
	const double kScale = 1.0;
}

namespace BackTitleWindow
{
	// ポーズウィンドウの横幅
	const int kWidth = 600;

	// ポーズウィンドウの縦幅
	const int kHeight = 200;

	// 透明度
	const int kAlpha = 200;
}

namespace BackTitleGraph
{
	// バックタイトルウィンドウのグラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Pause/BackTitle.png";
	// バックタイトルウィンドウのグラフィック距離
	const Vec2 kDistanceValue = Vec2(0, 10);
	// バックタイトルウィンドウの分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// バックタイトル中心座標
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX, Game::kWindowCenterY -50);
	// 拡大率
	const double kScale = 0.7;
}

namespace Graph
{
	// ポーズセレクトグラフィックファイルパス
	const char* const kPauseSelectFilePath = "Data/Graphic/Pause/PauseSelect.png";
	// ポーズセレクトグラフィックのグラフィック距離
	const Vec2 kPauseSelectDistanceValue = Vec2(0, 20);
	// ポーズセレクトの縦分割数
	const int kPauseSelectDivY = 4;
	// ポーズ中心座標
	const Vec2 kPauseCenterPos = Vec2(Game::kWindowCenterX+20, Game::kWindowCenterY);

	// バックタイトルセレクトグラフィックファイルパス
	const char* const kBackTitleSelectFilePath = "Data/Graphic/Selection/Selection.png";
	// バックタイトルセレクトグラフィックのグラフィック距離
	const Vec2 kBackTitleSelectDistanceValue = Vec2(80, 0);
	// バックタイトルセレクトの縦分割数
	const int kBackTitleSelectDivY = 2;
	// バックタイトル中心座標
	const Vec2 kBackTitleCenterPos = Vec2(Game::kWindowCenterX, Game::kWindowCenterY + 50);

	// 拡大率
	const double kScale = 0.8;

	// 透明度
	const int kAlpha = 100;

	// 選択されているグラフィックを左にずらす
	const float kSelectMoveX = -10;
}

namespace SelectTriangleGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Selection/SelectTriangle.png";

	// 拡大率
	const float kScale = 0.6f;

	// フレーム速度
	const int kFrameSpeed = 2;

	// ポーズ距離
	const Vec2 kPauseSelectDistanceValue = Vec2(-220, 0);

	// バックタイトル距離
	const Vec2 kBackTitleSelectDistanceValue = Vec2(-110, 0);
}


Pause::Pause():
	m_pauseSelect(),
	m_backTitleSelect(),
	m_windowNameGraph(),
	m_pauseSelectGprah(),
	m_backTitleGraph(),
	m_backTitleSelectGraph(),
	m_selectTriangleGraph(),
	m_pStateMachine(),
	m_pObjectFactory(),
	m_pControllerOption(std::make_shared<ControllerOption>()),
	m_pSoundOption(std::make_shared<SoundOption>())
{
}

Pause::~Pause()
{
	// グラフィックのデリート
	for (auto& handle : m_pauseSelectGprah.handle)
	{
		DeleteGraph(handle);
	};
	for (auto& handle : m_backTitleSelectGraph.handle)
	{
		DeleteGraph(handle);
	};
	for(auto& handle : m_windowNameGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& handle : m_backTitleGraph.handle)
	{
		DeleteGraph(handle);
	}
 
	DeleteGraph(m_selectTriangleGraph);
}

void Pause::Init()
{

	// コントローラーオプションの初期化
	m_pControllerOption->Init();

	// サウンドオプションの初期化
	m_pSoundOption->Init();


	// グラフィックのロード
	Load();

	// ステートマシンの初期化
	StateInit();



	m_pauseSelect = PauseSelect::Volume;

}

void Pause::Update()
{
	// ステートマシンの更新
	m_pStateMachine.Update();
}

void Pause::Draw()
{
	// ポーズウィンドウ描画
	DrawPauseWindow();

	// 選択描画
	DrawPauseSelect();


	// ステートマシンの描画
	m_pStateMachine.Draw();
}

void Pause::StateInit()
{

	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 通常ステート
	{
		auto enter = [this]() { StateNormalEnter(); };
		auto update = [this]() { StateNormalUpdate(); };
		
		m_pStateMachine.AddState(State::Normal, enter, update, dummy, dummy);
	}
	// ポーズステート
	{
		auto enter = [this]() { StatePauseEnter(); };
		auto update = [this]() { StatePauseUpdate(); };
		auto draw = [this]() { StatePauseDraw(); };
		auto exit = [this]() { StatePauseExit(); };

		m_pStateMachine.AddState(State::Pause, enter, update, draw, dummy);
	}
	// 入力切替ステート
	{
		auto enter = [this]() { StateChangeInputEnter(); };
		auto update = [this]() { StateChangeInputUpdate(); };
		auto draw = [this]() { StateChangeInputDraw(); };

		m_pStateMachine.AddState(State::ChangeInput, enter, update, draw, dummy);
	}
	// 音量調整ステート
	{
		auto enter = [this]() { StateSoundOptionEnter(); };
		auto update = [this]() { StateSoundOptionUpdate(); };
		auto draw = [this]() { StateSoundOptionDraw(); };

		m_pStateMachine.AddState(State::SoundOption, enter, update, draw, dummy);
	}
	// バックタイトルステート
	{
		auto enter = [this]() { StateBackTitleEnter(); };
		auto update = [this]() { StateBackTitleUpdate(); };
		auto draw = [this]() { StateBackTitleDraw(); };

		m_pStateMachine.AddState(State::BackTitle, enter, update, draw, dummy);
	}


	// 初期ステートを設定
	m_pStateMachine.SetState(State::Normal);
}

void Pause::StateNormalEnter()
{
	// ポーズ選択の初期化
	m_pauseSelect = PauseSelect::Resume;
	// バックタイトル選択の初期化
	m_backTitleSelect = BackTitleSelect::No;
}

void Pause::StateNormalUpdate()
{


	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);
		// オブジェクトファクトリーのステートをポーズに設定
		m_pObjectFactory->SetState(ObjectManager::State::Pause);

		// ポーズ音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Pause)]);
	}
}

void Pause::StatePauseEnter()
{
}

void Pause::StatePauseUpdate()
{
	// 選択更新
	PauseSelectUpdate();

	// 決定処理
	PauseSelectDecision();

	// キャンセルボタンが押されたらポーズ選択をゲーム再開セットする
	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::CANCEL))
	{
		m_pauseSelect = PauseSelect::Resume;

		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);
	}


	// ポーズボタンが押されたら通常ステートに遷移
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// 通常ステートに遷移
		ResumeProcess();

		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);
	}
}

void Pause::StatePauseDraw()
{
}

void Pause::StatePauseExit()
{

}

void Pause::StateChangeInputEnter()
{
	// コントローラーオプションの初期化
	m_pControllerOption->InitSettingItem();
}

void Pause::StateChangeInputUpdate()
{
	// 設定項目更新
	m_pControllerOption->Update();

	if(m_pControllerOption->GetIsCloseWindow())
	{
		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);
	}
}

void Pause::StateChangeInputDraw()
{
	// 設定項目描画
	m_pControllerOption->Draw();
}

void Pause::StateSoundOptionEnter()
{
	// サウンドオプションの初期化
	m_pSoundOption->InitSettingItem();
}

void Pause::StateSoundOptionUpdate()
{
	// 設定項目更新
	m_pSoundOption->Update();

	if (m_pSoundOption->GetIsCloseWindow())
	{
		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);
	}
}

void Pause::StateSoundOptionDraw()
{
	// 設定項目描画
	m_pSoundOption->Draw();
}

void Pause::StateBackTitleEnter()
{
}

void Pause::StateBackTitleUpdate()
{
	// バックタイトル選択更新
	BackTitleSelectUpdate();
	// バックタイトル決定処理
	BackTitleSelectDecision();
}

void Pause::StateBackTitleDraw()
{
	// バックタイトルウィンドウ描画
	DrawBackTitleWindow();

	// バックタイトル選択描画
	DrawBackTitleSelect();
}

void Pause::Load()
{

	// ウィンドウ名のグラフィックのロード
	{
		// グラフィックロード
		m_windowNameGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(WindowNameGraph::kFilePath, WindowNameGraph::kDivNum);

		// ウィンドウの上Y座標
		const float windowTop_Y = Game::kWindowCenterY - Window::kHeight / 2;


		// 座標の設定
		Vec2 pos = Vec2();
		pos.x = Game::kWindowCenterX;
		pos.y = windowTop_Y;

		// 座標の設定
		m_windowNameGraph.pos.push_back(pos);
	}

	// ポーズ選択グラフィック
	{
		// ポーズ選択グラフィック
		m_pauseSelectGprah.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(Graph::kPauseSelectFilePath, EvoLib::Load::DivNum(1, 4));

		// グラフィックの座標を均等に配置
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_pauseSelectGprah.handle, Graph::kScale);

		m_pauseSelectGprah.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, Graph::kPauseCenterPos, static_cast<int>(m_pauseSelectGprah.handle.size()), Graph::kPauseSelectDistanceValue, false);
	}

	// バックタイトルグラフィック
	{
		// バックタイトルグラフィック
		m_backTitleGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(BackTitleGraph::kFilePath, BackTitleGraph::kDivNum);

		// グラフィックを配置
		m_backTitleGraph.pos.push_back(BackTitleGraph::kCenterPos);
	}

	// バックタイトル選択グラフィック
	{
		// バックタイトル選択グラフィック
		m_backTitleSelectGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(Graph::kBackTitleSelectFilePath, EvoLib::Load::DivNum(1, 2));

		// グラフィックの座標を均等に配置
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_backTitleSelectGraph.handle, Graph::kScale);

		m_backTitleSelectGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, Graph::kBackTitleCenterPos, static_cast<int>(m_backTitleSelectGraph.handle.size()), Graph::kBackTitleSelectDistanceValue, true);
	}

	// セレクト三角形グラフィック
	{
		// グラフィックロード
		m_selectTriangleGraph = LoadGraph(SelectTriangleGraph::kFilePath);
	}
}

void Pause::PauseSelectUpdate()
{
	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) - 1 + static_cast<int>(PauseSelect::SelectNum)) % static_cast<int>(PauseSelect::SelectNum));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) + 1) % static_cast<int>(PauseSelect::SelectNum));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void Pause::PauseSelectDecision()
{
	// 決定ボタンが押されていなければreturn
	if(!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}


	if (m_pauseSelect != PauseSelect::Resume)
	{
		// 決定音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Decision)]);
	}
	else
	{
		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);
	}


	switch (m_pauseSelect)
	{
	case PauseSelect::Resume:

		// 通常ステートに遷移
		ResumeProcess();

		break;
	case PauseSelect::Volume:

		// サウンドオプションステートに遷移
		m_pStateMachine.SetState(State::SoundOption);

		break;
	case PauseSelect::ChangeInput:

		// 入力切替ステートに遷移
		m_pStateMachine.SetState(State::ChangeInput);

		break;
	case PauseSelect::Title:

		// バックタイトル選択に遷移
		m_pStateMachine.SetState(State::BackTitle);

		break;
	
	default:

		// 通常ステートに遷移
		ResumeProcess();

		break;
	}
}

void Pause::BackTitleSelectUpdate()
{


	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{
		m_backTitleSelect = static_cast<BackTitleSelect>((static_cast<int>(m_backTitleSelect) - 1 + static_cast<int>(BackTitleSelect::SelectNum)) % static_cast<int>(BackTitleSelect::SelectNum));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		m_backTitleSelect = static_cast<BackTitleSelect>((static_cast<int>(m_backTitleSelect) + 1) % static_cast<int>(BackTitleSelect::SelectNum));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void Pause::BackTitleSelectDecision()
{

	// 決定ボタンが押されていなければreturn
	if (!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}

	switch (m_backTitleSelect)
	{
	case BackTitleSelect::Yes:

		// タイトルに遷移
		m_pObjectFactory->ChangeScene(SceneMain::Scene::Title);

		// 操作受付を無効にする
		Controller::GetInstance()->SetAcceptInput(false);

		// 決定音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Decision)]);

		break;
	case BackTitleSelect::No:

		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);

		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		break;
	default:

		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);

		break;
	}
}


void Pause::DrawPauseWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - Window::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - Window::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + Window::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + Window::kHeight / 2;


	// 透明度設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);

	// ポーズウィンドウ
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズウィンドウ枠
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);


	// ウィンドウ名描画
	DrawRotaGraphF(m_windowNameGraph.pos[0].x, m_windowNameGraph.pos[0].y, WindowNameGraph::kScale, 0.0, m_windowNameGraph.handle[0], true);
}

void Pause::DrawPauseSelect()
{

	// ポーズ選択描画
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Graph::kAlpha);

		// 選ばれている選択肢は赤色で描画し、それ以外は半透明で描画
		for (int i = 0; i < static_cast<int>(PauseSelect::SelectNum); i++)
		{
			if (m_pauseSelect != static_cast<PauseSelect>(i))
			{
				DrawRotaGraphF(m_pauseSelectGprah.pos[i].x, m_pauseSelectGprah.pos[i].y, Graph::kScale, 0.0, m_pauseSelectGprah.handle[i], true);
			}
		}

		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 選ばれている選択肢は赤色で描画
	SetDrawBright(255, 0, 0);

	// 選択描画
	{
		DrawRotaGraphF(m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].x + Graph::kSelectMoveX, m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].y, Graph::kScale, 0.0, m_pauseSelectGprah.handle[static_cast<int>(m_pauseSelect)], true);
	}

	// 選択されている三角形描画
	{
		Vec2 pos = Vec2();
		pos.x = (m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].x + Graph::kSelectMoveX);
		pos.y = m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].y;

		pos += SelectTriangleGraph::kPauseSelectDistanceValue;

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

void Pause::DrawBackTitleSelect()
{

	// バックタイトル描画
	DrawRotaGraphF(m_backTitleGraph.pos[0].x, m_backTitleGraph.pos[0].y, BackTitleGraph::kScale, 0.0, m_backTitleGraph.handle[0], true);


	// バックタイトル選択描画
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Graph::kAlpha);

		// 選ばれている選択肢は赤色で描画し、それ以外は半透明で描画
		for (int i = 0; i < static_cast<int>(BackTitleSelect::SelectNum); i++)
		{
			if (m_backTitleSelect != static_cast<BackTitleSelect>(i))
			{
				DrawRotaGraphF(m_backTitleSelectGraph.pos[i].x, m_backTitleSelectGraph.pos[i].y, Graph::kScale, 0.0, m_backTitleSelectGraph.handle[i], true);
			}
		}

		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 選ばれている選択肢は赤色で描画
	SetDrawBright(255, 0, 0);

	// 選択描画
	{
		DrawRotaGraphF(m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].x, m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].y, Graph::kScale, 0.0, m_backTitleSelectGraph.handle[static_cast<int>(m_backTitleSelect)], true);	
	}

	// 選択されている三角形描画
	{
		Vec2 pos = Vec2();
		pos.x = m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].x;
		pos.y = m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].y;

		pos += SelectTriangleGraph::kBackTitleSelectDistanceValue;

		// 回転描画
		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			pos,
			SelectTriangleGraph::kFrameSpeed,
			false,
			SelectTriangleGraph::kScale,
			true,
			false,
			true,
			false);
	}

	// 色を元に戻す
	SetDrawBright(255, 255, 255);
}

void Pause::ResumeProcess()
{
	// 通常ステートに遷移
	m_pStateMachine.SetState(State::Normal);
	// オブジェクトファクトリーのステートを通常に設定
	m_pObjectFactory->SetState(ObjectManager::State::Normal);
}

void Pause::DrawBackTitleWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - BackTitleWindow::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - BackTitleWindow::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + BackTitleWindow::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + BackTitleWindow::kHeight / 2;





	// 透明度設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BackTitleWindow::kAlpha);

	// 画面全体を黒で塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);


	// ポーズウィンドウ
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズウィンドウ枠
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);

	
}
