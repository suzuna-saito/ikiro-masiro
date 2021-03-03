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

// アイドル状態から他の状態への移行
PlayerState PlayerActorStateIdle::Update(PlayerActor* owner, float deltaTime)
{
	// ジャンプキーが入力された
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


	// コントローラ入力されたか
	bool isControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	//方向キーが入力されたか
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

// アイドル状態への移行処理
void PlayerActorStateIdle::Enter(class PlayerActor* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
