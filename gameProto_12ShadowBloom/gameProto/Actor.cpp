// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

int Actor::mGlobalActorNo = 0;

// Actor�R���X�g���N�^  
Actor::Actor()
	: mState(EActive)
	, mGame(nullptr)
	, mPosition(Vector3::Zero)
	, mRotation(Quaternion::Identity)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(0.0f)
{
	mScale = Vector3(1, 1, 1);
	//�Q�[���V�X�e���{�̂� �A�N�^�[��ǉ�
	GAMEINSTANCE.AddActor(this);
	mGlobalActorNo++;
}

Actor::~Actor()
{
	// �A�N�^�[�������Ă���R���|�[�l���g�̍폜
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	// �Q�[���V�X�e���{�̂ɂ��̃A�N�^�[�̍폜���˗�
	GAMEINSTANCE.RemoveActor(this);
}

// Update�֐��@Game����R�[�������
// ���� in : deltaTime  1�t���[�����̌o�ߎ���
void Actor::Update(float deltaTime)
{
	// �A�N�^�[�������Ă���Ƃ��̂ݏ���
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

// �S�ẴR���|�[�l���g�̍X�V����
// ���� in : deltaTime  1�t���[�����̌o�ߎ���
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

// ���̃A�N�^�[�Ǝ��̍X�V���� �i�K�v�Ȃ�I�[�o�[���C�h�j
void Actor::UpdateActor(float deltaTime)
{
}

// ���̃A�N�^�[�������Ă���R���|�[�l���g�̓��͏���
void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		// ���͏������󂯎��R���|�[�l���g��D�悵�Ď��s
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

void Actor::SetScale(float scale)
{
	mScale = Vector3(scale, scale, scale);
	mRecomputeWorldTransform = true;
}

void Actor::SetScale(const Vector3& scale)
{
	mScale = scale;
	mRecomputeWorldTransform = true;
}

// forward�x�N�g���̌����ɉ�]����
// in forward : �����������O�������x�N�g��
void Actor::RotateToNewForward(const Vector3& forward)
{
	// X���x�N�g��(1,0,0)��forward�̊Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����forward�Ƃ̊O�ς����]�������Ƃ߁A��]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

// x,y,z��]�l�����]���Z�b�g
void Actor::SetRotation(const Vector3& rotation)
{
	Matrix4 mat;
	mat =  Matrix4::CreateRotationX(rotation.x);
	mat *= Matrix4::CreateRotationY(rotation.y);
	mat *= Matrix4::CreateRotationZ(rotation.z);

	mRotation = Quaternion::QuaeternionFromMatrix(mat);
	mRecomputeWorldTransform = true;
}

// ���[���h�ϊ��s����v�Z
// ���� �Ȃ�
void Actor::ComputeWorldTransform()
{
	//���[���h�ϊ��̍Čv�Z���K�v�Ȃ���s
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ��𑣂�
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// �R���|�[�l���g�̒ǉ�
// �����@in : component �ǉ��R���|�[�l���g�ւ̃|�C���^
void Actor::AddComponent(Component* component)
{
	// �R���|�[�l���g���\�[�g���Ēǉ�
	// �����̃I�[�_�[�ԍ������傫���}���_��������
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// �v�f���������|�C���g�̎�O�ɑ}������
	mComponents.insert(iter, component);
}

// �R���|�[�l���g�̍폜
// �����@in : component  �폜�R���|�[�l���g�ւ̃|�C���^ 
void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
