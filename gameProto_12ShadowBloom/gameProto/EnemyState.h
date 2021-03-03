#pragma once
#include "EnemyActorBase.h"
#include "EnemyBehaviorComponent.h"

class EnemyState
{
public:
	EnemyState(class EnemyBehaviorComponent* owner)
		: mOwnerComponent(owner)
		, mStateType(EnemyStateEnum::Invalid)
	{ 
		mOwnerActor = mOwnerComponent->GetOwnerActor();
	}
	virtual ~EnemyState() {};

	//��Ԃ��Ƃ̂ӂ�܂�
	virtual EnemyStateEnum Update(float deltaTime) = 0;
	virtual void           OnEnter() = 0;
	virtual void           OnExit() = 0;

	EnemyStateEnum         GetStateType() { return mStateType; }

protected:
	class EnemyBehaviorComponent* mOwnerComponent;
	class EnemyActorBase*         mOwnerActor;
	EnemyStateEnum                mStateType;
	
};