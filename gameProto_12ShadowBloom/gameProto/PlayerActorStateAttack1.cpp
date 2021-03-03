#include "PlayerActorStateAttack1.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"
#include "Input.h"
#include "SwordEffectActor.h"

PlayerActorStateAttack1::PlayerActorStateAttack1()
	: mIsNextCombo(false)
	, mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerActorStateAttack1::~PlayerActorStateAttack1()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerState PlayerActorStateAttack1::Update(PlayerActor* owner, float deltaTime)
{
	// �A�j���[�V�������I��������A�C�h����Ԃ��A���̃R���{��
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		if (mIsNextCombo)
		{
			return PlayerState::PLAYER_STATE_ATTACK2;
		}
		return PlayerState::PLAYER_STATE_IDLE;
	}

	// �U���{�^�������ꂽ�玟�̃X�e�[�g�ֈڍs���鏀��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_CONFIG::KEY_A))
	{
		mIsNextCombo = true;
	}
	// �U�����ݍ��݈ړ��̂��߂̃A�j���[�V�����Đ����Ԃ̌o�ߊ������v�Z
	mElapseTime += deltaTime;
	const float speed = 5.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// �o�ߊ��������ƂɈړ�����
	Vector3 pos, forward;
	pos     = owner->GetPosition();
	forward = owner->GetForward();	
	pos += Math::EaseIn(percentage ,speed ,0.0f) * forward;

	owner->SetPosition(pos);
	
	return PlayerState::PLAYER_STATE_ATTACK1;
}

void PlayerActorStateAttack1::Enter(PlayerActor* owner, float deltaTime)
{

	// ATTACK1�̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK1));
	mIsNextCombo = false;

	// �A�j���[�V�����Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK1)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox(1.5f);

	// effect����
	SwordEffectActor* effect = new SwordEffectActor(owner);
	Vector3 localPos, localAngle;
	localPos = Vector3(50, 0, 100);
	localAngle = Vector3(Math::ToRadians(30.0f), 0, Math::ToRadians(-30.0f));
	effect->SetLocalPos(localPos);
	effect->SetLocalRotation(localAngle);
	effect->SetEffectTime(mTotalAnimTime);
}

void PlayerActorStateAttack1::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
