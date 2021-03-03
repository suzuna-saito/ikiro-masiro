#pragma once
#include "Actor.h"

class SwordEffectActor : public Actor
{
public:
	SwordEffectActor(Actor* effectOrignActor);
	~SwordEffectActor();
	void UpdateActor(float deltaTime)override ;
	void SetEffectTime(float effectTime) { mEffectTime = effectTime; }
	void SetLocalRotation(const Vector3& angleRad);
	void SetLocalPos(const Vector3& pos) { mLocalPos = pos; }

private:
	Actor*  mEffectOrign;
	float   mEffectTime;
	Vector3 mLocalPos;
	Matrix4 mLocalRotation;
};