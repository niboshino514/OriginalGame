#include "ControllerOption.h"
#include <DxLib.h>
#include "game.h"
#include "EvoLib.h"

namespace SettingItem
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/ControllerOption/SettingItem.png";
	// グラフィックのグラフィック距離
	const Vec2 kDistanceValue = Vec2(0, 40);
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 4 };
	// 中心座標
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX-250, Game::kWindowCenterY);
	// 拡大率
	const double kScale = 0.5;
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
		const Vec2 kDistanceValue = Vec2(500, 0);
		// 拡大率
		const double kScale = 0.5;
	};



	// 設定項目
	const std::vector< ControllerSettingGraphInfo> m_controllerSettingGraphInfo =
	{
		// コントローラー自動切換え
		{
			"Data/ControllerOption/Switch.png",
			{ 1, 2 },
			{ 500, 0 },
			0.5
		},
		// コントローラータイプ選択
		{
			"Data/ControllerOption/ControllerType.png",
			{ 1, 3 },
			{ 500, 0 },
			0.5
		},
		// 入力デバイス切替
		{
			"Data/ControllerOption/InputDevice.png",
			{ 1, 2 },
			{ 500, 0 },
			0.5
		},
	};

}




ControllerOption::ControllerOption():
	m_settingItemSenect(),
	m_controllerSetting(),
	m_settingItemGraph(),
	m_controllerSettingGraph()
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

}

void ControllerOption::Init()
{
	// 選択項目の初期化
	m_settingItemSenect = SettingItemSenect::AUTO_SWITCH;
	
	// コントローラー設定取得
	m_controllerSetting = Controller::GetInstance()->GetControllerSetting();
	


	// グラフィックのロード
	LoadGraph();
}

void ControllerOption::Update()
{
}

void ControllerOption::Draw()
{
	// 設定項目の描画
	DrawSettingItem();

	// コントローラー設定の描画
	DrawControllerSetting();
}

void ControllerOption::LoadGraph()
{
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
		const int selectNum = static_cast<int>(SettingItemSenect::SELECT_NUM) - 1;

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
}

void ControllerOption::DrawSettingItem()
{
	// 設定項目の描画
	for (size_t i = 0; i < m_settingItemGraph.handle.size(); i++)
	{
		DrawRotaGraphF(m_settingItemGraph.pos[i].x, m_settingItemGraph.pos[i].y, SettingItem::kScale, 0.0, m_settingItemGraph.handle[i], TRUE);
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
		const int graphNum = 0;






		// グラフィックの描画
		DrawRotaGraphF(
			m_controllerSettingGraph[i].pos[0].x, 
			m_controllerSettingGraph[i].pos[0].y, 
			m_controllerSettingGraphInfo[settingNum].kScale,
			0.0, m_controllerSettingGraph[i].handle[graphNum], TRUE);
	}


}
