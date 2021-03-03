// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);       // ���b�V���R���|�[�l���g�R���X�g���N�^�B�i�v�F�e�A�N�^�[�j
	~MeshComponent();

	virtual void Draw(class Shader* shader);                          // ���b�V���R���|�[�l���g�̕`��
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }          // ���b�V���R���|�[�l���g�Ŏg�p���郁�b�V���̃Z�b�g
	void SetTextureIndex(size_t index) { mTextureIndex = index; }     // �e�N�X�`���̃Z�b�g

	void SetVisible(bool visible) { mVisible = visible; }             // �\���t���O�̃Z�b�g
	bool GetVisible() const { return mVisible; }                      // �\���t���O�̃Q�b�g

	bool GetIsSkeletal() const { return mIsSkeletal; }                // �X�P���^�����f�����ǂ����̎擾
protected:
	virtual void SetTextureToShader(class Shader* shader);
	class Mesh* mMesh;                                                // ���b�V���I�u�W�F�N�g�̎擾
	size_t mTextureIndex;                                             // �e�N�X�`���[�̃C���f�b�N�X
	bool mVisible;                                                    // �\���t���O
	bool mIsSkeletal;                                                 // �X�P���^�����f�����ǂ����̃t���O
};