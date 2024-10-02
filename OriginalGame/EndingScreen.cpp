#include "EndingScreen.h"
#include "SceneEnding.h"
#include "Controller.h"
#include "game.h"
#include "GameData.h"

namespace Graph
{
	// ゲームクリア画像
	const char* kGameClearGraphFileName = "Data/Graphic/Ending/GameClear.png";
	// ゲームクリア画像の位置
	const Vec2 kGameClearGraphPos = Vec2(Game::kWindowCenterX_F, Game::kWindowCenterY_F - 290.0f);
}

namespace SineCurve
{
	// 最大フレーム
	constexpr int kMaxFrame = 100;

	constexpr int kSineCurveSpeed = 1;

	constexpr float kMaxSineCurveValue = 10.0f;
}

namespace ScoreGraph
{
	// 番号グラフィックのファイル名
	const char* kNumberGraphFileName = "Data/Graphic/Number/Number.png";
	// スコアグラフィックの分割数
	const EvoLib::Load::DivNum kNumberGraphDivNum = EvoLib::Load::DivNum(10, 1);

	// 仕切りグラフィックのファイル名
	const char* kSeparateGraphFileName = "Data/Graphic/Number/Separate.png";
	// カウントグラフィックのファイル名
	const char* kCountGraphFileName = "Data/Graphic/Number/Count.png";

	// クリアリトライ回数テキストグラフィックのファイル名
	const char* kClearRetryTextGraphFileName = "Data/Graphic/Ending/ClearRetryText.png";

	// サンクスグラフィックのファイル名
	const char* kThanksGraphFileName = "Data/Graphic/Ending/Thanks.png";

	// 新記録グラフィックのファイル名
	const char* kNewRecordGraphFileName = "Data/Graphic/Ending/NewRecord.png";


	// タイムスコアの位置
	const Vec2 kTimeScorePos = Vec2(Game::kWindowCenterX_F + 40.0f, Game::kWindowCenterY_F - 70.0f);
	// デスカウントの位置
	const Vec2 kDeathCountPos = Vec2(Game::kWindowCenterX_F + 40.0f, Game::kWindowCenterY_F + 100.0f);

	// グラフィック同士の感覚
	const float kTextGraphInterval = -250.0f;

	// クリアリトライ回数テキストの位置
	const Vec2 kClearTextPos = Vec2(kTimeScorePos.x + kTextGraphInterval, kTimeScorePos.y);
	const Vec2 kRetryTextPos = Vec2(kDeathCountPos.x + kTextGraphInterval, kDeathCountPos.y);

	// サンクスグラフィックの位置
	const Vec2 kThanksGraphPos = Vec2(Game::kWindowCenterX_F, Game::kWindowCenterY_F + 300.0f);
	// サンクスグラフィックのサイズ
	constexpr double kThanksGraphSize = 1.0;

	// 新記録のグラフィックの位置
	const Vec2 kNewRecordGraphPos = Vec2(250, Game::kWindowCenterY_F - 150.0f);
	// 新記録のグラフィックのサイズ
	constexpr double kNewRecordGraphSize = 0.5;



	// テキストグラフィックサイズ
	constexpr double kTextGraphSize = 1.0;

	// グラフィック同士の間隔
	static float kGraphInterval = 0.0f;
	// グラフィック同士の間隔の設定
	const float kGraphIntervalSetting = -50.0f;

	// グラフィックサイズ
	constexpr double kGraphSize = 1.1;
}

EndingScreen::EndingScreen() :
	m_gameClearGraphHandle(-1),
	m_scoreGraphData(),
	m_thanksGraphHandle(-1),
	m_newRecordGraphHandle(-1),
	m_isNewRecord(false),
	m_score(),
	m_gameClearGraphSineCuve(),
	m_gameClearGraphSineCurveValue(0.0f),
	m_pSceneEnding(nullptr)
{
}

EndingScreen::~EndingScreen()
{
	// 画像ハンドル解放
	DeleteGraph(m_gameClearGraphHandle);
	for(auto& handle : m_scoreGraphData.numberGraphHandle)
	{
		DeleteGraph(handle);
	}
	DeleteGraph(m_scoreGraphData.separateGraphHandle);
	DeleteGraph(m_scoreGraphData.countGraphHandle);
	for(auto& handle : m_scoreGraphData.clearRetryTextGraphHandle)
	{
		DeleteGraph(handle);
	}
	DeleteGraph(m_thanksGraphHandle);
	DeleteGraph(m_newRecordGraphHandle);
}

void EndingScreen::Init()
{
	// 画像ロード
	Load();


	// サインカーブのデータの初期化
	m_gameClearGraphSineCuve.sineCurrentFrame = 0;
	m_gameClearGraphSineCuve.sineMaxFrame = SineCurve::kMaxFrame;
	m_gameClearGraphSineCuve.sineMaxValue = SineCurve::kMaxSineCurveValue;


	// 新記録かどうかの判定
	{
		// 新記録かどうかの判定
		m_isNewRecord = GameData::GetInstance()->IsNewRecord();
	}

	if(m_isNewRecord)
	{
		// 新記録の場合、セーブデータを更新
		GameData::GetInstance()->SetScoreData();
	}


	m_score = GameData::GetInstance()->
		CalcScore(
			GameData::GetInstance()->GetTime(),
			GameData::GetInstance()->GetDeathCount());
}

void EndingScreen::Update()
{
	// サインカーブの更新
	UpdateSineCurve();

	// ボタンが押されたらフェードアウトを設定する
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		m_pSceneEnding->ChangeScene(SceneEnding::Scene::Title);
	}
}

void EndingScreen::Draw()
{	
	// 背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xa9a9a9, true);

	// ゲームクリア画像
	DrawRotaGraphF(
		Graph::kGameClearGraphPos.x, 
		Graph::kGameClearGraphPos.y + m_gameClearGraphSineCurveValue,
		1.0,
		0.0,
		m_gameClearGraphHandle, TRUE);

	// スコアの描画
	DrawScore();


	// サンクス画像
	DrawRotaGraphF(
		ScoreGraph::kThanksGraphPos.x,
		ScoreGraph::kThanksGraphPos.y,
		ScoreGraph::kThanksGraphSize,
		0.0,
		m_thanksGraphHandle,
		TRUE);
}

void EndingScreen::Load()
{
	// ゲームクリア画像ロード
	m_gameClearGraphHandle = LoadGraph(Graph::kGameClearGraphFileName);

	// スコアグラフィックロード
	{
		m_scoreGraphData.numberGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph::kNumberGraphFileName, ScoreGraph::kNumberGraphDivNum);

		m_scoreGraphData.separateGraphHandle = LoadGraph(ScoreGraph::kSeparateGraphFileName);
		m_scoreGraphData.countGraphHandle = LoadGraph(ScoreGraph::kCountGraphFileName);
		
		// クリアリトライ回数テキストグラフィックロード
		m_scoreGraphData.clearRetryTextGraphHandle =
			EvoLib::Load::LoadDivGraph_EvoLib_Revision(ScoreGraph::kClearRetryTextGraphFileName, EvoLib::Load::DivNum(1, 2));

		// サンクスグラフィックのロード
		m_thanksGraphHandle = LoadGraph(ScoreGraph::kThanksGraphFileName);

		{
			// グラフィックのサイズを取得
			const Vec2 size = EvoLib::Calculation::GetGraphSize_EvoLib(m_scoreGraphData.numberGraphHandle, ScoreGraph::kGraphSize);

			// グラフィックの間隔を設定
			ScoreGraph::kGraphInterval = size.x + ScoreGraph::kGraphIntervalSetting;
		}

		// 新記録のグラフィックのロード
		m_newRecordGraphHandle = LoadGraph(ScoreGraph::kNewRecordGraphFileName);
	}


}

void EndingScreen::UpdateSineCurve()
{
	// サインカーブの更新
	m_gameClearGraphSineCurveValue =
		EvoLib::Calculation::SineCurve<float>(m_gameClearGraphSineCuve);
}

void EndingScreen::DrawScore()
{


	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph::kClearTextPos.x,
		ScoreGraph::kClearTextPos.y,
		ScoreGraph::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[0],
		TRUE);


	// クリアリトライ回数テキストの描画
	DrawRotaGraphF(
		ScoreGraph::kRetryTextPos.x,
		ScoreGraph::kRetryTextPos.y,
		ScoreGraph::kTextGraphSize,
		0.0,
		m_scoreGraphData.clearRetryTextGraphHandle[1],
		TRUE);

	// 新記録が出ていた場合、赤色で描画
	if (m_isNewRecord)
	{
		// 新記録の描画
		DrawRotaGraphF(
			ScoreGraph::kNewRecordGraphPos.x,
			ScoreGraph::kNewRecordGraphPos.y,
			ScoreGraph::kNewRecordGraphSize,
			0.0,
			m_newRecordGraphHandle,
			TRUE);




		// 赤色に変更
		SetDrawBright(255, 0, 0);
	}



	// 時間スコア
	{
		// 時間スコアの描画
		const int loopNum = static_cast<int>(m_score.time.size());

		int count = 0;

		for (int i = 0; i < loopNum; i++)
		{
			if (i != 0 && i % 2 == 0)
			{
				const float x = ScoreGraph::kTimeScorePos.x + ((i + count) * ScoreGraph::kGraphInterval);

				DrawRotaGraphF(
					x,
					ScoreGraph::kTimeScorePos.y,
					ScoreGraph::kGraphSize,
					0.0,
					m_scoreGraphData.separateGraphHandle,
					TRUE);

				count++;
			}

			const float x = ScoreGraph::kTimeScorePos.x + ((i + count) * ScoreGraph::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph::kTimeScorePos.y,
				ScoreGraph::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[m_score.time[i]],
				TRUE);
		}
	}

	// デスカウント
	{
		// デスカウントの描画
		const int loopNum = static_cast<int>(m_score.deathCount.size());

		for (int i = 0; i < loopNum; i++)
		{
			

			const float x = ScoreGraph::kDeathCountPos.x + (i * ScoreGraph::kGraphInterval);

			DrawRotaGraphF(
				x,
				ScoreGraph::kDeathCountPos.y,
				ScoreGraph::kGraphSize,
				0.0,
				m_scoreGraphData.numberGraphHandle[m_score.deathCount[i]],
				TRUE);
		}
		
		float x = ScoreGraph::kDeathCountPos.x + (loopNum * ScoreGraph::kGraphInterval);
		x += 20.0f;

		DrawRotaGraphF(
			x,
			ScoreGraph::kDeathCountPos.y,
			ScoreGraph::kGraphSize,
			0.0,
			m_scoreGraphData.countGraphHandle,
			TRUE);
	}

	// 色を元に戻す
	SetDrawBright(255, 255, 255);


}
