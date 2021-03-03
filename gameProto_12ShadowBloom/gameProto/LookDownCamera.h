#pragma once
#include "CameraActor.h"

class LookDownCamera : public CameraActor
{
public:
	LookDownCamera(Actor *targetActor);
	~LookDownCamera();
	void UpdateActor(float deltaTime) override;

private:
	float mCameraHeight;
	float mCameraBack;
};