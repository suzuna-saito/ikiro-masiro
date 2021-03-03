#pragma once
#include "Actor.h"

class CameraActor 
{
public:
	CameraActor( Actor* targetActor );
	virtual ~CameraActor();
	virtual void UpdateActor(float deltaTime);

	void     Update(float deltaTime);      //�I�[�o�[���C�h�s��
	void     SetActive();
	void     Init(const Vector3 & cameraPos, const Vector3 & targetPos, const Vector3& upVec);
	Matrix4& GetViewMatrix() { return mViewMatrix; }
	Vector3& GetViewTarget() { return mViewTarget; }
	Vector3& GetViewPos()    { return mPosition  ; }

protected:
	Actor   *mTargetActor;  // �J��������Ƃ���A�N�^�[
	Matrix4  mViewMatrix;   // �r���[�s��
	Vector3  mPosition;
	Vector3  mViewTarget;   // �J�������ړ_
	Vector3  mViewVector;   // �����x�N�g��
	
};