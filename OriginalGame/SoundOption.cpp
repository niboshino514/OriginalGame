#include "SoundOption.h"
#include "game.h"
#include "EvoLib.h"
#include "Controller.h"
#include "Sound.h"
#include "ObjectManager.h"


namespace Window
{
	// ポーズウィンドウの横幅
	const int kWidth = 650;

	// ポーズウィンドウの縦幅
	const int kHeight = 350;

	// 透明度
	const int kAlpha = 150;
}

namespace WindowNameGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/SoundOption/WindowName.png";
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// 拡大率
	const double kScale = 1.0;
}

namespace SettingItemGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/SoundOption/SelectItem.png";
	// グラフィックのグラフィック距離
	const Vec2 kDistanceValue = Vec2(0, 40);
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 2 };
	// 中心座標
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX - 200, Game::kWindowCenterY);
	// 拡大率
	const double kScale = 0.8;
	// 透明度
	const int kAlpha = 100;
}

namespace SoundVolumeGraph
{

	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/SoundOption/Bar.png";

	// グラフィックのグラフィック距離
	const Vec2 kDistanceValue = Vec2(0, 65);

	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 2 };

	// 中心座標
	const Vec2 kCenterPos = Vec2(Game::kWindowCenterX, Game::kWindowCenterY);

	// 拡大率
	const double kBigScale = 0.5;
	const double kSmallScale = 0.3;

	// 透明度
	const int kAlpha = 100;

	// 横幅
	constexpr int kWidth = 50;
}

namespace SelectTriangleGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Selection/SelectTriangle.png";

	// 拡大率
	const float kScale = 0.7f;

	// 距離
	const Vec2 kDistance = Vec2(-160, 0);
}

namespace BackGraph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Selection/Back.png";
	// 分割数
	const EvoLib::Load::DivNum kDivNum = { 1, 1 };
	// 拡大率
	const double kScale = 0.5;
}

namespace
{
	constexpr int kSoundVolumeMax = 6;
}


SoundOption::SoundOption():
	m_windowNameGraph(),
	m_settingItemGraph(),
	m_soundVolumeGraph(),
	m_selectTriangleGraph(),
	m_backGraph(),
	m_settingItemSelect(),
	m_soundVolume(),
	m_isCloseWindow()
{
}

SoundOption::~SoundOption()
{
	// グラフィックの解放
	for(auto& handle : m_windowNameGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& handle : m_settingItemGraph.handle)
	{
		DeleteGraph(handle);
	}
	for (auto& handle : m_soundVolumeGraph.handle)
	{
		DeleteGraph(handle);
	}
	for(auto& handle : m_backGraph.handle)
	{
		DeleteGraph(handle);
	}
	DeleteGraph(m_selectTriangleGraph);
}

void SoundOption::Init()
{
	// サウンドボリュームの初期化
	const int bgmVolume = static_cast<int>(EvoLib::Convert::ConvertFromPercentToValue(kSoundVolumeMax, static_cast<float>(Sound::GetInstance()->GetSoundVolume().bgm)));
	const int seVolume = static_cast<int>(EvoLib::Convert::ConvertFromPercentToValue(kSoundVolumeMax, static_cast<float>(Sound::GetInstance()->GetSoundVolume().se)));
	

	m_soundVolume.bgmVolume = bgmVolume;
	m_soundVolume.seVolume = seVolume;


	// グラフィックのロード
	Load();
}

void SoundOption::Update()
{
	// 設定項目の更新
	UpdateSettingItem();

	// BGMの音量調整
	UpdateSoundBgmVolume();

	// SEの音量調整
	UpdateSoundSeVolume();

	// キャンセルボタンが押されたらウィンドウを閉じる
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::CANCEL))
	{
		m_isCloseWindow = true;

		// キャンセル音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);
	}
}

void SoundOption::Draw()
{
	// ウィンドウの描画
	DrawWindow();

	// 設定項目の描画
	DrawSettingItem();

	// BGMの音量の描画
	DrawSoundBgmVolume();

	// SEの音量の描画
	DrawSoundSeVolume();

	// 戻るボタンの描画
	DrawRotaGraphF(m_backGraph.pos[0].x, m_backGraph.pos[0].y, BackGraph::kScale, 0.0, m_backGraph.handle[0], TRUE);
}

void SoundOption::InitSettingItem()
{
	// 設定項目の初期化
	m_settingItemSelect = SettingItemSenect::BGM;

	// ウィンドウを閉じるかどうか
	m_isCloseWindow = false;
}

void SoundOption::Load()
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

	// 設定項目のグラフィックのロード
	{
		// グラフィックロード
		m_settingItemGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(SettingItemGraph::kFilePath, SettingItemGraph::kDivNum);

		// 座標の設定
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_settingItemGraph.handle, SettingItemGraph::kScale);

		// 座標の設定
		m_settingItemGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, SettingItemGraph::kCenterPos, static_cast<int>(m_settingItemGraph.handle.size()), SettingItemGraph::kDistanceValue, false);

	}

	// サウンドボリュームのグラフィックのロード
	{
		// グラフィックロード
		m_soundVolumeGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(SoundVolumeGraph::kFilePath, SoundVolumeGraph::kDivNum);

		// 座標の設定
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_soundVolumeGraph.handle, SoundVolumeGraph::kBigScale);

		// 座標の設定
		m_soundVolumeGraph.pos = EvoLib::Calculation::GraphEqualization
		(graphSize, SoundVolumeGraph::kCenterPos, static_cast<int>(m_soundVolumeGraph.handle.size()), SoundVolumeGraph::kDistanceValue, false);

	}

	// バックグラフィックのロード
	{
		// グラフィックロード
		m_backGraph.handle = EvoLib::Load::LoadDivGraph_EvoLib_Revision
		(BackGraph::kFilePath, BackGraph::kDivNum);

		// ウィンドウの下Y座標
		const float windowBottom_Y = Game::kWindowCenterY + Window::kHeight / 2;
		// ウィンドウの右X座標
		const float windowRight_X = Game::kWindowCenterX + Window::kWidth / 2;

		// 座標の設定
		const Vec2 graphSize = EvoLib::Calculation::GetGraphSize_EvoLib(m_backGraph.handle, BackGraph::kScale);


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

void SoundOption::UpdateSettingItem()
{
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::DOWN))
	{
		// 選択項目の更新
		m_settingItemSelect = static_cast<SettingItemSenect>((static_cast<int>(m_settingItemSelect) + 1) % static_cast<int>(SettingItemSenect::SELECT_NUM));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::UP))
	{
		// 選択項目の更新
		m_settingItemSelect = static_cast<SettingItemSenect>((static_cast<int>(m_settingItemSelect) - 1 + static_cast<int>(SettingItemSenect::SELECT_NUM)) % static_cast<int>(SettingItemSenect::SELECT_NUM));

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void SoundOption::UpdateSoundBgmVolume()
{
	// BGMの音量調整以外が選択されている場合、処理を抜ける
	if(m_settingItemSelect != SettingItemSenect::BGM)
	{
		return;
	}



	// 音量の更新
	if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{

		// 上限ループ
		m_soundVolume.bgmVolume++;

		if(m_soundVolume.bgmVolume >= kSoundVolumeMax)
		{
			m_soundVolume.bgmVolume = 0;
		}

		// 音量のパーセンテージを求める
		const int soundPercentageVolume = EvoLib::Convert::PercentageRate(static_cast<float>(kSoundVolumeMax), static_cast<float>(m_soundVolume.bgmVolume));

		// 音量の更新
		Sound::GetInstance()->SetSoundVolume(Sound::SoundType::BGM, soundPercentageVolume);
		
		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if (Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{

		// 下限ループ
		m_soundVolume.bgmVolume--;

		if(m_soundVolume.bgmVolume < 0)
		{
			m_soundVolume.bgmVolume = kSoundVolumeMax;
		}



		// 音量のパーセンテージを求める
		const int soundPercentageVolume = EvoLib::Convert::PercentageRate(static_cast<float>(kSoundVolumeMax), static_cast<float>(m_soundVolume.bgmVolume));

		// 音量の更新
		Sound::GetInstance()->SetSoundVolume(Sound::SoundType::BGM, soundPercentageVolume);
	
		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void SoundOption::UpdateSoundSeVolume()
{
	if(m_settingItemSelect != SettingItemSenect::SE)
	{
		return;
	}


	// 音量の更新
	if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::RIGHT))
	{
		
		m_soundVolume.seVolume++;

		if(m_soundVolume.seVolume >= kSoundVolumeMax)
		{
			m_soundVolume.seVolume = 0;
		}

		// 音量のパーセンテージを求める
		const int soundPercentageVolume = EvoLib::Convert::PercentageRate(static_cast<float>(kSoundVolumeMax), static_cast<float>(m_soundVolume.seVolume));

		// 音量の更新
		Sound::GetInstance()->SetSoundVolume(Sound::SoundType::SE, soundPercentageVolume);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
	else if(Controller::GetInstance()->IsTrigger(Controller::ControllerButton::LEFT))
	{
	
		m_soundVolume.seVolume--;

		if(m_soundVolume.seVolume < 0)
		{
			m_soundVolume.seVolume = kSoundVolumeMax;
		}

		// 音量のパーセンテージを求める
		const int soundPercentageVolume = EvoLib::Convert::PercentageRate(static_cast<float>(kSoundVolumeMax), static_cast<float>(m_soundVolume.seVolume));

		// 音量の更新
		Sound::GetInstance()->SetSoundVolume(Sound::SoundType::SE, soundPercentageVolume);

		// 選択音を再生
		Sound::GetInstance()->Play(kSoundFileName[static_cast<int>(SoundName::Select)]);
	}
}

void SoundOption::DrawWindow()
{
	int const windowLeftTopX = Game::kWindowCenterX - Window::kWidth / 2;
	int const windowLeftTopY = Game::kWindowCenterY - Window::kHeight / 2;

	int const windowRightBottomX = Game::kWindowCenterX + Window::kWidth / 2;
	int const windowRightBottomY = Game::kWindowCenterY + Window::kHeight / 2;


	// 透明度設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Window::kAlpha);

	// 画面全体を黒で塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	// ポーズウィンドウ
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0x000000, true);

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズウィンドウ枠
	DrawBox(windowLeftTopX, windowLeftTopY, windowRightBottomX, windowRightBottomY, 0xffffff, false);




	// ウィンドウ名の描画
	DrawRotaGraphF(m_windowNameGraph.pos[0].x, m_windowNameGraph.pos[0].y, WindowNameGraph::kScale, 0.0, m_windowNameGraph.handle[0], TRUE);

}

void SoundOption::DrawSettingItem()
{
	// 設定項目の描画
	for (int i = 0; i < static_cast<int>(m_settingItemGraph.handle.size()); i++)
	{
		// 選択されていない項目を透明にして描画
		if(static_cast<int>(m_settingItemSelect) != i)
		{
			
			// 透明度設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, SettingItemGraph::kAlpha);
		}
		else
		{
			// 選ばれている選択肢は赤色で描画
			SetDrawBright(255, 0, 0);
		}

		// グラフィックの描画
		DrawRotaGraphF(m_settingItemGraph.pos[i].x, m_settingItemGraph.pos[i].y, SettingItemGraph::kScale, 0.0, m_settingItemGraph.handle[i], TRUE);


		// 色を元に戻す
		SetDrawBright(255, 255, 255);
		// 透明度解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SoundOption::DrawSoundBgmVolume()
{
	if(m_settingItemSelect != SettingItemSenect::BGM)
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, SoundVolumeGraph::kAlpha);
	}
	else
	{
		// 選ばれている選択肢は赤色で描画
		SetDrawBright(255, 0, 0);

		// 三角形の座標
		Vec2 trianglePos = m_soundVolumeGraph.pos[0];
		trianglePos.x += ((kSoundVolumeMax / 2) - 1) * SoundVolumeGraph::kWidth;

		// 距離
		Vec2 distance = SelectTriangleGraph::kDistance;


		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			Vec2(trianglePos.x + distance.x, trianglePos.y + distance.y),
			0,
			false,
			SelectTriangleGraph::kScale,
			true,
			true,
			true,
			false);

		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			Vec2(trianglePos.x - distance.x, trianglePos.y - distance.y),
			0,
			false,
			SelectTriangleGraph::kScale,
			true,
			false,
			true,
			false);
	}


	for (int i = 0; i < kSoundVolumeMax-1; i++)
	{
		Vec2 pos = m_soundVolumeGraph.pos[0];
		pos.x += i * SoundVolumeGraph::kWidth;

		

		if (i >= m_soundVolume.bgmVolume)
		{
			// グラフィックの描画
			DrawRotaGraphF(pos.x, pos.y, SoundVolumeGraph::kSmallScale, 0.0, m_soundVolumeGraph.handle[1], TRUE);

		}

		if(i < m_soundVolume.bgmVolume)
		{
			// グラフィックの描画
			DrawRotaGraphF(pos.x, pos.y, SoundVolumeGraph::kBigScale, 0.0, m_soundVolumeGraph.handle[0], TRUE);
		}
	}

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 色を元に戻す
	SetDrawBright(255, 255, 255);
}

void SoundOption::DrawSoundSeVolume()
{
	if(m_settingItemSelect != SettingItemSenect::SE)
	{
		// 透明度設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, SoundVolumeGraph::kAlpha);
	}
	else
	{
		// 選ばれている選択肢は赤色で描画
		SetDrawBright(255, 0, 0);

		// 三角形の座標
		Vec2 trianglePos = m_soundVolumeGraph.pos[1];
		trianglePos.x += ((kSoundVolumeMax / 2) - 1) * SoundVolumeGraph::kWidth;

		// 距離
		Vec2 distance = SelectTriangleGraph::kDistance;


		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			Vec2(trianglePos.x + distance.x, trianglePos.y + distance.y),
			0,
			false,
			SelectTriangleGraph::kScale,
			true,
			true,
			true,
			false);

		EvoLib::Draw::DrawRotatingImage(
			m_selectTriangleGraph,
			Vec2(trianglePos.x - distance.x, trianglePos.y - distance.y),
			0,
			false,
			SelectTriangleGraph::kScale,
			true,
			false,
			true,
			false);
	}

	for(int i = 0; i < kSoundVolumeMax-1; i++)
	{
		Vec2 pos = m_soundVolumeGraph.pos[1];
		pos.x += i * SoundVolumeGraph::kWidth;

		if (i >= m_soundVolume.seVolume)
		{
			// グラフィックの描画
			DrawRotaGraphF(pos.x, pos.y, SoundVolumeGraph::kSmallScale, 0.0, m_soundVolumeGraph.handle[1], TRUE);
		}

		if(i < m_soundVolume.seVolume)
		{
			// グラフィックの描画
			DrawRotaGraphF(pos.x, pos.y, SoundVolumeGraph::kBigScale, 0.0, m_soundVolumeGraph.handle[0], TRUE);
		}
	}

	// 透明度解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 色を元に戻す
	SetDrawBright(255, 255, 255);

}
