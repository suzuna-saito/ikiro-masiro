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
	// アニメーションが終了したらcStopTime硬直後、IDLE状態へ
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

	// 攻撃踏み込み移動のためのアニメーション再生時間の経過割合を計算
	const float speed = 10.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	// 経過割合をもとに移動処理
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_ATTACK3;
}

void PlayerActorStateAttack3::Enter(PlayerActor* owner, float deltaTime)
{
	// ATTACK3のアニメーション再生
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK3));

	// アニメーション再生時間取得
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK3)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox(1.5f);
	// 剣エフェクト発生
	SwordEffectActor* effect = new SwordEffectActor(owner);
	effect->SetLocalPos(swordLocalPos);
	effect->SetLocalRotation(swordLocalAngle);
	effect->SetEffectTime(mTotalAnimTime + cStopTime);
}

void PlayerActorStateAttack3::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
