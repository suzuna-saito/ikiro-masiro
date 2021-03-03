#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);

	~SkeletalMeshComponent();

	// Draw this mesh component                                              ���b�V���R���|�[�l���g�̕`��
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters                                                               �X�P���g���̃Z�b�g
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation                 �A�j���[�V�����̍Đ��B�A�j���[�V�����̎c�蒷����Ԃ�
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
	// ���ݍĐ����̃A�j���[�V�����͍Đ������H true : �Đ��� false : �Đ��I��
	bool IsPlaying();

	// ���ݍĐ����̃A�j���[�V����
	const class Animation* GetNowPlayingAnimation() { return mAnimation; }

	float GetPlayTime() { return mAnimTime; }

	int GetBoneIndexFromName(const char* boneName) const;
	void GetMatrixFromBoneIndex( Matrix4& boneWorldMatrix, int boneIndex)const;

protected:
	void ComputeMatrixPalette();                                           // �s��p���b�g�̌v�Z

	MatrixPalette mPalette;                                                // �s��p���b�g
	const class Skeleton* mSkeleton;                                       // �X�P���g��
	const class Animation* mAnimation;                                     // �A�j���[�V����
	float mAnimPlayRate;                                                   // �A�j���[�V�����̍Đ����x
	float mAnimTime;                                                       // �A�j���[�V��������

};
