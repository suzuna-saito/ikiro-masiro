#include "BlackKnightActor.h"

#include "game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"

#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyHit.h"

#include "Input.h"

#include <iostream>

BlackKnightActor::BlackKnightActor()
	: mEnemyBehaviorComponent(nullptr)
	, mFrontTriggerBox(nullptr)
	, mHitBox(nullptr)
{
	printf("Create : [Actor] BlackKnight 0x%p\n", this);

	// 初期パラメータ設定
	mWalkSpeed  = 100.0f;
	mRunSpeed   = 200.0f;
	mTurnSpeed  = Math::Pi;
	mHitPoint   = 2;
	mIsOnGround = true;

	// モデル読み込み
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/BlackKnightSK.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/BlackKnightSK.gpskel"));

	// アニメーション読み込み
	mAnimations.emplace(EnemyStateEnum::Walk,       RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/BlackKnight_SenseSomethingRPTAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage,  RENDERER->GetAnimation("Assets/Enemy/BlackKnight_GetHitAnim.gpanim", false));

	// EnemyBehaviorComponent に ふるまいを追加
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyPatrol(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	// 敵キャラの当たり判定を追加
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 0.5f;
	enemyBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	AABB enemyForward;
	enemyForward.mMin.x = enemyBox.mMax.x;
	enemyForward.mMin.y = enemyBox.mMin.y;
	enemyForward.mMin.z = enemyBox.mMin.z + 100;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;

	SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);

	// 武器取り付け
	AttachMeshComponent* amc = new AttachMeshComponent(this, mSkelMeshComponent, "Axe");
	mesh = RENDERER->GetMesh("Assets/Enemy/AxeSM.gpmesh");
	amc->SetMesh(mesh);
	Vector3 rot, pos;
	rot = Vector3(0, 0, 0);
	pos = Vector3(-320, 0, 145);
	amc->SetOffsetRotation(rot);
	amc->SetOffsetPosition(pos);

	amc = new AttachMeshComponent(this, mSkelMeshComponent, "Shield");
	mesh = RENDERER->GetMesh("Assets/Enemy/ShieldSM.gpmesh");
	amc->SetMesh(mesh);

	rot = Vector3(-Math::PiOver2 * 0.5f, 0, Math::PiOver2);
	pos = Vector3(300, 0, 160);
	amc->SetOffsetRotation(rot);
	amc->SetOffsetPosition(pos);
}	

BlackKnightActor::~BlackKnightActor()
{
	printf("Remove : [Actor] BlackKnight 0x%p\n", this);
}

void BlackKnightActor::UpdateActor(float deltaTime)
{

}

void BlackKnightActor::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// 当たり判定で帰ってきた結果がmHitBox、背景との衝突だった場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumBG)
	{
		AABB bgBox   = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, bgBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// プレイヤーの攻撃がヒット
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{
		//プレイヤーの方向へ向く
		Vector3 playerPos = hitOtherBox->GetOwner()->GetPosition();
		Vector3 dir = playerPos - mPosition;
		dir.z = 0.0f; // 上方向の向きをキャンセル
		const float epsiron = 0.0001f;
		if (dir.LengthSq() < epsiron)
		{
			dir = -1.0f * mForwardVec;
		}
		else
		{
			dir.Normalize();
		}
		mForwardVec = dir;
		mRecomputeWorldTransform = true;

		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::GetDamage);
	}

}

bool BlackKnightActor::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
