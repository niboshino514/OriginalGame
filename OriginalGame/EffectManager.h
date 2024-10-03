#pragma once
#include <memory>
#include <vector>

class Effect;

class EffectManager
{
public:
	EffectManager();
	virtual ~EffectManager();

	
	void Init();
	void Update();
	void Draw();


	void CreateEffect();
private:

	// エフェクトハンドル
	int m_effectHandle;

	// フレームカウント
	int m_frameCount;

	// エフェクトリスト
	std::vector<std::shared_ptr<Effect>> m_pEffectList;

};

