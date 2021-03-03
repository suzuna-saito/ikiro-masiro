#include "BGBlock.h"
#include "MeshComponent.h"
#include "BoxCollider.h"
#include "Mesh.h"
BGBlock::BGBlock()
	:mMeshComp(nullptr)
{
	printf("Create BGBlock : %d\n", mID);
}

BGBlock::~BGBlock()
{
	printf("Destroy BGBlock : %d\n", mID);
}

void BGBlock::SetMesh(class Mesh* mesh)
{
	// メッシュをセット
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// あたり判定を設定
	mBox = new BoxCollider(this, EnumPhysicsType::EnumBG);
	mBox->SetObjectBox(mesh->GetCollisionBox());
}



