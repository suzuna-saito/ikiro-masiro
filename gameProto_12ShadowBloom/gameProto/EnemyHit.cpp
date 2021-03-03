#include "EnemyHit.h"

EnemyHit::EnemyHit(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::GetDamage;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyHit::~EnemyHit()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyHit::Update(float deltaTime)
{
	Vector3 pos, backward;
	pos = mOwnerActor->GetPosition();
	backward = -1.0f * mOwnerActor->GetForwardVec();

	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	//ノックバック移動
	const float speed = 10.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//経過割合をもとに移動処理
	pos += Math::EaseOut(percentage, speed, 0.0f) * backward;

	mOwnerActor->SetPosition(pos);

	return EnemyStateEnum::GetDamage;
}

void EnemyHit::OnEnter()
{
	mElapseTime = 0.0f;
	mTotalAnimTime = mOwnerActor->PlayAnimation(EnemyStateEnum::GetDamage);
}

void EnemyHit::OnExit()
{
}
