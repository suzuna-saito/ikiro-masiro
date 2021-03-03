// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

// ���b�V���R���|�[�l���g�@owner�ƃX�L�����b�V�����̏������
MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
	, mIsSkeletal(isSkeletal)
{
	GAMEINSTANCE.GetRenderer()->AddMeshComponent(this);
	printf("Create : MeshComponent [%5d] owner->( 0x%p )\n", GetID(), owner);
}

MeshComponent::~MeshComponent()
{
	printf("Remove : MeshComponent [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetRenderer()->RemoveMeshComponent(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform�@���[���h�ϊ����Z�b�g
		shader->SetMatrixUniform("uWorldTransform",
			 mOwner->GetWorldTransform());

		// Set specular power�@�X�y�L�������x�Z�b�g
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		shader->SetFloatUniform("uLuminance", mMesh->GetLuminace());

		// ���b�V���ɒ�`����Ă���e�N�X�`�����Z�b�g
		SetTextureToShader(shader);

		// Set the mesh's vertex array as active�@���_�z����A�N�e�B�u��
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw�@�`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void MeshComponent::SetTextureToShader(Shader* shader)
{
	// ���b�V���e�N�X�`���Z�b�g
	int texID, stageCount = 0;
	texID = mMesh->GetTextureID(TextureStage::DiffuseMap); // �f�B�t���[�Y
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uDiffuseMap", stageCount);
		stageCount++;
	}
	texID = mMesh->GetTextureID(TextureStage::NormalMap); // �@���}�b�v
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uNormalMap", stageCount);
		stageCount++;
	}
	texID = mMesh->GetTextureID(TextureStage::SpecularMap); // �X�y�L�����[�}�b�v
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uSpecularMap", stageCount);
		stageCount++;
	}
	texID = mMesh->GetTextureID(TextureStage::EmissiveMap); // ���ȕ��˃}�b�v
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uEmissiveMap", stageCount);
		stageCount++;
	}
}
