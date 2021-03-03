#include "PlayerActorStateHit.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"
#include "Animation.h"

const float cStopTime = 0.5f;// 硬直時間

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

	// ダメージアニメ終了か？
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		//ダメージ硬直時間終了したらアイドル状態へ復帰
		if (mElapseTime - mTotalAnimTime > cStopTime)
		{
			return PlayerState::PLAYER_STATE_IDLE;
		}
		else
		{
			return PlayerState::PLAYER_STATE_HIT;
		}
	}

	// ノックバック移動
	const float speed = 15.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// 経過割合をもとに移動処理
	Vector3 pos, backward;
	pos      = owner->GetPosition();
	backward = owner->GetForward() * -1.0f;
	pos += Math::EaseOut(percentage, speed, 0.0f) * backward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_HIT;

}

void PlayerActorStateHit::Enter(PlayerActor* owner, float deltaTime)
{
	// HITアニメーション再生
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_HIT));

	// アニメーション再生時間取得
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_HIT)->GetDuration();
	mElapseTime = 0.0f;

}
