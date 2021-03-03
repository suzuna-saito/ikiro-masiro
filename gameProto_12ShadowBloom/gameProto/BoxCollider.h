#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"

enum class EnumPhysicsType;

class BoxCollider : public Component
{
public:
	BoxCollider(class Actor* owner, EnumPhysicsType physicsType,int updateOrder = 100);
	~BoxCollider();
	void OnUpdateWorldTransform() override;                       // ワールド変換時
	void SetObjectBox(const AABB& box) { mObjectBox = box; }      // あたり判定用境界ボックスをセット
	const AABB& GetWorldBox() const    { return mWorldBox; }      // ワールド空間上での境界ボックスを取得
	void  SetArrowRotate(bool value)   { mRotatable = value; }    // 回転を許可するか？
	EnumPhysicsType GetType()          { return mPhisicsType; }   // 自分の当たり判定属性

	bool  IsTrigerHit()                { return mHitTriggerFlag; }// トリガー時に何かとヒットしたか？
	void  SetHitTriggerFlag(bool hit)  { mHitTriggerFlag = hit; } // トリガー時にヒットした情報をセット
	void  SetForceTransForm(Matrix4 transform);                   // 当たり判定ボックスの移動を

private:
	AABB            mObjectBox;                                   // オブジェクト空間（変換前）のボックス
	AABB            mWorldBox;                                    // ワールド空間に置いた時のボックス
	bool            mRotatable;                                   // 回転を許可するか？
	bool            mHitTriggerFlag;                              // トリガー時に何かとヒットしたか
	bool            mIsTriggerType;                               // これはトリガータイプか
	bool            mIsIgnoreOwener;                              // オーナーの移動を無視するか
	EnumPhysicsType mPhisicsType;

	friend class PhysicsWorld;                                    
};