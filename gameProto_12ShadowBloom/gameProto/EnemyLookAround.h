#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyLookAround : public EnemyState
{
public:
	EnemyLookAround(class EnemyBehaviorComponent* owner);
	~EnemyLookAround();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

};