#pragma once
#include "Actor.h"
#include "Collision.h"

class LevelActor : public Actor
{
public:
	LevelActor();
	~LevelActor();

	void UpdateActor(float deltaTime)override;
	void LoadLevel(const char* gpmeshFileName, const char* collisionJsonFileName, Vector3& offset);

};