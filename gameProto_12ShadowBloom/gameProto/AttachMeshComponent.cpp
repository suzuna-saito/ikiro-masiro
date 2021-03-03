#include "AttachMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include <typeinfo>

AttachMeshComponent::AttachMeshComponent(Actor* owner, SkeletalMeshComponent* skMesh, const char* AttachBoneName)
	: MeshComponent(owner, false)
	, mAttachBoneIndex(0)
    , mAttachSkeletalMesh(skMesh)
{
	mAttachBoneIndex = skMesh->GetBoneIndexFromName(AttachBoneName);
	printf("Create : AttachMeshComponent [%d]\n", mID);
}

AttachMeshComponent::~AttachMeshComponent()
{
	printf("Remove : AttachMeshComponent [%d]\n", mID);
}

void AttachMeshComponent::SetOffsetPosition(Vector3& offset)
{
	mOffsetPos = Matrix4::CreateTranslation(offset);
}

void AttachMeshComponent::SetOffsetRotation(Vector3& rotation)
{
	mOffsetRotation = Matrix4::CreateRotationY(rotation.y)
		            * Matrix4::CreateRotationX(rotation.x)
		            * Matrix4::CreateRotationZ(rotation.z);
}

void AttachMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		Matrix4 finalMat, animationMat;
		mAttachSkeletalMesh->GetMatrixFromBoneIndex(animationMat, mAttachBoneIndex);
		finalMat = mOffsetRotation * mOffsetPos * animationMat;

		// �A�^�b�`�ʒu�Z�o���X�V
		Vector3 pos(0, 0, 0);
		mComputeAttachPos   = Vector3::Transform(pos, finalMat);
		mComputeTransMatrix = finalMat;

		shader->SetMatrixUniform("uWorldTransform",finalMat);

		// Set specular power�@�X�y�L�������x�Z�b�g
		shader->SetFloatUniform("uSpecPower", 100);

		// Set the active texture�@�A�N�e�B�u�e�N�X�`���Z�b�g
		//Texture* t = mMesh->GetTexture(mTextureIndex);
		//if (t)
		//{
		//	t->SetActive();
		//}

		SetTextureToShader(shader);

		// Set the mesh's vertex array as active�@���_�z����A�N�e�B�u��
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// Draw�@�`�悷��[
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
