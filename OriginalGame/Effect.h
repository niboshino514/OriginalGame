#pragma once
#include "Vec2.h"
#include <EvoLib.h>


class Effect
{
public:
	Effect();
	virtual ~Effect();

	
	void Init();
	void Update();
	void Draw();

	// エフェクトハンドルを取得
	void SetEffectHandle(int effectHandle){ m_effectHandle = effectHandle; }

	bool GetIsExist(){ return m_isExist; }
public:

	struct Color
	{
		// 赤
		int r = 0;
		// 緑
		int g = 0;
		// 青
		int b = 0;
	};


	// エフェクトデータ
	struct EffectData
	{
		// 座標
		Vec2 pos = Vec2();
		// 速度
		float speed = 0.0f;
		// 移動ラジアン
		float moveRad = 0.0f;


		// 画像のサイズ
		double graphSize = 0.0;
		// 画像のラジアン
		float graphRad = 0.0f;
		// 画像の透明度
		int alpha = 255;

		// 画像の色
		Color color = Color();
	};


private:

	// エフェクトデータ
	EffectData m_effectData;

	// エフェクトハンドル
	int m_effectHandle;

	// 存在するかのフラグ
	bool m_isExist = false;

};