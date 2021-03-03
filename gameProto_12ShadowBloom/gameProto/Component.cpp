// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Component.h"
#include "Actor.h"

int Component::mGlobalID = 0;


// �R���|�[�l���g�R���X�g���N�^
// ���� : in owner�@     �R���|�[�l���g����������e�A�N�^�[�ւ̃|�C���^
//        in updateOrder �X�V�����i�������قǑ������s�����j
Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	// �A�N�^�[�ɂ��̃R���|�[�l���g��ǉ�
	mOwner->AddComponent(this);
	mID = mGlobalID;
	mGlobalID++;
}

// �R���|�[�l���g�f�X�g���N�^
// ���̃R���|�[�l���g����������e�A�N�^�[����폜����
Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{

}
