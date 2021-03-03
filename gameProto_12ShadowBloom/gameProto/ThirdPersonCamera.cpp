#include "ThirdPersonCamera.h"
#include "Input.h"
#include "InputController.h"

const float maxLookDownAngle = Math::ToRadians(70.0f); // �ő�p
const float minLookDownAngle = Math::ToRadians(20.0f); // �ŏ��p

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
	// �ڋߌW��
	const float dumper = 1.5f;
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();

	// �����x�N�g������v���[���[�ʒu�܂ŋ߂Â���
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	// �L�[����
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

	// �p�b�h����
	Vector2 axisR;
	axisR = INPUT_INSTANCE.GetRStick();
	mRotateZAngle  += axisR.x * rotate;
	mLookDownAngle += axisR.y * rotate;

	// ���~�낵�p�x�̊p�x����
	if (mLookDownAngle < minLookDownAngle)
	{
		mLookDownAngle = minLookDownAngle;
	}
	if (mLookDownAngle > maxLookDownAngle)
	{
		mLookDownAngle = maxLookDownAngle;
	}

	// ���[��]�E�s�b�`��]
	Vector3 rotatePos;
	rotatePos.x = mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle); 

	// �����_�E�J�����ʒu���Z�b�g
	mPosition = rotatePos + playerPos;
	mViewTarget = playerPos;
}
