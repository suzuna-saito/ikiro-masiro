#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateJump : public PlayerActorStateBase
{
public:
	PlayerActorStateJump();
	~PlayerActorStateJump();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float deltaTime)override;

private:
	void Move(class PlayerActor* owner, float deltaTime);
};