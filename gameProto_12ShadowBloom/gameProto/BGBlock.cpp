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
	// ���b�V�����Z�b�g
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �����蔻���ݒ�
	mBox = new BoxCollider(this, EnumPhysicsType::EnumBG);
	mBox->SetObjectBox(mesh->GetCollisionBox());
}



