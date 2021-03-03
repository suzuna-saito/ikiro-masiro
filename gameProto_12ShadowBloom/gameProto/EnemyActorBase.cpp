#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"

#include "EnemyActorBase.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

// ��ԂɕύX���������炱������ύX
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

// state�ɃZ�b�g����Ă���A�j���[�V�������Đ�
float EnemyActorBase::PlayAnimation(EnemyStateEnum state, float rate)
{
	auto itr = mAnimations.find(state);
	if (itr == mAnimations.end())
	{
		return 0.0f;
	}
	// �A�j���[�V�����������Ă���΍Đ�
	return mSkelMeshComponent->PlayAnimation(mAnimations[state], rate);
}

void EnemyActorBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}

// �A�j���[�V�����͍Đ������H
bool EnemyActorBase::IsAnimationPlaying()
{
	return mSkelMeshComponent->IsPlaying();
}

// TriggerBox�̃Z�b�g BGTrigger�Ƃ���BoxCollider���Z�b�g
void EnemyActorBase::SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box)
{
	BoxCollider* tb = new BoxCollider(this, EnumPhysicsType::EnumBGTrigger);
	tb->SetObjectBox(box);
	// trigerType���L�[�Ƃ��� TrigerBox���Z�b�g
	mTrigerBoxs.emplace(trigerType, tb);
}

// TrigerBox���Z�b�g����Ă���΁A�g���K�[�{�b�N�X�̃q�b�g�̗L����Ԃ�
bool EnemyActorBase::IsHitTrigger(EnemyTriggerEnum type)
{
	auto itr = mTrigerBoxs.find(type);
	if (itr == mTrigerBoxs.end())
	{
		return false;
	}
	return itr->second->IsTrigerHit();
}

// TriggerEnum��TrigerBox�͓o�^����Ă��邩�H
bool EnemyActorBase::IsExistTriggerBox(EnemyTriggerEnum type)
{
	auto iter = mTrigerBoxs.find(type);
	return iter != mTrigerBoxs.end();
}

// �A�j���[�V�����̏�����
void EnemyActorBase::LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state)
{
	const Animation* anim = RENDERER->GetAnimation(animationFileName.c_str(), loop);
	// �A�z�z��� state���L�[�Ƃ��ăA�j���[�V������ǉ�
	mAnimations.emplace(state, anim);
}

// ���b�V���f�[�^�̓ǂݍ���
void EnemyActorBase::LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename)
{
	
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName.c_str());
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton(gskelFilename.c_str()));
}

// ��Ԗ��𕶎���ŕԂ�
const char* GetEnemyStateEnumName(EnemyStateEnum state)
{
	return stateEnumName[static_cast<int>(state)];
}
