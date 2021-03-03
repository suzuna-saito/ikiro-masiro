#include "PlayerActorStateJump.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "Input.h"
#include "Game.h"
#include "AudioManager.h"

#include <iostream>

PlayerActorStateJump::PlayerActorStateJump()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateJump\n");
}

PlayerActorStateJump::~PlayerActorStateJump()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateJump\n");
}

PlayerState PlayerActorStateJump::Update(PlayerActor* owner, float deltaTime)
{
	// �W�����v�J�n�E�W�����v���[�v/�I���A�j��
	const Animation* jumpStart  = owner->GetAnim(PlayerState::PLAYER_STATE_JUMPSTART);
	const Animation* jumpLoop   = owner->GetAnim(PlayerState::PLAYER_STATE_JUMPLOOP);

	// ���ݍĐ����̃A�j�����擾
	SkeletalMeshComponent* mesh = owner->GetSkeletalMeshComp();
	const Animation* nowAnim    = mesh->GetNowPlayingAnimation();
	
	//�W�����v�ړ��v�Z
	Move(owner, deltaTime);

	// �J�n�A�j���I���Ȃ�W�����v���[�v�A�j���Ɉڍs
	if (nowAnim == jumpStart && mesh->IsPlaying())
	{
		mesh->PlayAnimation(jumpLoop);
		return PlayerState::PLAYER_STATE_JUMPLOOP;
	}

	return PlayerState::PLAYER_STATE_JUMPSTART;
}

void PlayerActorStateJump::Enter(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();

	// �A�j���[�V�������W�����v�J�n��
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPSTART));

	// �W�����v�J�n
	Vector3 jumpvec = owner->GetJumpVec();
	jumpvec.z += 320.0f * deltaTime;
	owner->SetJumpVec(jumpvec);
	owner->SetIsJump(true);

	AUDIO->PlaySound("assets/audio/jump.wav");

}

void PlayerActorStateJump::Move(PlayerActor* owner, float deltaTime)
{
	//���݈ʒu�ƃW�����v���x�擾
	Vector3 position = owner->GetPosition();

	//�d�͉����x
	Vector3 jumpVec = owner->GetJumpVec();
	jumpVec.z -= 5.0f * deltaTime;
	owner->SetJumpVec(jumpVec);

	// �󒆂ł̕����L�[�ړ�����
	const float speed = 200.0f;
	Vector3 forwardVec = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = owner->GetForward();

	float nowSpeed = 0.0f;

	// �L�����N�^�[�ړ�
	Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	{
		DirVec -= rightVec;
	}

	if (DirVec.LengthSq() > 0.5f)
	{
		// �����L�[����
		charaForwardVec = DirVec;

		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);

		// ���݂̃X�s�[�h��ۑ�
		nowSpeed = speed * deltaTime;
	}
	else
	{
		charaForwardVec = Vector3(0, 0, 0);
	}
	// �i�s�����Ɉړ�
	position += nowSpeed * charaForwardVec + jumpVec;

	owner->SetPosition(position);
	owner->SetOnground(false);
	owner->SetComputeWorldTransform();

	std::cout << "jumpVec" << jumpVec.x << "," << jumpVec.y << "," << jumpVec.z << std::endl;
	std::cout << "position" << position.x << "," << position.y << ","<< position.z << std::endl;

}

