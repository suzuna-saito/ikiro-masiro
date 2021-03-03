#pragma once
#include "Actor.h"
class Coin : public Actor
{
public:
	Coin();
	~Coin();
	void SetMesh(class Mesh* mesh);
	void UpdateActor(float deltaTime) override;
	void GetCoin();
private:
	class MeshComponent* mMeshComp;
	class BoxCollider*   mBox;
	float mAngle;
};