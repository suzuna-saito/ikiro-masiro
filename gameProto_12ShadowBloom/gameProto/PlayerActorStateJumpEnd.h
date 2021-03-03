#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateJumpEnd : public PlayerActorStateBase
{
public:
	PlayerActorStateJumpEnd();
	~PlayerActorStateJumpEnd();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float delataTime)override;
};