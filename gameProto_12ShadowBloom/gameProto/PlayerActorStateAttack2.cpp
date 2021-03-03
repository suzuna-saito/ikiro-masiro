#include "PlayerActorStateAttack2.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"
#include "Input.h"
#include "SwordEffectActor.h"

PlayerActorStateAttack2::PlayerActorStateAttack2()
	: mIsNextCombo(false)
	, mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack2\n");
}

PlayerActorStateAttack2::~PlayerActorStateAttack2()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerState PlayerActorStateAttack2::Update(PlayerActor* owner, float deltaTime)
{
	// �A�j���[�V�������I��������A�C�h����Ԃ��A���̃R���{��
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		if (mIsNextCombo)
		{

			return PlayerState::PLAYER_STATE_ATTACK3;
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
	const float speed = 8.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// �o�ߊ��������ƂɈړ�����
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_ATTACK2;
}

void PlayerActorStateAttack2::Enter(PlayerActor* owner, float deltaTime)
{
	// ATTACK1�̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK2));
	mIsNextCombo = false;

	// �A�j���[�V�����Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK2)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox(1.5f);

	// ���G�t�F�N�g����
	SwordEffectActor* effect = new SwordEffectActor(owner);
	Vector3 localPos, localAngle;
	localPos = Vector3(50, 0, 100);
	localAngle = Vector3(Math::ToRadians(-180.0f), 0, 0);
	effect->SetLocalPos(localPos);
	effect->SetLocalRotation(localAngle);
	effect->SetEffectTime(mTotalAnimTime);
}

void PlayerActorStateAttack2::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
