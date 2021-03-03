#include "PlayerActor.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AudioManager.h"

#include "PlayerActorStateBase.h"
#include "PlayerActorStateRun.h"
#include "PlayerActorStateIdle.h"
#include "PlayerActorStateJump.h"
#include "PlayerActorStateJumpLoop.h"
#include "PlayerActorStateJumpEnd.h"
#include "PlayerActorStateAttack1.h"
#include "PlayerActorStateAttack2.h"
#include "PlayerActorStateAttack3.h"
#include "PlayerActorStateHit.h"

const float cAnimationSpeed = 0.2f;

PlayerActor::PlayerActor()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mIsJump(false)
	, mOnGroundChk(true)
	, mWeaponMesh(nullptr)
{
	//���b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("Assets/valkiria.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/valkiria.gpskel"));
	
	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)]      = RENDERER->GetAnimation("Assets/valkiria_idle.gpanim"     , true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)]       = RENDERER->GetAnimation("Assets/valkiria_Run.gpanim"      , true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPSTART)] = RENDERER->GetAnimation("Assets/valkiria_Jumpstart.gpanim", false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPLOOP)]  = RENDERER->GetAnimation("Assets/valkiria_jumploop.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPEND)]   = RENDERER->GetAnimation("Assets/valkiria_jumpend.gpanim"  , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK1)]   = RENDERER->GetAnimation("Assets/valkiria_attack01.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK2)]   = RENDERER->GetAnimation("Assets/valkiria_attack02.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK3)]   = RENDERER->GetAnimation("Assets/valkiria_attack03.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HIT)]       = RENDERER->GetAnimation("Assets/valkiria_hit.gpanim"      , false);

	// �A�C�h����ԃA�j���[�V�������Z�b�g
	mMeshComp->PlayAnimation( mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new PlayerActorStateIdle);       // mStatePool[PLAYER_STATE_IDLE]
	mStatePools.push_back(new PlayerActorStateRun);        // mStatepool[PLAYER_STATE_RUN]
	mStatePools.push_back(new PlayerActorStateJump);       // mStatepool[PLAYER_STATE_JUMPSTART];  
	mStatePools.push_back(new PlayerActorStateJumpLoop);   // mStatepool[PLAYER_STATE_JUMPLOOP];
	mStatePools.push_back(new PlayerActorStateJumpEnd);    // mStatepool[PLAYER_STATE_JUMPEND];
	mStatePools.push_back(new PlayerActorStateAttack1);    // mStatepool[PLAYER_STATE_ATTACK1];
	mStatePools.push_back(new PlayerActorStateAttack2);
	mStatePools.push_back(new PlayerActorStateAttack3);
	mStatePools.push_back(new PlayerActorStateHit); 

	// �����蔻��Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// �v���[���[�̑����𒲂ׂ�{�b�N�X���쐬�@�{�b�N�X��1/4, �{�b�N�X��ʂ����_�ɗ���悤�ɂ���
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //�W�����v���Ɉ���������Ȃ�����
	groundBox.mMax.z *= 0.1f;
	mHitGroundBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitGroundBox->SetObjectBox(groundBox);

	// �v���[���[�̓���𒲂ׂ�{�b�N�X���쐬 �{�b�N�X��ʂ�����ɗ���悤�ɂ���
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitHeadBox->SetObjectBox(headBox);
	
	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
	mJumpVec = Vector3(0, 0, 0);

	// ����R���|�[�l���g��t
    // ��
	mesh = RENDERER->GetMesh("Assets/valkiria_Sword.gpmesh");
	mWeaponMesh = new AttachMeshComponent(this, mMeshComp, "index_01_r");
	mWeaponMesh->SetMesh(mesh);
	Vector3 rot, pos;
	rot = Vector3(-Math::PiOver2 * 0.5f, Math::Pi, 0);
	pos = Vector3(-55, 15, 110);
	mWeaponMesh->SetOffsetRotation(rot);
	mWeaponMesh->SetOffsetPosition(pos);

	// ����{�b�N�X�̑傫���擾
	mWeaponAABB = mesh->GetCollisionBox();

	// ��
	AttachMeshComponent* amc;
	amc = new AttachMeshComponent(this, mMeshComp, "lowerarm_l");
	amc->SetMesh(RENDERER->GetMesh("Assets/valkiria_Shield.gpmesh"));
	rot = Vector3(Math::PiOver2 * 0.5f, -Math::PiOver2 * 0.5f, -Math::PiOver2);
	pos = Vector3(60, 0, 120);
	amc->SetOffsetRotation(rot);
	amc->SetOffsetPosition(pos);

}

PlayerActor::~PlayerActor()
{
	// �o�^�X�e�[�g�����ׂĉ��
	for (auto ps : mStatePools)
	{
		delete ps;
	}
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	// �O�̃V�[�����珰���Ȃ�������
	if (!mIsJump && !mOnGroundChk)
	{
		mIsJump = true;
		mNextState = PlayerState::PLAYER_STATE_JUMPLOOP;
	}
	mOnGroundChk = false;

	// �X�e�[�g�O������X�e�[�g�ύX�����������H
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// �X�e�[�g���s
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// �X�e�[�g��������X�e�[�g�ύX���������H
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// �����U���Ă���Ƃ��̓����蔻��̍X�V����
	if (mAttackBox)
	{
		Matrix4 mat = mWeaponMesh->GetAttachTransMatrix();
		mAttackBox->SetForceTransForm(mat);
	}

}


// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void PlayerActor::FixCollision(BoxCollider *hitPlayerBox, BoxCollider * hitBox)
{

	Vector3 fix;
	// �W�����v�I����
	if (hitPlayerBox == mHitGroundBox && mJumpVec.z <= 0.0f)
	{
		// �n�ʔ���p�{�b�N�X���n�ʂƓ������Ă���(�ڒn�`�F�b�N�j
		mOnGroundChk = true;

		// �W�����v���Ȃ����
		if (mIsJump)
		{
			mOnGroundChk = true;
			mIsJump = false;
			mJumpVec = Vector3(0, 0, -1.0f);
			mPosition += mJumpVec;
				mNextState = PlayerState::PLAYER_STATE_JUMPEND;
			return;
		}
	}

	//�ǂƂԂ������Ƃ�
	AABB bgBox = hitBox->GetWorldBox();
	AABB playerBox = mHitBox->GetWorldBox();

	// �߂荞�݂��C��
	calcCollisionFixVec(playerBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	mPosition += fix;

	// �W�����v�A�N�V�������ɓ����ǂɓ��������Ƃ�
	if (hitPlayerBox == mHitHeadBox && mIsJump)
	{
		//�W�����v������ɍs���������t�]������
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec; // ��������ɓ�����Ȃ��l�ɗ���
	}

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	mHitBox->OnUpdateWorldTransform();

}

SkeletalMeshComponent* PlayerActor::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerActor::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerActor::SetAttackHitBox(float scale)
{
	mAttackBox = new BoxCollider(this, EnumPhysicsType::EnumPlayerAttack);

	AABB box = mWeaponAABB;
	box.mMin *= scale;
	box.mMax *= scale;

	mAttackBox->SetObjectBox(box);
}

void PlayerActor::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

void PlayerActor::HitEnemy(class BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//�v���C���[�̍U�����G�Ƀq�b�g������A�G�̂ق�������
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayerAttack &&
		enemyBox->GetType()  == EnumPhysicsType::EnumEnemy)
	{
		Vector3 enemyPos = enemyBox->GetOwner()->GetPosition();
		Vector3 dir      = enemyPos - mPosition;
		Vector3 nowDir   = GetForward();
		dir.z = 0.0f;// ������������L�����Z��

		//�G�Ɛڋ߂������Č��������Ȃ��ꍇ�A90���ȏ�̏ꍇ�͂��̂܂�
		const float epsiron = 0.0001f;
		float cos = Vector3::Dot(dir, nowDir);

		if (dir.LengthSq() < epsiron || cos < 0.0f)
		{
			dir = GetForward();
		}
		else
		{
			dir.Normalize();
		}
		RotateToNewForward(dir);
		RemoveAttackHitBox();
	}
}
