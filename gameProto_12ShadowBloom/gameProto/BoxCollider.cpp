#include "BoxCollider.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "Actor.h"

// BoxColliderコンストラクタ
BoxCollider::BoxCollider(Actor * owner, EnumPhysicsType physicsType, int updateOrder)
	: Component(owner, updateOrder)
	, mObjectBox(Vector3::Zero, Vector3::Zero)                    // 初期値は大きさなしのボックスに
	, mWorldBox(Vector3::Zero, Vector3::Zero)
	, mRotatable(true)
	, mIsTriggerType(false)
	, mHitTriggerFlag(false)
	, mPhisicsType(physicsType)
	, mIsIgnoreOwener(false)
{
	if (EnumPhysicsType::EnumBGTrigger == physicsType)
	{
		mIsTriggerType = true;
	}
	GAMEINSTANCE.GetPhysics()->AddBoxCollider(physicsType, this); // ボックスコライダーをPhysicsWirldに登録
	printf("Create : BoxCollider [%d] owner->( 0x%p )\n", GetID(), mOwner);
}

BoxCollider::~BoxCollider()
{
	printf("Remove : BoxCollider [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetPhysics()->RemoveBoxCollider(this);
}

void BoxCollider::OnUpdateWorldTransform()
{
	// 親アクターからでなく強制位置モードならOnWorldTransformを無視する
	if (mIsIgnoreOwener)
	{
		return;
	}
	// オブジェクト空間のボックスにリセット
	mWorldBox = mObjectBox;

	// スケーリング
	Vector3 scale = mOwner->GetScale();
	mWorldBox.mMin.x *= scale.x;
	mWorldBox.mMin.y *= scale.y;
	mWorldBox.mMin.z *= scale.z;

	mWorldBox.mMax.x *= scale.x;
	mWorldBox.mMax.y *= scale.y;
	mWorldBox.mMax.z *= scale.z;

	// 回転
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//平行移動
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}

// 強制的にボックスに対し変換行列
void BoxCollider::SetForceTransForm(Matrix4 transform)
{
	mIsIgnoreOwener = true;
	// オブジェクト空間のボックスにリセット
	mWorldBox = mObjectBox;

	// スケーリング
	Vector3 scale = mOwner->GetScale();
	mWorldBox.mMin.x *= scale.x;
	mWorldBox.mMin.y *= scale.y;
	mWorldBox.mMin.z *= scale.z;

	mWorldBox.mMax.x *= scale.x;
	mWorldBox.mMax.y *= scale.y;
	mWorldBox.mMax.z *= scale.z;

	mWorldBox.mMin = Vector3::Transform(mWorldBox.mMin, transform);
	mWorldBox.mMax = Vector3::Transform(mWorldBox.mMax, transform);

}
