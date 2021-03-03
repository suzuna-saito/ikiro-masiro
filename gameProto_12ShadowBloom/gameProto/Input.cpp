#include "Input.h"
#include "InputController.h"

const float stickTrigThreshold = 0.01f;

Input::Input()
	: mNowKey(0)
	, mPrevKey(1)
	, mController(nullptr)
{
	// �L�[�{�[�h������
	mNowKey = 0;
	mPrevKey = 1;

	// �L�[�{�[�h��Ԃ̏�����
	for (int i = 0; i < 256; i++)
	{
		mKey[i] = KEY_STATE_OFF;
		mKeyState[mNowKey][i] = KEY_STATE_OFF;
		mKeyState[mPrevKey][i] = KEY_STATE_OFF;
	}

	// �L�[�R���t�B�O�i�{���͊O������ǂݍ��݂����Ƃ���j
	mKeyConfig[KEY_A]     = SDL_SCANCODE_Z;
	mKeyConfig[KEY_B]     = SDL_SCANCODE_X;
	mKeyConfig[KEY_X]     = SDL_SCANCODE_C;
	mKeyConfig[KEY_Y]     = SDL_SCANCODE_V;
	mKeyConfig[KEY_L]     = SDL_SCANCODE_Q;
	mKeyConfig[KEY_R]     = SDL_SCANCODE_E;
	mKeyConfig[KEY_START] = SDL_SCANCODE_0;
	mKeyConfig[KEY_UP]    = SDL_SCANCODE_UP;
	mKeyConfig[KEY_DOWN]  = SDL_SCANCODE_DOWN;
	mKeyConfig[KEY_LEFT]  = SDL_SCANCODE_LEFT;
	mKeyConfig[KEY_RIGHT] = SDL_SCANCODE_RIGHT;

	mPadConfig[KEY_A]     = SDL_CONTROLLER_BUTTON_A;
	mPadConfig[KEY_B]     = SDL_CONTROLLER_BUTTON_B;
	mPadConfig[KEY_X]     = SDL_CONTROLLER_BUTTON_X;
	mPadConfig[KEY_Y]     = SDL_CONTROLLER_BUTTON_Y;
	mPadConfig[KEY_L]     = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	mPadConfig[KEY_R]     = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	mPadConfig[KEY_UP]    = SDL_CONTROLLER_BUTTON_DPAD_UP;
	mPadConfig[KEY_START] = SDL_CONTROLLER_BUTTON_START;
	mPadConfig[KEY_DOWN]  = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	mPadConfig[KEY_LEFT]  = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	mPadConfig[KEY_RIGHT] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
}

void Input::Update()
{
	// ���݂̃L�[��1�O�̃L�[�̏�Ԃ����ւ���  0���P�ɁA1��0��
	mNowKey ^= 1;
	mPrevKey = mNowKey ^ 1;

	//�L�[��Ԏ擾
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memcpy(mKeyState[mNowKey], keys, sizeof(char) * 256);

	for (int i = 0; i < 256; i++)
	{
		char nowInput  = mKeyState[mNowKey][i];
		char prevInput = mKeyState[mPrevKey][i];

		if (nowInput) //���݉�����Ă���
		{
			//�O�̃t���[����������Ă����H
			if (prevInput)
			{
				mKey[i] = KEY_STATE_PRESSED; // ��������ςȂ�
			}
			else {
				mKey[i] = KEY_STATE_PUSHDOWN;// �������ꂽ
			}

		}
		else // ���݃L�[�͉�����Ă��Ȃ�
		{
			// �O�̃t���[���ŉ�����Ă����H
			if (prevInput)
			{
				mKey[i] = KEY_STATE_PULLUP;
			}
			else {
				mKey[i] = KEY_STATE_OFF;
			}
		}
	}
	// �R���g���[���A�b�v�f�[�g
	mController->Update();
	mLStick = mController->GetLAxisVec();
	mRStick = mController->GetRAxisVec();

	// �R���g���[���̃X�e�B�b�N���|��Ă��邩�H
	float stickMoveLen2;
	stickMoveLen2 = Vector2::Dot(mLStick, mLStick);
	mIsLStickMove = stickTrigThreshold < stickMoveLen2;

	stickMoveLen2 = Vector2::Dot(mRStick, mRStick);
	mIsRStickMove = stickTrigThreshold < stickMoveLen2;

	// �L�[�R���t�B�O���
	for (unsigned int i = 0; i < KEY_TYPE_ENUM_MAX; i++)
	{
		mKeyConfigedKeyState[i] = mKey[mKeyConfig[i]];
	}

	// �p�b�h�̏�Ԃ𓾂�B�L�[�{�[�h����ɓ��͂���Ă����疳��
	for (unsigned int i = 0; i < KEY_TYPE_ENUM_MAX; i++)
	{
		if (mKeyConfigedKeyState[i] == KEY_STATE_OFF)
		{
			mKeyConfigedKeyState[i] = mController->GetInput(mPadConfig[i]);
		}
	}
}

char Input::GetInput(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key];
}

KEY_STATE_ENUM Input::GetInput(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key];
}

bool Input::IsKeyPressed(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PRESSED;
}

bool Input::IsKeyPullup(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PULLUP;
}

bool Input::IsKeyPushdown(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PUSHDOWN;
}

bool Input::IsKeyOff(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_OFF;
}

bool Input::IsKeyPressed(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PRESSED;
}

bool Input::IsKeyPullup(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PULLUP;
}

bool Input::IsKeyPushdown(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PUSHDOWN;
}

bool Input::IsKeyOff(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_OFF;
}

InputController* Input::GetControllerInstance()
{
	return mController;
}

bool Input::ControllerInit()
{
	// �R���g���[��������
	mController = new InputController;
	return mController->Initialize();
}
