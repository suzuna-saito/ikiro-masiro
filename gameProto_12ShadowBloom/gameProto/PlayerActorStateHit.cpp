#include "PlayerActorStateHit.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"

const float cStopTime = 0.5f;// �d������

PlayerActorStateHit::PlayerActorStateHit()
	: mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateHit\n");
}

PlayerActorStateHit::~PlayerActorStateHit()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateHit\n");
}

PlayerState PlayerActorStateHit::Update(PlayerActor* owner, float deltaTime)
{
	mElapseTime += deltaTime;

	// �_���[�W�A�j���I�����H
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		//�_���[�W�d�����ԏI��������A�C�h����Ԃ֕��A
		if (mElapseTime - mTotalAnimTime > cStopTime)
		{
			return PlayerState::PLAYER_STATE_IDLE;
		}
		else
		{
			return PlayerState::PLAYER_STATE_HIT;
		}
	}

	// �m�b�N�o�b�N�ړ�
	const float speed = 15.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// �o�ߊ��������ƂɈړ�����
	Vector3 pos, backward;
	pos      = owner->GetPosition();
	backward = owner->GetForward() * -1.0f;
	pos += Math::EaseOut(percentage, speed, 0.0f) * backward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_HIT;

}

void PlayerActorStateHit::Enter(PlayerActor* owner, float deltaTime)
{
	// HIT�A�j���[�V�����Đ�
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_HIT));

	// �A�j���[�V�����Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_HIT)->GetDuration();
	mElapseTime = 0.0f;

}
