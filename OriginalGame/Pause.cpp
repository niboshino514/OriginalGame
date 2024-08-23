#include "Pause.h"
#include "game.h"
#include <DxLib.h>
#include "Controller.h"
#include "ObjectManager.h"
#include "EvoLib.h"
#include "ControllerOption.h"

namespace Window
{
	// ポーズウィンドウの横幅
	const int kWidth = 700;

	// ポーズウィンドウの縦幅
	const int kHeight = 500;

	// 透明度
	const int kAlpha = 200;
}

namespace Graph
{
	// ポーズセレクトグラフィックファイルパス
	const char* const kPauseSelectFilePath = "Data/Pause/PauseSelect.png";
	// ポーズセレクトグラフィックのグラフィック距離
	const Vec2 kPauseSelectDistanceValue = Vec2(0, 10);
	// ポーズセレクトの縦分割数
	const int kPauseSelectDivY = 4;
	// ポーズ中心座標
	const Vec2 kPauseCenterPos = Vec2(Game::kWindowCenterX, Game::kWindowCenterY);

	// バックタイトルセレクトグラフィックファイルパス
	const char* const kBackTitleSelectFilePath = "Data/Selection/Selection.png";
	// バックタイトルセレクトグラフィックのグラフィック距離
	const Vec2 kBackTitleSelectDistanceValue = Vec2(50, 0);
	// バックタイトルセレクトの縦分割数
	const int kBackTitleSelectDivY = 2;
	// バックタイトル中心座標
	const Vec2 kBackTitleCenterPos = Vec2(kPauseCenterPos.x, kPauseCenterPos.y+200);

	// 拡大率
	const double kScale = 0.8;

	// 透明度
	const int kAlpha = 100;
}

Pause::Pause():
	m_pauseSelect(),
	m_backTitleSelect(),
	m_pauseSelectGprah(),
	m_backTitleSelectGraph(),
	m_pStateMachine(),
	m_pObjectFactory(),
	m_pControllerOption(std::make_shared<ControllerOption>())
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

}

void Pause::Init()
{

	// コントローラーオプションの初期化
	m_pControllerOption->Init();

	// グラフィックのロード
	LoadGraph();

	// ステートマシンの初期化
	StateInit();



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
	PauseSelectDraw();

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

	////////// test
	{
		// オブジェクトファクトリーのステートをポーズに設定
		m_pObjectFactory->SetState(ObjectManager::State::Pause);
		// 入力切替ステートに遷移
		m_pStateMachine.SetState(State::Pause);
		m_pauseSelect = PauseSelect::ChangeInput;
	}






	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);
		// オブジェクトファクトリーのステートをポーズに設定
		m_pObjectFactory->SetState(ObjectManager::State::Pause);
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


	// ポーズボタンが押されたら通常ステートに遷移
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::PAUSE))
	{
		// 通常ステートに遷移
		ResumeProcess();
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
}

void Pause::StateChangeInputUpdate()
{
	// 設定項目更新
	m_pControllerOption->Update();
}

void Pause::StateChangeInputDraw()
{
	// 設定項目描画
	m_pControllerOption->Draw();
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
	// バックタイトル選択描画
	BackTitleSelectDraw();
}

void Pause::LoadGraph()
{
	// ポーズ選択グラフィック
	{
		// ポーズ選択グラフィック
		m_pauseSelectGprah.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision(Graph::kPauseSelectFilePath, EvoLib::Load::DivNum(1, 4));

		// グラフィックの座標を均等に配置
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_pauseSelectGprah.handle, Graph::kScale);

		m_pauseSelectGprah.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, Graph::kPauseCenterPos, static_cast<int>(m_pauseSelectGprah.handle.size()), Graph::kPauseSelectDistanceValue, false);
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
}

void Pause::PauseSelectUpdate()
{
	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) - 1 + static_cast<int>(PauseSelect::SelectNum)) % static_cast<int>(PauseSelect::SelectNum));
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		m_pauseSelect = static_cast<PauseSelect>((static_cast<int>(m_pauseSelect) + 1) % static_cast<int>(PauseSelect::SelectNum));
	}
}

void Pause::PauseSelectDecision()
{
	// 決定ボタンが押されていなければreturn
	if(!Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DECIDE))
	{
		return;
	}



	switch (m_pauseSelect)
	{
	case PauseSelect::Resume:

		// 通常ステートに遷移
		ResumeProcess();

		break;
	case PauseSelect::Volume:
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
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		m_backTitleSelect = static_cast<BackTitleSelect>((static_cast<int>(m_backTitleSelect) + 1) % static_cast<int>(BackTitleSelect::SelectNum));
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

		break;
	case BackTitleSelect::No:

		// ポーズステートに遷移
		m_pStateMachine.SetState(State::Pause);

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
}

void Pause::PauseSelectDraw()
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

	// 選択描画
	{
		// 選ばれている選択肢は赤色で描画
		SetDrawBright(255, 0, 0);

		DrawRotaGraphF(m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].x, m_pauseSelectGprah.pos[static_cast<int>(m_pauseSelect)].y, Graph::kScale, 0.0, m_pauseSelectGprah.handle[static_cast<int>(m_pauseSelect)], true);

		// 色を元に戻す
		SetDrawBright(255, 255, 255);
	}
}

void Pause::BackTitleSelectDraw()
{

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

	// 選択描画
	{
		// 選ばれている選択肢は赤色で描画
		SetDrawBright(255, 0, 0);

		DrawRotaGraphF(m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].x, m_backTitleSelectGraph.pos[static_cast<int>(m_backTitleSelect)].y, Graph::kScale, 0.0, m_backTitleSelectGraph.handle[static_cast<int>(m_backTitleSelect)], true);

		// 色を元に戻す
		SetDrawBright(255, 255, 255);
	}

}

void Pause::ResumeProcess()
{
	// 通常ステートに遷移
	m_pStateMachine.SetState(State::Normal);
	// オブジェクトファクトリーのステートを通常に設定
	m_pObjectFactory->SetState(ObjectManager::State::Normal);
}
