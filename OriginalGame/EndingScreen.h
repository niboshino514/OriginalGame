#pragma once
#include <memory>
#include "EvoLib.h"
#include "GameData.h"

class SceneEnding;

class EndingScreen
{
public:
	EndingScreen();
	virtual ~EndingScreen();

	/// <summary>
	/// シーンエンディングのポインタを取得
	/// </summary>
	/// <param name="sceneEnding">シーンエンディングポインター</param>
	void SetSceneEndingPointer(SceneEnding* sceneEnding) { m_pSceneEnding = sceneEnding; }

	void Init();
	void Update();
	void Draw();

private:

	// 画像ロード
	void Load();

	// サインカーブの更新
	void UpdateSineCurve();

	// スコアの描画
	void DrawScore();


private:

	// スコアグラフィックデータ
	struct ScoreGraphData
	{
		// 番号グラフ
		std::vector<int> numberGraphHandle;

		// 仕切りグラフ
		int separateGraphHandle = -1;

		// 回数のグラフィック
		int countGraphHandle = -1;

		// クリアリトライ回数テキストグラフィック
		std::vector<int> clearRetryTextGraphHandle;
	};


private:


	//////////////////////
	// グラフィック関係 //
	//////////////////////

	int m_gameClearGraphHandle;	// ゲームクリア画像ハンドル

	ScoreGraphData m_scoreGraphData;	// スコアグラフィックデータ

	int m_thanksGraphHandle;	// サンクス画像ハンドル

	int m_newRecordGraphHandle;	// ニューレコード画像ハンドル

	////////////////
	// スコア関連 //
	////////////////


	GameData::Score m_score;	// スコア

	bool m_isNewRecord;	// ニューレコードかどうか

	//////////////////////
	// サインカーブ関連 //
	//////////////////////

	EvoLib::Calculation::SineCurveData<float> m_gameClearGraphSineCuve;	// ゲームクリア画像のサインカーブデータ
	float m_gameClearGraphSineCurveValue;	// ゲームクリア画像のサインカーブの値

	// シーンエンディングポインタ
	SceneEnding* m_pSceneEnding;
};

