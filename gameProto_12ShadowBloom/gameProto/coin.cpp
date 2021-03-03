#include "coin.h"
#include "MeshComponent.h"
#include "BoxCollider.h"
#include "Mesh.h"
#include "AudioManager.h"

Coin::Coin()
	:mMeshComp(nullptr)
	,mAngle(0.0f)
{
}

Coin::~Coin()
{
}

void Coin::SetMesh(Mesh * mesh)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mesh);

	mBox = new BoxCollider(this, EnumPhysicsType::EnumCoin);
	mBox->SetObjectBox(mesh->GetCollisionBox());
}

void Coin::UpdateActor(float deltaTime)
{
	const float coinAnimSpeed = 0.15f;
	mAngle += coinAnimSpeed;
	Quaternion q;

	q.Set(0, 0, sinf(mAngle*0.25f), cosf(mAngle*0.25f));
	q.Normalize();
	SetRotation(q);
	
	// �s��Čv�Z
	mRecomputeWorldTransform = true;
}

void Coin::GetCoin()
{
	//�����ŁA�R�C����炷�T�E���h���Đ�
	AUDIO->PlaySound("assets/audio/coin.wav");
	SetState(Actor::EDead);
}