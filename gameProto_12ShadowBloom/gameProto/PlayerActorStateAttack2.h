#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateAttack2 : public PlayerActorStateBase
{
public:
	PlayerActorStateAttack2();
	~PlayerActorStateAttack2();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void        Enter(class PlayerActor* owner, float deltaTime)override;
	void        Exit(class PlayerActor* owner, float deltaTime)override;

private:
	bool  mIsNextCombo;   // 次のコンボにつなげるか  
	float mElapseTime;    // このステートに入ってからの経過時刻
	float mTotalAnimTime; // アニメーション総時間

};