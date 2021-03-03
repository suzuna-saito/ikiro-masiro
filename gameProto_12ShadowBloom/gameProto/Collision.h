#pragma once
#include "game.h"

// 球体
struct Sphere
{
	Sphere(const Vector3& center, float radius);                      
	bool Contains(const Vector3& point) const;                        // 球体内に点が含まれるか？

	Vector3 mCenter;                                                  // 中心位置
	float mRadius;                                                    // 半径
};

// 軸並行ボックス(Axis-Aligned Bounding Box)
struct AABB
{
	AABB();
	AABB(const Vector3& min, const Vector3& max);                     // AABB初期化用
	// モデル読み込み時の最小最大の点を求めるのに使用             
	void InitMinMax(const Vector3& point) { mMin = mMax = point; }    // 
	void UpdateMinMax(const Vector3& point);                          // 更新処理
	bool Contains(const Vector3& point) const;                        // 点がボックス内に含まれるか
	float MinDistSq(const Vector3& point) const;                      // 点との最小距離を求める
	void Rotate(const Quaternion& q);                                 // 回転
	void SetArrowRotate(bool value) { mIsRotatable = value; }         // 回転を許可するか？

	Vector3 mMin;                                                     // ボックス最小座標
	Vector3 mMax;                                                     // ボックス最大座標 
	bool    mIsRotatable;                                             // 回転するかしないか？
};


bool Intersect(const AABB& a, const AABB& b);                         // AABB 同士の衝突
bool Intersect(const Sphere& s, const AABB& box);                     // 球体とAABBとの衝突

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec); // AABBのめり込みをもどす量を計算
