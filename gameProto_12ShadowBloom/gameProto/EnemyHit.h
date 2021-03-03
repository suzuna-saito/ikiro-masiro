#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyHit : public EnemyState
{
public:
	EnemyHit(class EnemyBehaviorComponent* owner);
	~EnemyHit();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float mElapseTime;
	float mTotalAnimTime;
};