#include "PlayerActorStateIdle.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"

PlayerActorStateIdle::PlayerActorStateIdle()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateIdle\n");
}

PlayerActorStateIdle::~PlayerActorStateIdle()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateIdle\n");
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerState PlayerActorStateIdle::Update(PlayerActor* owner, float deltaTime)
{
	// �W�����v�L�[�����͂��ꂽ
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_B))
	{
		return PlayerState::PLAYER_STATE_JUMPSTART;
	}
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	{
		return PlayerState::PLAYER_STATE_ATTACK1;
	}
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X))
	{
		return PlayerState::PLAYER_STATE_HIT;
	}


	// �R���g���[�����͂��ꂽ��
	bool isControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	//�����L�[�����͂��ꂽ��
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
  		          INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		          INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		          INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		          isControllerInputOff;

	if (!IsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerActorStateIdle::Enter(class PlayerActor* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
