#pragma once
#include "Math.h"
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation
{
public:
	bool Load(const std::string& fileName, bool loop);                               // �A�j���[�V�����ǂݍ���

	size_t GetNumBones() const { return mNumBones; }                                 // �{�[�����̎擾
	size_t GetNumFrames() const { return mNumFrames; }                               // �t���[�����̎擾
	float  GetDuration() const { return mDuration; }                                 // �A�j���[�V�������Ԃ̎擾
	float  GetFrameDuration() const { return mFrameDuration; }                       // �t���[�����Ԃ̎擾
	bool   IsLoopAnimation() const { return mIsLoopAnimation; }                      // �A�j���[�V�����̓��[�v�A�j���[�V�������H

	// Fills the provided vector with the global (current) pose matrices for each   �w�肳�ꂽ�z����A�A�j���[�V�����̎w�肳�ꂽ���ԂɁA
	// bone at the specified time in the animation. It is expected that the time	�e�{�[���̃O���[�o���i���݂́j�|�[�Y�}�g���b�N�X�Ŗ��߂܂��B
	// is >= 0.0f and <= mDuration													���Ԃ� > = 0.0f���� <= ���Ԃł���Ɨ\�z�����
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;
private:
	// Number of bones for the animation                         �A�j���[�V�����̂��߂̃{�[����
	size_t mNumBones;
	// Number of frames in the animation                         �A�j���[�V�����̃t���[����
	size_t mNumFrames;
	// Duration of the animation in seconds                      �A�j���[�V�����̍Đ�����
	float mDuration;
	// Duration of each frame in the animation                   �A�j���[�V�����̃t���[���Ԃ̎���
	float mFrameDuration;
	// Transform information for each frame on the track         �g���b�N��̊e�t���[���̕ϊ����B
	// Each index in the outer vector is a bone, inner vector	 �O���̃x�N�g���̊e�C���f�b�N�X�̓{�[���A
	// is a frame												 �����̃x�N�g���̓t���[���ł��B�@�@�@�@�@�@�@mTracks[�{�[��][�t���[����]
	std::vector<std::vector<BoneTransform>> mTracks;
	bool mIsLoopAnimation;                                       // ���[�v�A�j���[�V�������邩�H
};
