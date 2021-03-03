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
	float mCameraLength;         // ƒJƒƒ‰‹——£
	float mRotateZAngle;         // ƒJƒƒ‰üˆÍ‰ñ“]•ûŒü‰ñ“]Šp
	float mLookDownAngle;        // ƒJƒƒ‰Œ©~‚ë‚µ•ûŒü‰ñ“]Šp

	Vector3 mCameraForwardVec;   // ‘O•û•ûŒü
	Vector3 mCameraPos;          // ƒJƒƒ‰ˆÊ’u
};