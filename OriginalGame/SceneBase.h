#pragma once

// ゲームシーン基底クラス
class SceneBase
{
protected:

	// フェード
	struct Fade
	{
		// フェードの状態
		enum State
		{
			// フェードイン
			In,
			// フェードアウト
			Out,
			// フェードイン完了
			InEnd,
			// フェードアウト完了
			OutEnd,
		};

		// フェードの状態
		State state = InEnd;

		// フェードの速度
		int speed = 0;

		// フェードの色
		int color = 0x000000;

		// フェードのアルファ値
		int alpha = 0;
	};

public:
	SceneBase() {}
	virtual ~SceneBase() {}


	virtual void Init() {}
	virtual void End() {}

	virtual SceneBase* Update() { return this; }
	virtual void Draw() {}

	// シーン終了
	virtual bool isEnd() { return false; }

	// フェードインが終了しているかどうか
	bool IsFadeInEnd() { return m_fade.state == Fade::InEnd; }


protected:

	/// <summary>
	/// フェードイン設定
	/// </summary>
	/// <param name="fadeSpeed">フェードスピード</param>
	/// <param name="fadeColor">フェードカラー</param>
	void SetFadeIn(const int& fadeSpeed, const int& fadeColor);

	/// <summary>
	/// フェードアウト設定
	/// </summary>
	/// <param name="fadeSpeed">フェードスピード</param>
	/// <param name="fadeColor">フェードカラー</param>
	void SetFadeOut(const int& fadeSpeed, const int& fadeColor);

	/// <summary>
	/// フェードアウトが終了しているかどうか
	/// </summary>
	/// <returns>フェードアウトが終了しているかどうかフラグを返す</returns>
	bool IsFadeOutEnd();

	/// <summary>
	/// フェード更新
	/// </summary>
	void UpdateFade();

	/// <summary>
	/// フェード描画
	/// </summary>
	void DrawFade();

	/// <summary>
	/// フェードアウト中かどうか
	/// </summary>
	/// <returns>フェードアウトフラグ</returns>
	bool GetIsFadeOut() { return m_fade.state == Fade::Out; }

	/// <summary>
	/// フェードアルファ値取得
	/// </summary>
	/// <returns>フェードアルファ値</returns>
	int GetFadeAlpha() { return m_fade.alpha; }

	/// <summary>
	/// フェードアウト時、だんだん音が小さくなる処理
	/// </summary>
	void FadeOutSound();

protected:

	// 次のシーン
	SceneBase* m_nextScene = nullptr;

private:
	// フェード
	Fade m_fade;
};