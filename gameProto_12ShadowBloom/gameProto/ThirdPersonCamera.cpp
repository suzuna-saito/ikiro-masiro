#include "ThirdPersonCamera.h"
#include "Input.h"
#include "InputController.h"

const float maxLookDownAngle = Math::ToRadians(70.0f); // 最大仰角
const float minLookDownAngle = Math::ToRadians(20.0f); // 最小仰角

ThirdPersonCamera::ThirdPersonCamera(Actor* targetActor)
	: CameraActor(targetActor)
	, mCameraLength(0.0f)
	, mRotateZAngle(Math::Pi)
	, mLookDownAngle(maxLookDownAngle)
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::UpdateActor(float deltaTime)
{
	// 接近係数
	const float dumper = 1.5f;
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();

	// 差分ベクトルからプレーヤー位置まで近づける
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	// キー入力
	const float rotate = 0.5f * deltaTime;
	if (INPUT_INSTANCE.IsKeyPressed(KEY_L))
	{
		mRotateZAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_R))
	{
		mRotateZAngle -= rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_X))
	{
		mLookDownAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_Y))
	{
		mLookDownAngle -= rotate;
	}

	// パッド入力
	Vector2 axisR;
	axisR = INPUT_INSTANCE.GetRStick();
	mRotateZAngle  += axisR.x * rotate;
	mLookDownAngle += axisR.y * rotate;

	// 見降ろし角度の角度制限
	if (mLookDownAngle < minLookDownAngle)
	{
		mLookDownAngle = minLookDownAngle;
	}
	if (mLookDownAngle > maxLookDownAngle)
	{
		mLookDownAngle = maxLookDownAngle;
	}

	// ヨー回転・ピッチ回転
	Vector3 rotatePos;
	rotatePos.x = mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle); 

	// 注視点・カメラ位置をセット
	mPosition = rotatePos + playerPos;
	mViewTarget = playerPos;
}
