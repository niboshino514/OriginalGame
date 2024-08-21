#pragma once
#include "Vec2.h"
#include <memory>


class ObjectManager;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Update();

	/// <summary>
	/// オブジェクトファクトリークラスポインタを取得
	/// </summary>
	/// <param name="objectFactory">オブジェクトファクトリーポインタ</param>
	void SetObjectFactoryPointer(const std::shared_ptr<ObjectManager>& objectFactory) { m_pObjectFactory = objectFactory; }

	/// <summary>
	/// オフセット値計算
	/// </summary>
	void OffsetCalculation(const Vec2& targetPos);

	/// <summary>
	/// カメラ座標をを返す
	/// </summary>
	Vec2 GetCameraPos() { return m_cameraPos; }

private:

	// カメラ座標
	Vec2 m_cameraPos;

	// オブジェクトファクトリークラス
	std::shared_ptr<ObjectManager> m_pObjectFactory;
};

