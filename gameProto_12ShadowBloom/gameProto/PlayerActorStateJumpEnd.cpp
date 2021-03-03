#include "PlayerActorStateJumpEnd.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"

PlayerActorStateJumpEnd::PlayerActorStateJumpEnd()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateJumpEnd\n");
}

PlayerActorStateJumpEnd::~PlayerActorStateJumpEnd()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateJumpEnd\n");
}

PlayerState PlayerActorStateJumpEnd::Update(PlayerActor* owner, float deltaTime)
{
	if (owner->GetSkeletalMeshComp()->IsPlaying())
	{
		return PlayerState::PLAYER_STATE_JUMPEND;
	}
	// 再生終了ならアイドル状態へ
	return PlayerState::PLAYER_STATE_IDLE;
}

void PlayerActorStateJumpEnd::Enter(PlayerActor* owner, float delataTime)
{
	// アニメーションをジャンプ開始へ
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPEND));
	AUDIO->PlaySound("assets/audio/jump.wav");
}
