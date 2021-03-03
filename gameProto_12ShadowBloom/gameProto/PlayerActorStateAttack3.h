#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateAttack3 : public PlayerActorStateBase
{
public:
	PlayerActorStateAttack3();
	~PlayerActorStateAttack3();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void        Enter(class PlayerActor* owner, float deltaTime)override;
	void        Exit(class PlayerActor* owner, float deltaTime)override;
private:
	float mElapseTime;    // このステートに入ってからの経過時刻
	float mTotalAnimTime; // アニメーション総時間
};