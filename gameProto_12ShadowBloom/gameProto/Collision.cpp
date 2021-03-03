#include "Collision.h"
#include <algorithm>
#include <array>

Sphere::Sphere(const Vector3 & center, float radius)
	:mCenter(center)
	, mRadius(radius)
{
}

bool Sphere::Contains(const Vector3 & point) const
{
	//球の中心と点との距離の二乗を計算したものと半径の2乗と比較
	float distSq = (mCenter - point).LengthSq();
	return distSq <= mRadius * mRadius;
}

AABB::AABB()
{
	mMin = Vector3(0, 0, 0);
	mMax = Vector3(0, 0, 0);
}

AABB::AABB(const Vector3 & min, const Vector3 & max)
	:mMin(min)
	,mMax(max)
{
}

void AABB::UpdateMinMax(const Vector3 & point)
{
	// x,y,zそれぞれの最大最小を求める
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

bool AABB::Contains(const Vector3 & point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// もし一つもtrueがなければ、boxの内側に点が存在する
	return !outside;
}

float AABB::MinDistSq(const Vector3 & point) const
{
	// それぞれの軸での差をとる
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	// 3次元空間での距離の二乗の公式より
	return dx * dx + dy * dy + dz * dz;
}

void AABB::Rotate(const Quaternion & q)
{
	// ボックスの8つの頂点の配列作る
	std::array<Vector3, 8> points;
	// 最小値は常にコーナーである
	points[0] = mMin;
	// 2つの最小値と1個の最大値の並べ替え
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	// 2つの最大値と1個の最小値の並べ替え
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);
	// 最大値は常にコーナーである
	points[7] = Vector3(mMax);

	// 最初の点を回転
	Vector3 p = Vector3::Transform(points[0], q);
	// 最小値と最大値を求めるために最初の点を入れてリセット
	mMin = p;
	mMax = p;
	// 回転によって最大値と最小値を求めなおす。
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

// AABBどうしの衝突
bool Intersect(const AABB & a, const AABB & b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// 一つもtrueがなければ、二つは衝突している
	return !no;
}
//球とAABBの衝突
bool Intersect(const Sphere& s, const AABB & box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

////////////////////////////////////////////////////////////////////
// 衝突したことが確定したとき、めり込みを戻す関数
// in    movableBox 移動物体 (ex プレーヤー)
// in    fixedBox   移動しない物体（ex ブロック）
// inout calcFixVec 移動物体の補正差分ベクトル
////////////////////////////////////////////////////////////////////
void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz には それぞれ1,2のうち絶対値が小さい方をセットする
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, zのうち最も差が小さい軸で位置を調整
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

