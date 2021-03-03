#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateIdle : public PlayerActorStateBase
{
public:
	PlayerActorStateIdle();
	~PlayerActorStateIdle();
	PlayerState Update(PlayerActor* owner, float deltaTime) override;
	void Enter(class PlayerActor* owner, float deltaTime)override;
};