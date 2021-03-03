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

	// �����p�����[�^�ݒ�
	mWalkSpeed  = 100.0f;
	mRunSpeed   = 200.0f;
	mTurnSpeed  = Math::Pi;
	mHitPoint   = 2;
	mIsOnGround = true;

	// ���f���ǂݍ���
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/BlackKnightSK.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/BlackKnightSK.gpskel"));

	// �A�j���[�V�����ǂݍ���
	mAnimations.emplace(EnemyStateEnum::Walk,       RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/BlackKnight_SenseSomethingRPTAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage,  RENDERER->GetAnimation("Assets/Enemy/BlackKnight_GetHitAnim.gpanim", false));

	// EnemyBehaviorComponent �� �ӂ�܂���ǉ�
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyPatrol(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	// �G�L�����̓����蔻���ǉ�
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

	// ������t��
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
	// �����蔻��ŋA���Ă������ʂ�mHitBox�A�w�i�Ƃ̏Փ˂������ꍇ
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

	// �v���C���[�̍U�����q�b�g
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{
		//�v���C���[�̕����֌���
		Vector3 playerPos = hitOtherBox->GetOwner()->GetPosition();
		Vector3 dir = playerPos - mPosition;
		dir.z = 0.0f; // ������̌������L�����Z��
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
