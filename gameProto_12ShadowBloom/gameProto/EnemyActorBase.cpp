#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"

#include "EnemyActorBase.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

// 状態に変更があったらこちらも変更
const char* stateEnumName[static_cast<int>(EnemyStateEnum::StateNum)] =
{
	"EnemyStateEnum::Invalid",
	"EnemyStateEnum::Spawn",
	"EnemyStateEnum::Idle",
	"EnemyStateEnum::Walk",
	"EnemyStateEnum::Turn",
	"EnemyStateEnum::Run",
	"EnemyStateEnum::LookAround"
	"EnemyStateEnum::Attack1",
	"EnemyStateEnum::Attack2",
	"EnemyStateEnum::Attack3",
	"EnemyStateEnum::GetDamage",
	"EnemyStateEnum::Roar",
	"EnemyStateEnum::Stun",
	"EnemyStateEnum::Die",

	"EnemyStateEnum::StateNum"
};

EnemyActorBase::EnemyActorBase()
	: mNowState(EnemyStateEnum::Invalid)
	, mVelocityVec(0.0f, 0.0f, 0.0f)
	, mForwardVec(1.0f, 0.0f, 0.0f)
	, mWalkSpeed(0.0f)
	, mRunSpeed(0.0f)
	, mTurnSpeed(0.0f)
	, mHitPoint(1)
	, mIsOnGround(false)
	, mSkelMeshComponent(nullptr)
{
}

// stateにセットされているアニメーションを再生
float EnemyActorBase::PlayAnimation(EnemyStateEnum state, float rate)
{
	auto itr = mAnimations.find(state);
	if (itr == mAnimations.end())
	{
		return 0.0f;
	}
	// アニメーションを持っていれば再生
	return mSkelMeshComponent->PlayAnimation(mAnimations[state], rate);
}

void EnemyActorBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

// アニメーションは再生中か？
bool EnemyActorBase::IsAnimationPlaying()
{
	return mSkelMeshComponent->IsPlaying();
}

// TriggerBoxのセット BGTriggerとしてBoxColliderをセット
void EnemyActorBase::SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box)
{
	BoxCollider* tb = new BoxCollider(this, EnumPhysicsType::EnumBGTrigger);
	tb->SetObjectBox(box);
	// trigerTypeをキーとして TrigerBoxをセット
	mTrigerBoxs.emplace(trigerType, tb);
}

// TrigerBoxがセットされていれば、トリガーボックスのヒットの有無を返す
bool EnemyActorBase::IsHitTrigger(EnemyTriggerEnum type)
{
	auto itr = mTrigerBoxs.find(type);
	if (itr == mTrigerBoxs.end())
	{
		return false;
	}
	return itr->second->IsTrigerHit();
}

// TriggerEnumのTrigerBoxは登録されているか？
bool EnemyActorBase::IsExistTriggerBox(EnemyTriggerEnum type)
{
	auto iter = mTrigerBoxs.find(type);
	return iter != mTrigerBoxs.end();
}

// アニメーションの初期化
void EnemyActorBase::LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state)
{
	const Animation* anim = RENDERER->GetAnimation(animationFileName.c_str(), loop);
	// 連想配列に stateをキーとしてアニメーションを追加
	mAnimations.emplace(state, anim);
}

// メッシュデータの読み込み
void EnemyActorBase::LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename)
{
	
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName.c_str());
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton(gskelFilename.c_str()));
}

// 状態名を文字列で返す
const char* GetEnemyStateEnumName(EnemyStateEnum state)
{
	return stateEnumName[static_cast<int>(state)];
}
