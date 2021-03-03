
#include "LookDownCamera.h"

LookDownCamera::LookDownCamera(Actor * targetActor)
	:CameraActor(targetActor)
{
	mCameraHeight = 700;
	mCameraBack = 650;
}

LookDownCamera::~LookDownCamera()
{
}

void LookDownCamera::UpdateActor(float deltaTime)
{
	// 接近係数
	const float dumper = 1.5f;
	// 注視点とキャラ座標の差
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();

	// 差分ベクトルからプレーヤー位置まで近づける
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	// 注視点・カメラ位置をセット
	mViewTarget = mViewTarget + diff;
	mPosition = mViewTarget;
	mPosition.x -= mCameraBack;
	mPosition.z = mCameraHeight;

}
