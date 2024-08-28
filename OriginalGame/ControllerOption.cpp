#include "ControllerOption.h"
#include <DxLib.h>
#include "game.h"
#include "EvoLib.h"
#include "Pad.h"
#include "Sound.h"
#include "ObjectManager.h"

namespace Window
{
	// ポーズウィンドウの横幅
	const int kWidth = 900;

	// ポーズウィンドウの縦幅
	const int kHeight = 650;

	// 透明度
	const int kAlpha = 150;
}



namespace WindowName
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/ControllerOption/WindowName.png";
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// 拡大率
	const double kScale = 1.0;
}


namespace SettingItem
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/ControllerOption/SettingItem.png";
	// グラフィックのグラフィック距離
	const Vec2 kDistanceValue = Vec2(0, 40);
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 3 };
	// 中心座標
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX-200, Game::kWindowCenterY-120);
	// 拡大率
	const double kScale = 0.5;
	// 透明度
	const int kAlpha = 100;
}

namespace Back
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/ControllerOption/Back.png";
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// 拡大率
	const double kScale = 0.5;
}

namespace SelectTriangleGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Selection/SelectTriangle.png";

	// 拡大率
	const float kScale = 0.6f;

	// 自動切換え距離
	const Vec2 kAutoSwitchDistance = Vec2(-90, 0);

	// コントローラータイプ選択距離
	const Vec2 kCtrlTypeDistance= Vec2(-170, 0);

	// 入力デバイス切替距離
	const Vec2 kInputDeviceDistance = Vec2(-150, 0);
}

namespace
{
	struct ControllerSettingGraphInfo
	{
		// グラフィックファイルパス
		const char* const kFilePath = "";
		// 分割数
		const EvoLib::Load::DivNum kDivNum = { 1, 1 };
		// 設定項目からの距離
		const Vec2 kDistanceValue = Vec2(450, 0);
		// 拡大率
		const double kScale = 0.5;
	};



	// 設定項目
	const std::vector<ControllerSettingGraphInfo> m_controllerSettingGraphInfo =
	{
		// コントローラー自動切換え
		{
			"Data/ControllerOption/Switch.png",
			{ 1, 2 },
		
		},
		// コントローラータイプ選択
		{
			"Data/ControllerOption/ControllerType.png",
			{ 1, 3 },
		
		},
		// 入力デバイス切替
		{
			"Data/ControllerOption/InputDevice.png",
			{ 1, 2 },
		
		},
	};

	// 透明度
	const int kAlpha = 100;
}




ControllerOption::ControllerOption():
	m_settingItemSenect(),
	m_controllerSetting(),
	isCloseWindow(),
	m_settingItemGraph(),
	m_controllerSettingGraph(),
	m_windowNameGraph(),
	m_backGraph(),
	m_selectTriangleGraph()
{
}

ControllerOption::~ControllerOption()
{
	// ハンドルの解放
	for(auto& handle : m_settingItemGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& graph : m_controllerSettingGraph)
	{
		for(auto& handle : graph.handle)
		{
			DeleteGraph(handle);
		}
	}
	for(auto& handle : m_windowNameGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& handle : m_backGraph.handle)
	{
		DeleteGraph(handle);
	}
	DeleteGraph(m_selectTriangleGraph);
}

void ControllerOption::Init()
{
	// グラフィックのロード
	Load();
}

void ControllerOption::Update()
{
	// コントローラー設定取得
	m_controllerSetting = Controller::GetInstance()->GetControllerSetting();

	// 選択項目の更新
	UpdateSettingItem();

	// コントローラー設定の更新
	UpdateControllerSetting();

	// キャンセルボタンが押されたらウィンドウを閉じる
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::CANCEL))
	{
		isCloseWindow = true;

		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);
	}
}

void ControllerOption::Draw()
{
	// ウィンドウの描画
	DrawWindow();

	// 設定項目の描画
	DrawSettingItem();

	// コントローラー設定の描画
	DrawControllerSetting();

	// コントローラー説明の描画
	DrawControllerExplanation();


	// 戻るボタンの描画
	DrawRotaGraphF(m_backGraph.pos[0].x, m_backGraph.pos[0].y, Back::kScale, 0.0, m_backGraph.handle[0], TRUE);
}

void ControllerOption::InitSettingItem()
{
	// 選択項目の初期化
	m_settingItemSenect = SettingItemSenect::AUTO_SWITCH;

	// ウィンドウを閉じるかどうか
	isCloseWindow = false;
}

void ControllerOption::Load()
{
	// ウィンドウ名のグラフィックのロード
	{
		// グラフィックロード
		m_windowNameGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(WindowName::kFilePath, WindowName::kDivNum);

		// ウィンドウの上Y座標
		const float windowTop_Y = Game::kWindowCenterY - Window::kHeight / 2;

		
		// 座標の設定
		Vec2 pos = Vec2();
		pos.x = Game::kWindowCenterX;
		pos.y = windowTop_Y;

		// 座標の設定
		m_windowNameGraph.pos.push_back(pos);
	}


	// 設定項目のグラフィックのロード
	{
		// グラフィックロード
		m_settingItemGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(SettingItem::kFilePath, SettingItem::kDivNum);

		// 座標の設定
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_settingItemGraph.handle, SettingItem::kScale);

		// 座標の設定
		m_settingItemGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, SettingItem::kCenterPos, static_cast<int>(m_settingItemGraph.handle.size()), SettingItem::kDistanceValue, false);

	}
	
	{
		// セレクト数
		const int selectNum = static_cast<int>(SettingItemSenect::SELECT_NUM);

		// リサイズ
		m_controllerSettingGraph.resize(selectNum);

		for (int i = 0; i < selectNum; i++)
		{
			// 設定ナンバー
			const int settingNum = static_cast<int>(SettingItemSenect(i));


			// グラフィックロード
			m_controllerSettingGraph[i].handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
			(m_controllerSettingGraphInfo[settingNum].kFilePath, m_controllerSettingGraphInfo[settingNum].kDivNum);

			// 座標の設定
			const Vec2 pos = m_settingItemGraph.pos[i] + m_controllerSettingGraphInfo[settingNum].kDistanceValue;

			// 座標の設定
			m_controllerSettingGraph[i].pos.push_back(pos);
		}
	}

	// バックグラフィックのロード
	{
		// グラフィックロード
		m_backGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(Back::kFilePath,Back::kDivNum);

		// ウィンドウの下Y座標
		const float windowBottom_Y = Game::kWindowCenterY + Window::kHeight / 2;
		// ウィンドウの右X座標
		const float windowRight_X = Game::kWindowCenterX + Window::kWidth / 2;

		// 座標の設定
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_backGraph.handle, Back::kScale);


		// 座標の設定
		Vec2 pos = Vec2();
		pos.x = windowRight_X - (graphSize.x * 0.5f);
		pos.y = windowBottom_Y - (graphSize.y * 0.5f);

		// 座標の設定
		m_backGraph.pos.push_back(pos);
	}

	// 選択三角形のグラフィックのロード
	m_selectTriangleGraph = LoadGraph(SelectTriangleGraph::kFilePath);
}

void ControllerOption::UpdateSettingItem()
{
	// セレクト数
	int selectNum = static_cast<int>(SettingItemSenect::SELECT_NUM);

	// オートスイッチがONの場合、選択肢を一つ減らす
	if (m_controllerSetting.autoSwitch == Controller::AutoSwitch::ON)
	{
		selectNum--;
	}


	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		// 選択項目の更新
		m_settingItemSenect = static_cast<SettingItemSenect>((static_cast<int>(m_settingItemSenect) + 1) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		// 選択項目の更新
		m_settingItemSenect = static_cast<SettingItemSenect>((static_cast<int>(m_settingItemSenect) - 1 + selectNum) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void ControllerOption::UpdateControllerSetting()
{
	// 選択項目によって処理を分ける
	switch (m_settingItemSenect)
	{
	case ControllerOption::SettingItemSenect::AUTO_SWITCH:

		// オートスイッチがONの場合
		UpdateAutoSwitch();
		break;
	case ControllerOption::SettingItemSenect::CTRL_TYPE:

		// コントローラータイプ選択
		UpdateCtrlType();
		break;
	case ControllerOption::SettingItemSenect::INPUT_DEVICE:

		// 入力デバイス切替
		UpdateInputDevice();
		break;
	default:

		// オートスイッチがONの場合
		UpdateAutoSwitch();
		break;
	}
}

void ControllerOption::UpdateAutoSwitch()
{
	// セレクト数
	const int selectNum = static_cast<int>(Controller::AutoSwitch::AUTO_SWITCH_NUM);

	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		// 選択項目の更新
		m_controllerSetting.autoSwitch = static_cast<Controller::AutoSwitch>((static_cast<int>(m_controllerSetting.autoSwitch) + 1) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{
		// 選択項目の更新
		m_controllerSetting.autoSwitch = static_cast<Controller::AutoSwitch>((static_cast<int>(m_controllerSetting.autoSwitch) - 1 + selectNum) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void ControllerOption::UpdateCtrlType()
{
	// セレクト数
	const int selectNum = static_cast<int>(Controller::PadType::PAD_TYPE_NUM);

	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		// 選択項目の更新
		m_controllerSetting.padType = static_cast<Controller::PadType>((static_cast<int>(m_controllerSetting.padType) + 1) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{
		// 選択項目の更新
		m_controllerSetting.padType = static_cast<Controller::PadType>((static_cast<int>(m_controllerSetting.padType) - 1 + selectNum) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void ControllerOption::UpdateInputDevice()
{
	// セレクト数
	const int selectNum = static_cast<int>(Controller::ControllerType::CONTROLLER_NUM);

	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		// 選択項目の更新
		m_controllerSetting.controllerType = static_cast<Controller::ControllerType>((static_cast<int>(m_controllerSetting.controllerType) + 1) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		// 選択項目の更新
		m_controllerSetting.controllerType = static_cast<Controller::ControllerType>((static_cast<int>(m_controllerSetting.controllerType) - 1 + selectNum) % selectNum);

		// コントローラー設定の更新
		Controller::GetInstance()->SetControllerSetting(m_controllerSetting);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void ControllerOption::DrawWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - Window::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - Window::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + Window::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + Window::kHeight / 2;


	// 透明度設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);

	DrawBox(0,0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	// ポーズウィンドウ
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズウィンドウ枠
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);




	// ウィンドウ名の描画
	DrawRotaGraphF(m_windowNameGraph.pos[0].x, m_windowNameGraph.pos[0].y, WindowName::kScale, 0.0, m_windowNameGraph.handle[0], TRUE);
}

void ControllerOption::DrawSettingItem()
{
	// 設定項目の描画
	for (size_t i = 0; i < m_settingItemGraph.handle.size(); i++)
	{
		if (m_controllerSetting.autoSwitch == Controller::AutoSwitch::ON &&
			SettingItemSenect(i) == SettingItemSenect::INPUT_DEVICE)
		{
			// 透明度設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, SettingItem::kAlpha);
		}

		DrawRotaGraphF(m_settingItemGraph.pos[i].x, m_settingItemGraph.pos[i].y, SettingItem::kScale, 0.0, m_settingItemGraph.handle[i], TRUE);

		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ControllerOption::DrawControllerSetting()
{
	// グラフィック数
	const int selectNum = static_cast<int>(m_controllerSettingGraph.size());

	// 設定項目の描画
	for (int i = 0; i < selectNum; i++)
	{



		// 設定ナンバー
		const int settingNum = static_cast<int>(SettingItemSenect(i));
		// 画像ナンバー
		int graphNum = 0;

		switch (SettingItemSenect(i))
		{
		case ControllerOption::SettingItemSenect::AUTO_SWITCH:
			// 画像ナンバー
			graphNum = static_cast<int>(m_controllerSetting.autoSwitch);
			break;
		case ControllerOption::SettingItemSenect::CTRL_TYPE:
			// 画像ナンバー
			graphNum = static_cast<int>(m_controllerSetting.padType);
			break;
		case ControllerOption::SettingItemSenect::INPUT_DEVICE:
			// 画像ナンバー
			graphNum = static_cast<int>(m_controllerSetting.controllerType);
			break;
		default:
			// 画像ナンバー
			graphNum = 0;
			break;
		}

		// 選ばれている選択肢は赤色で描画
		if(m_settingItemSenect == SettingItemSenect(i))
		{
			// 選ばれている選択肢は赤色で描画
			SetDrawBright(255, 0, 0);

			Vec2 trianglePos = m_controllerSettingGraph[i].pos[0];

			Vec2 pos = Vec2();


			switch (m_settingItemSenect)
			{
			case ControllerOption::SettingItemSenect::AUTO_SWITCH:

				pos += SelectTriangleGraph::kAutoSwitchDistance;
				break;
			case ControllerOption::SettingItemSenect::CTRL_TYPE:

				pos += SelectTriangleGraph::kCtrlTypeDistance;
				break;
			case ControllerOption::SettingItemSenect::INPUT_DEVICE:

				pos += SelectTriangleGraph::kInputDeviceDistance;
				break;
			
			default:
				break;
			}

			EvoLib::Draw::DrawRotatingImage(
				m_selectTriangleGraph,
				Vec2(trianglePos.x + pos.x, trianglePos.y + pos.y),
				0,
				false,
				SelectTriangleGraph::kScale,
				true,
				true,
				true,
				false);

			EvoLib::Draw::DrawRotatingImage(
				m_selectTriangleGraph,
				Vec2(trianglePos.x - pos.x, trianglePos.y - pos.y),
				0,
				false,
				SelectTriangleGraph::kScale,
				true,
				false,
				true,
				false);

			
		}
		else
		{
			// 透明度設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);
		}

		
		


		// グラフィックの描画
		DrawRotaGraphF(
			m_controllerSettingGraph[i].pos[0].x, 
			m_controllerSettingGraph[i].pos[0].y, 
			m_controllerSettingGraphInfo[settingNum].kScale,
			0.0, m_controllerSettingGraph[i].handle[graphNum], TRUE);

		// 色を元に戻す
		SetDrawBright(255, 255, 255);
		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ControllerOption::DrawControllerExplanation()
{
	int const width = 600;
	int const height = 280;

	int const setPosX = 0;
	int const setPosY = 150;


	int const leftTopX = (Game::kWindowCenterX - width / 2) + setPosX;
	int const leftTopY = (Game::kWindowCenterY - height / 2) + setPosY;

	int const rightBottomX = (Game::kWindowCenterX + width / 2) + setPosX;
	int const rightBottomY = (Game::kWindowCenterY + height / 2) + setPosY;




	DrawBox(leftTopX, leftTopY, rightBottomX, rightBottomY, 0xffffff, false);


}
