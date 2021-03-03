#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <string>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	: MeshComponent(owner, true)
	, mSkeleton(nullptr)
{
	printf("    Create : SkeletalMeshComponent\n");
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	printf("Remove : SkeletalMeshComponent\n    ");
}

void SkeletalMeshComponent::Draw(Shader* shader)                         // �`��
{
	if (mMesh)
	{
		// Set the world transform                                        ���[���h�ϊ����Z�b�g
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// Set the matrix palette                                         �s��p���b�g���Z�b�g    
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],
			MAX_SKELETON_BONES);
		// Set specular power                                             �X�y�L�����[���x���Z�b�g
		shader->SetFloatUniform("uSpecPower", 100);
		// Set the active texture                                         �e�N�X�`�����Z�b�g 
		//Texture* t = mMesh->GetTexture(mTextureIndex);
		//if (t)
		//{
		//	t->SetActive();
		//}

		SetTextureToShader(shader);

		// Set the mesh's vertex array as active                          ���b�V���̒��_�z����A�N�e�B�u��
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw                                                           �`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;

		// �A�j���[�V���������[�v�A�j���[�V�����Ȃ犪���߂�����
		if (mAnimation->IsLoopAnimation())
		{
			// Wrap around anim time if past duration                         �A�j���������߂��čĐ�
			while (mAnimTime > mAnimation->GetDuration())
			{
				mAnimTime -= mAnimation->GetDuration();
			}
		}
		// ���[�v���Ȃ��A�j���ōĐ����Ԓ�������ŏI���Ԃ܂łƂ���
		else if(mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime = mAnimation->GetDuration();
		}
		// Recompute matrix palette                                      �s��p���b�g�̍Čv�Z
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)  // �A�j���[�V�����̍Đ�
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

// ���݃A�j���[�V�����Đ������H true : �Đ��� / false : �Đ��I��
bool SkeletalMeshComponent::IsPlaying()
{
	if (!mAnimation->IsLoopAnimation())
	{
		if (mAnimTime >= mAnimation->GetDuration())
		{
			return false;
		}
	}
	return true;
}

// �{�[�����̕����񂩂�{�[���C���f�b�N�X������
int SkeletalMeshComponent::GetBoneIndexFromName(const char* boneName) const
{
	std::string name;
	name = boneName;
	return mSkeleton->GetBoneIndexFromName(name);
}

// �{�[���C���f�b�N�X�l���炻�̃{�[���̍ŏI���[���h�s���Ԃ�
void SkeletalMeshComponent::GetMatrixFromBoneIndex(Matrix4& boneWorldMatrix, int boneIndex) const
{
	boneWorldMatrix = mPalette.mEntry[boneIndex] * mOwner->GetWorldTransform();
}

void SkeletalMeshComponent::ComputeMatrixPalette()                              // �s��p���b�g�̌v�Z
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();   // �O���[�o���t�o�C���h�s��z��̎擾
	std::vector<Matrix4> currentPoses;                                         // ���݂̃|�[�Y�s��
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);       // �A�j���������_�̃O���[�o���|�[�Y�̎擾

	// Setup the palette for each bone                                        ���ꂼ��̃{�[���̍s��p���b�g�̃Z�b�g
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix �@�@�@�@�@�s��p���b�g[i] = �O���[�o���t�o�C���h�s��[i]�@�~�@���݂̃|�[�Y�s��[i]  (i�̓{�[��ID)         
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
