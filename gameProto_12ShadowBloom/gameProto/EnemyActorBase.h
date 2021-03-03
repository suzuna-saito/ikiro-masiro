#pragma once
#include "Actor.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// �G�N���X���enum ��ԂɕύX����������stateEnumName���ύX
enum class EnemyStateEnum : unsigned char
{
	Invalid,    // �����ȃX�e�[�g
	Spawn,      // �o��
	Idle,       // �ҋ@���
	Walk,       // ����
	Turn,       // �^�[��
	Run,        // ����
	LookAround, // ����
	Attack1,    // �U���P
	Attack2,    // �U���Q
	Attack3,    // �U���R
	GetDamage,  // �_���[�W�󂯂�
	Roar,       // �i����
	Stun,       // �C��
	Die,        // ���S
	StateNum
};

// �G�g���K�[�{�b�N�X���
enum class EnemyTriggerEnum : unsigned char
{
	ForwardBox,          // ����
	LeftBox,             // ����
	RightBox,            // �E
	BackBox,             // ���
	HeadBox,             // ����
	GroundBox,           // ����
	GroundForwardBox     // �����O��
};

// �X�e�[�g�����擾����w���p�[�֐�
const char* GetEnemyStateEnumName(EnemyStateEnum state);

// �G�x�[�X�N���X
class EnemyActorBase : public Actor
{
public:
                   EnemyActorBase();
	virtual       ~EnemyActorBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	float          PlayAnimation(EnemyStateEnum state, float rate = 1.0f);
	virtual void   OnCollision(class BoxCollider* hitThisBox, class BoxCollider* hitOtherBox) = 0;

	// �Z�b�^�[�n
	void           SetVelocityVec(Vector3& v)   { mVelocityVec = v       ; }  // ���x�x�N�g���Z�b�g
	void           SetForwardVec(Vector3& v);								  // �O�i�x�N�g���Z�b�g
	void           SetWalkSpeed(float speed)    { mWalkSpeed   = speed   ; }  // �����X�s�[�h
	void           SetTurnSpeed(float speed)    { mTurnSpeed   = speed   ; }  // ����X�s�[�h
	void           SetRunSpeed(float speed)     { mRunSpeed    = speed   ; }  // ����X�s�[�h
	void           SetHitPoint(int hitpoint)    { mHitPoint    = hitpoint; }  // �q�b�g�|�C���g�Z�b�g
	void           SetIsOnGround(bool onGround) { mIsOnGround  = onGround; }  // �ڒn��Ԃ̕ύX

	void           SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box);     // �g���K�[�{�b�N�X�̃Z�b�g

	// �Q�b�^�[�n
	const Vector3& GetVelocityVec() const { return mVelocityVec ; } // ���x�x�N�g���̎擾
	const Vector3& GetForwardVec()  const { return mForwardVec  ; } // ���x�x�N�g���̎擾
	float          GetWalkSpeed()   const { return mWalkSpeed   ; } // �����X�s�[�h�̎擾
	float          GetRunSpeed()    const { return mRunSpeed    ; } // ����X�s�[�h�擾
	float          GetTurnSpeed()   const { return mTurnSpeed   ; } // ����X�s�[�h�擾
	int            GetHitPoint()    const { return mHitPoint    ; } // �̗͂̎擾
	bool           GetIsOnGround()  const { return mIsOnGround  ; } // �ڒn��Ԃ̎擾
	EnemyStateEnum GetEnemyBehaviorState() const { return mNowState; }

	bool           IsHitTrigger(EnemyTriggerEnum type);             // TriggerEnum���q�b�g���Ă��邩�H
	bool           IsExistTriggerBox(EnemyTriggerEnum type);        // TriggerBox���o�^����Ă��邩�H

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	EnemyStateEnum mNowState    ; // ���݂̃X�e�[�g

	Vector3        mVelocityVec ; // ���x�x�N�g��
	Vector3        mForwardVec  ; // �O�i�����x�N�g��  
	float          mWalkSpeed   ; // �����X�s�[�h
	float          mRunSpeed    ; // ����X�s�[�h
	float          mTurnSpeed   ; // ����X�s�[�h
	int            mHitPoint    ; // �̗�
	bool           mIsOnGround  ; // �n�ʂɂ���H

	SkeletalMeshComponent* mSkelMeshComponent; //���b�V���f�[�^
	std::unordered_map<EnemyStateEnum, const class Animation*> mAnimations; // �A�j���[�V�����Z�b�g
	std::unordered_map<EnemyTriggerEnum, class BoxCollider*>   mTrigerBoxs; // �g���K�[�{�b�N�X
};