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
	// ジャンプ開始・ジャンプループ/終了アニメ
	const Animation* jumpStart  = owner->GetAnim(PlayerState::PLAYER_STATE_JUMPSTART);
	const Animation* jumpLoop   = owner->GetAnim(PlayerState::PLAYER_STATE_JUMPLOOP);

	// 現在再生中のアニメを取得
	SkeletalMeshComponent* mesh = owner->GetSkeletalMeshComp();
	const Animation* nowAnim    = mesh->GetNowPlayingAnimation();
	
	//ジャンプ移動計算
	Move(owner, deltaTime);

	// 開始アニメ終了ならジャンプループアニメに移行
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

	// アニメーションをジャンプ開始へ
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPSTART));

	// ジャンプ開始
	Vector3 jumpvec = owner->GetJumpVec();
	jumpvec.z += 320.0f * deltaTime;
	owner->SetJumpVec(jumpvec);
	owner->SetIsJump(true);

	AUDIO->PlaySound("assets/audio/jump.wav");

}

void PlayerActorStateJump::Move(PlayerActor* owner, float deltaTime)
{
	//現在位置とジャンプ速度取得
	Vector3 position = owner->GetPosition();

	//重力加速度
	Vector3 jumpVec = owner->GetJumpVec();
	jumpVec.z -= 5.0f * deltaTime;
	owner->SetJumpVec(jumpVec);

	// 空中での方向キー移動入力
	const float speed = 200.0f;
	Vector3 forwardVec = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = owner->GetForward();

	float nowSpeed = 0.0f;

	// キャラクター移動
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
		// 方向キー入力
		charaForwardVec = DirVec;

		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);

		// 現在のスピードを保存
		nowSpeed = speed * deltaTime;
	}
	else
	{
		charaForwardVec = Vector3(0, 0, 0);
	}
	// 進行方向に移動
	position += nowSpeed * charaForwardVec + jumpVec;

	owner->SetPosition(position);
	owner->SetOnground(false);
	owner->SetComputeWorldTransform();

	std::cout << "jumpVec" << jumpVec.x << "," << jumpVec.y << "," << jumpVec.z << std::endl;
	std::cout << "position" << position.x << "," << position.y << ","<< position.z << std::endl;

}

