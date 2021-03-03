#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateRun : public PlayerActorStateBase
{
public:
	PlayerActorStateRun();
	~PlayerActorStateRun();
	PlayerState Update(PlayerActor* owner, float deltaTime) override;
	void Enter(class PlayerActor* owner, float deltaTime)override;
private:
	void MoveCalc(PlayerActor* owner, float deltaTime);
};

