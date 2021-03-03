#include "PlayerActorStateAttack3.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"
#include "Input.h"
#include "SwordEffectActor.h"

const float cStopTime = 1.0f;
const Vector3 swordLocalPos(0, 0, 100);
const Vector3 swordLocalAngle(Math::ToRadians(80.0f), Math::ToRadians(40.0f), 0);

PlayerActorStateAttack3::PlayerActorStateAttack3()
	: mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack3\n");
}

PlayerActorStateAttack3::~PlayerActorStateAttack3()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack3\n");
}

PlayerState PlayerActorStateAttack3::Update(PlayerActor* owner, float deltaTime)
{
	mElapseTime += deltaTime;
	// �A�j���[�V�������I��������cStopTime�d����AIDLE��Ԃ�
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		if (mElapseTime - mTotalAnimTime > cStopTime)
		{
			owner->RemoveAttackHitBox();
			return PlayerState::PLAYER_STATE_IDLE;
		}
		else
		{
			return PlayerState::PLAYER_STATE_ATTACK3;
		}
	}

	// �U�����ݍ��݈ړ��̂��߂̃A�j���[�V�����Đ����Ԃ̌o�ߊ������v�Z
	const float speed = 10.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// �o�ߊ��������ƂɈړ�����
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_ATTACK3;
}

void PlayerActorStateAttack3::Enter(PlayerActor* owner, float deltaTime)
{
	// ATTACK3�̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK3));

	// �A�j���[�V�����Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK3)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox(1.5f);
	// ���G�t�F�N�g����
	SwordEffectActor* effect = new SwordEffectActor(owner);
	effect->SetLocalPos(swordLocalPos);
	effect->SetLocalRotation(swordLocalAngle);
	effect->SetEffectTime(mTotalAnimTime + cStopTime);
}

void PlayerActorStateAttack3::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
