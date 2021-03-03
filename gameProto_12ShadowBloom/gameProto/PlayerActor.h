#pragma once
#include "Actor.h"
#include "Collision.h"

enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,   // �ҋ@
	PLAYER_STATE_RUN,        // ����
	PLAYER_STATE_JUMPSTART,  // �W�����v�X�^�[�g
	PLAYER_STATE_JUMPLOOP,   // �W�����v���[�v    
	PLAYER_STATE_JUMPEND,    // �W�����v�I��
	PLAYER_STATE_ATTACK1,    // �U��1
	PLAYER_STATE_ATTACK2,    // �U��2
	PLAYER_STATE_ATTACK3,    // �U��3
	PLAYER_STATE_HIT,

	PLAYER_STATE_NUM,   // ���A�j���[�V������
};

class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	void                         UpdateActor(float deltaTime) override;
	void                         FixCollision(class BoxCollider *hitPlayerBox, class BoxCollider *hitBox);
	class SkeletalMeshComponent* GetSkeletalMeshComp();
	const class Animation*       GetAnim(PlayerState state);
	bool                         GetIsJump()const { return mIsJump; }
	bool                         GetOnground() const { return mOnGroundChk; }
	Vector3&                     GetJumpVec() { return mJumpVec; }

	void                         SetOnground(bool onGround) { mOnGroundChk = onGround; }
	void                         SetIsJump(bool jump) { mIsJump = jump; }
	void                         SetJumpVec(Vector3& vec) { mJumpVec = vec; }
	void                         SetAttackHitBox(float scale = 1.0f);
	void                         RemoveAttackHitBox();
	void                         HitEnemy(class BoxCollider* playerBox, class BoxCollider* enemyBox);

private:
	class SkeletalMeshComponent*             mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider*                       mHitBox;
	class BoxCollider*                       mHitGroundBox;
	class BoxCollider*                       mHitHeadBox;
	class BoxCollider*                       mAttackBox;       // �v���C���[�̍U�������蔻��{�b�N�X                                            
	class AttachMeshComponent*               mWeaponMesh;      // ���탁�b�V��

	PlayerState                              mNowState;        // ���݂̃X�e�[�g
	PlayerState                              mNextState;       // ���̃X�e�[�g
	std::vector<class PlayerActorStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	bool                                     mIsJump;
	bool                                     mOnGroundChk;
	Vector3                                  mJumpVec;
	Vector3                                  mVelocityVec;
	AABB                                     mWeaponAABB;
};