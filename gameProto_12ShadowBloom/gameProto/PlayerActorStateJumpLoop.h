#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateJumpLoop : public PlayerActorStateBase
{
public:
	PlayerActorStateJumpLoop();
	~PlayerActorStateJumpLoop();
	PlayerState Update(PlayerActor* owner, float deltaTime);
	void Enter(class PlayerActor* owner, float deltaTime)override;

private:
	void Move(class PlayerActor* owner, float deltaTime);
};