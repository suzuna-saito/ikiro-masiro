#include "EnemyLookAround.h"

EnemyLookAround::EnemyLookAround(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::LookAround;
	printf("Create : [EnemyState] EnemyLookAround (0x%p)\n", mOwnerActor);
}

EnemyLookAround::~EnemyLookAround()
{
	printf("Remove : [EnemyState] EnemyLookAround (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyLookAround::Update(float deltaTime)
{
	// �A�j���[�V�����Đ��I�������������[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// ���s
	return EnemyStateEnum::LookAround;
}

void EnemyLookAround::OnEnter()
{
	// ���n���A�j���Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::LookAround);
}

void EnemyLookAround::OnExit()
{
}