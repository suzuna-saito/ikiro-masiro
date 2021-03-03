// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor();
	virtual ~Actor();

	void              Update(float deltaTime);                                                     // Update�֐��@Game����R�[�������i�I�[�o�[���C�h�s�j
	void              UpdateComponents(float deltaTime);                                           // �A�^�b�`����Ă�S�ẴR���|�[�l���g�����̍X�V�i�I�[�o�[���C�h�s�j
	virtual void      UpdateActor(float deltaTime);                                                // �A�N�^�[��p�̃A�b�v�f�[�g�@�i�I�[�o�[���C�h�\�j
	void              ProcessInput();                                                              // �Q�[������Ă΂����͊֘A�i�I�[�o�[���C�h�s�j

	// Getters/setters�@�Q�b�^�[�E�Z�b�^�[
	const Vector3&    GetPosition() const { return mPosition; }                                                            // �ʒu�̃Q�b�g
	void              SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }                // �ʒu�̃Z�b�g
	const Vector3&    GetScale() const { return mScale; }                                                                  // �X�P�[�����O�̃Q�b�g
	void              SetScale(float scale);                                                       // �X�P�[�����O�̃Z�b�g
	void              SetScale(const Vector3& scale);
	const Quaternion& GetRotation() const { return mRotation; }                                                            // ��]�i�N�H�[�^�j�I���j�̃Q�b�g
	void              SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }  // ��]�i�N�H�[�^�j�I���j�̃Z�b�g
	void              SetRotation(const Vector3& rotation);

	void              ComputeWorldTransform();                                                     // ���[���h�ϊ��s��̌v�Z
	void              SetComputeWorldTransform() { mRecomputeWorldTransform = true; }              // �ړ��ȂǕϊ��s��̍Čv�Z���K�v�Ȏ�
	const Matrix4&    GetWorldTransform() const { return mWorldTransform; }                        // ���[���h�ϊ��s��̃Q�b�g
	Vector3           GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // �O�i�x�N�g���̃Q�b�g
	Vector3           GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // �E�����x�N�g���̃Q�b�g
	State             GetState() const { return mState; }                                          // �A�N�^�[�̏�ԃQ�b�g
	float             GetSpeed() const { return mSpeed; }                                          // �A�N�^�[�̌��݂̃X�s�[�h
	void              SetSpeed(float speed) { mSpeed = speed; }
	void              RotateToNewForward(const Vector3& forward);                                  // �O�i�x�N�g���̃Z�b�g
	void              SetState(State state) { mState = state; }                                    // �A�N�^�[�̏�ԃZ�b�g
	int               GetID() { return mID; };                                                     // �A�N�^�[�̊Ǘ�ID�̃Q�b�g
	void              AddComponent(class Component* component);                                    // �R���|�[�l���g�̒ǉ� 
	void              RemoveComponent(class Component* component);                                 // �R���|�[�l���g�̍폜 

protected:
	State             mState;                                                                      // �A�N�^�[�̏��
	Matrix4           mWorldTransform;                                                             // ���[���h�ϊ��s��
	Vector3           mPosition;                                                                   // �ʒu
	Quaternion        mRotation;                                                                   // ��]
	Vector3           mRotationXYZ;
	Vector3           mScale;
	float             mSpeed;                                                                      // ���݂̃X�s�[�h
	bool              mRecomputeWorldTransform;                                                    // ���[���h�ϊ��̍Čv�Z���K�v���H
	std::vector<class Component*> mComponents;                                                     // �R���|�[�l���g�z��
	class Game*       mGame;                                                                       // �Q�[���V�X�e���N���X�ւ̃|�C���^
	int               mID;                                                                         // �Ǘ�ID
	static int        mGlobalActorNo;                                                              // �Ǘ�ID�p�̐ÓI�J�E���^
};
