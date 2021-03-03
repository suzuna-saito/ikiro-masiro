// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);                      // �R���X�g���N�^ updateOder���������Ƒ������s�����
	virtual ~Component();                                                      // �f�X�g���N�^
	virtual void Update(float deltaTime);                                      // �f���^�^�C���ɂ��R���|�[�l���g�̍X�V (�I�[�o�[���C�h�j
	virtual void ProcessInput() {}                                             // �R���|�[�l���g�̓��͏����i�I�[�o�[���C�h�j
	virtual void OnUpdateWorldTransform() { }                                  // ���[���h�ϊ����K�v�Ȏ��ɌĂ΂��֐��i�I�[�o�[���C�h�j
	int GetID() { return mID; }
	class Actor* GetOwner() { return mOwner; }                                 // �I�[�i�[�A�N�^�[�̎擾
	int GetUpdateOrder() const { return mUpdateOrder; }                        // update order�̎擾
protected:
	class Actor* mOwner;                                                       // �I�[�i�[�A�N�^�[�ւ̃|�C���^
	int mUpdateOrder;                                                          // �R���|�[�l���g�̍X�V����
	int mID;                                                                   // �R���|�[�l���g�̊Ǘ�ID

	static int mGlobalID;                                                      // �R���|�[�l���g�̊Ǘ�ID�̘A�ԗp
};
