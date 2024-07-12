// コントローラー
#pragma once

// コントローラーからの入力状態を取得する
namespace Pad
{
	void Update();					// パッドの入力状態取得
	bool IsPress(int button);		// 押し下げ判定
	bool IsTrigger(int button);		// トリガー判定
	bool IsRelase(int button);		// 離した判定
}
