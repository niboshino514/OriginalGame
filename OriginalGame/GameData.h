#pragma once
#include "FunctionConclusion.h"

class GameData
{
public:

	// セーブポイントデータ
	struct SavePointData
	{
		// ステージナンバー
		int stageNumber = 0;

		// セーブしたセル
		Cell cell = Cell();
	}; 


public:
	GameData();
	virtual ~GameData();

	/// <summary>
	/// セーブポイントデータを取得する
	/// </summary>
	/// <param name="savePointData">セーブポイントデータ</param>
	void SetSavePointData(const SavePointData& savePointData) { m_savePointData = savePointData; }

	/// <summary>
	/// セーブポイントデータを返す
	/// </summary>
	/// <returns>セーブポイントデータ</returns>
	SavePointData GetSavePointData() { return m_savePointData; }

private:

	// セーブポイントデータ
	SavePointData m_savePointData;
};