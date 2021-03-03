#pragma once
#include "CameraActor.h"

class ThirdPersonCamera : public CameraActor
{
public:
	ThirdPersonCamera(Actor* targetActor);
	~ThirdPersonCamera();
	void UpdateActor(float deltaTime) override;
	
	void SetRotateZAngle(float angle)  { mRotateZAngle  = angle ; }
	void SetLookDownAngle(float angle) { mLookDownAngle = angle ; }
	void SetCameraLength(float length) { mCameraLength  = length; }
	void SetCameraPos(Vector3& vec)    { mCameraPos     = vec   ; }

	Vector3& GetCameraForwardVec()     { return mCameraForwardVec; }
	Vector3& GetCameraPos()            { return mCameraPos; }

private:
	float mCameraLength;         // �J��������
	float mRotateZAngle;         // �J�������͉�]������]�p
	float mLookDownAngle;        // �J�������~�낵������]�p

	Vector3 mCameraForwardVec;   // �O������
	Vector3 mCameraPos;          // �J�����ʒu
};