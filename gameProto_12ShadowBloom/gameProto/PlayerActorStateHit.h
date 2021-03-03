#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateHit : public PlayerActorStateBase
{
public:
	PlayerActorStateHit();
	~PlayerActorStateHit();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void        Enter(class PlayerActor* owner, float deltaTime)override;
private:
	float mElapseTime;
	float mTotalAnimTime;

};