#include "EffectManager.h"
#include "Effect.h"
#include <DxLib.h>

namespace
{
	// エフェクトグラフィックファイルパス
	const char* const kEffectGraphicFilePath = "Data/Graphic/Ending/Paper.png";

	// エフェクトインターバル
	constexpr int kEffectInterval = 1;

	// エフェクト最大数
	constexpr int kEffectMax = 100;
}


EffectManager::EffectManager():
	m_effectHandle(-1), 
	m_frameCount(0)
{
}

EffectManager::~EffectManager()
{
	// エフェクトグラフィックを削除
	DeleteGraph(m_effectHandle);
}

void EffectManager::Init()
{
	// エフェクトグラフィックを読み込む
	m_effectHandle = LoadGraph(kEffectGraphicFilePath);

}

void EffectManager::Update()
{
	// フレームカウントを更新
	m_frameCount++;


	// エフェクトを生成
	if(m_frameCount >= kEffectInterval)
	{
		if (static_cast<int>(m_pEffectList.size() < kEffectMax))
		{
			CreateEffect();
		}
		m_frameCount = 0;
	}

	// エフェクトを更新
	for (auto& effect : m_pEffectList)
	{
		effect->Update();
	}


	// エフェクトを削除
	m_pEffectList.erase(std::remove_if(m_pEffectList.begin(), m_pEffectList.end(), [](const std::shared_ptr<Effect>& effect)
	{
		return !effect->GetIsExist();
	}), m_pEffectList.end());
}

void EffectManager::Draw()
{
	// エフェクトを描画
	for (auto& effect : m_pEffectList)
	{
		effect->Draw();
	}
}

void EffectManager::CreateEffect()
{
	// エフェクトを生成
	m_pEffectList.push_back(std::make_shared<Effect>());
	m_pEffectList.back()->Init();
	m_pEffectList.back()->SetEffectHandle(m_effectHandle);
}
