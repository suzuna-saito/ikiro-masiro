#include "InputController.h"
#include <limits.h>
#include <iostream>

//// XBox �p�b�h�̂������l
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

InputController::InputController()
	: mpGameController(nullptr)
	, mWichController(-1)
{
	// �{�^���Ǝ���0�ŏ�����
	memset(mButtonStates     , 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(mButtonStatesPrev , 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(mAxisValues       , 0, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
}

// ������
bool InputController::Initialize(void)
{
	// �p�b�h�̐ݒ�t�@�C�� gamecontrollerdb.txt �̓ǂݍ��݂Ɩ�肪�������̃`�F�b�N
	int iNumOfControllers = SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt");
	if (iNumOfControllers == -1) 
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
		return false;
	}

	// �R���g���[���J��
	mpGameController = SDL_GameControllerOpen(0);
	if (!mpGameController)
	{
		return false;
	}

	if (SDL_IsGameController(0))
	{
		std::cout << SDL_GameControllerMapping(mpGameController) << std::endl;
	}

	// �R���g���[���C�x���g�̖����i�����炩��t���[�����ɏ�Ԃ��擾���邽�߁j
	SDL_GameControllerEventState(SDL_IGNORE);
	return true;
}

// �R���g���[���X�V����
void InputController::Update(void)
{
	// �R���g���[���������ꍇ�� early exit����
	if (mpGameController == NULL) 
	{
		return;
	}

	// �O�̃t���[���̃R���g���[���̏�Ԃ��R�s�[����
	memcpy(&mButtonStatesPrev, &mButtonStates, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);

	// �R���g���[���̏�Ԃ��X�V����
	SDL_GameControllerUpdate();

	// ���݂̃R���g���[���̃{�^����Ԃ�ۑ�
	for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; ++b) 
	{
		mButtonStates[b] = SDL_GameControllerGetButton(mpGameController, (SDL_GameControllerButton)b);
	}

	// ���݂̃R���g���[���̎�����ۑ�
	for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; ++a) 
	{
		mAxisValues[a] = SDL_GameControllerGetAxis(mpGameController, (SDL_GameControllerAxis)a);
	}

	// LPAD���͂��x�N�g��������
	const float maxInput = 32767.0f;
	mLAxis.x = (float)mAxisValues[SDL_CONTROLLER_AXIS_LEFTX];
	mLAxis.y = (float)mAxisValues[SDL_CONTROLLER_AXIS_LEFTY];
	mLAxis.x = (fabs(mLAxis.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f :  mLAxis.x / maxInput;
	mLAxis.y = (fabs(mLAxis.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : mLAxis.y / maxInput;

	mRAxis.x = (float)mAxisValues[SDL_CONTROLLER_AXIS_RIGHTX];
	mRAxis.y = (float)mAxisValues[SDL_CONTROLLER_AXIS_RIGHTY];
	mRAxis.x = (fabs(mRAxis.x) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f :  mRAxis.x / maxInput;
	mRAxis.y = (fabs(mRAxis.y) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : mRAxis.y / maxInput;

	// �{�^�������߂���
	for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		Uint8 nowInput  = mButtonStates[i];
		Uint8 prevInput = mButtonStatesPrev[i];

		// ���݉�����Ă�����E�E�E
		if (nowInput)
		{
			//�ߋ��̃L�[��Ԃɂ���ĉ������u�Ԃ��A��������ςȂ����f
			if (prevInput)
			{
				mButtonFinalStates[i] = KEY_STATE_PRESSED;
			}
			else
			{
				mButtonFinalStates[i] = KEY_STATE_PUSHDOWN;
			}
		}
		else
		{
			// �ߋ��L�[���A���ܗ��������A������Ă��Ȃ���
			if (prevInput)
			{
				mButtonFinalStates[i] = KEY_STATE_PULLUP;
			}
			else
			{
				mButtonFinalStates[i] = KEY_STATE_OFF;
			}
		}
	}

}

void InputController::Release(void)
{
	if (!mpGameController)
	{
		SDL_GameControllerClose(mpGameController);
	}
	mpGameController = nullptr;
}

void InputController::ReceiveEvent(const SDL_Event& oEvent)
{
	switch (oEvent.type) 
	{
		// �R���g���[���ǉ��C�x���g
	case SDL_CONTROLLERDEVICEADDED:
		// �R���g���[�������t�����Ă��Ȃ������ꍇ
		if (mpGameController != NULL)
		{
			// �R���g���[���̃I�[�v��
			mWichController = oEvent.cdevice.which;
			mpGameController = SDL_GameControllerOpen(mWichController);
			// �O�Ɏ��t����ꂽ�R���g���[���̖���������邽�߃�������0�ɃZ�b�g�B
			memset(mButtonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(mButtonStatesPrev, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(mAxisValues, 0, sizeof(float)*SDL_CONTROLLER_AXIS_MAX);
		}
		break;

		// �R���g���[�����O���C�x���g
	case SDL_CONTROLLERDEVICEREMOVED:

		// ���݂̃R���g���[�������O���ꂩ�̃`�F�b�N
		if (mWichController == oEvent.cdevice.which) 
		{
			mWichController = -1;
			mpGameController = NULL;
		}
		break;
	}
}

bool InputController::IsControllerButtonTriggered(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 1 && mButtonStatesPrev[iButton] == 0);
}

bool InputController::IsControllerButtonPressed(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 1);
}

bool InputController::IsControllerButtonReleased(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 0 && mButtonStatesPrev[iButton] == 1);
}

float InputController::GetAxisValue(SDL_GameControllerAxis iAxis) const
{
	return mAxisValues[iAxis];
}

KEY_STATE_ENUM InputController::GetInput(SDL_GameControllerButton key)
{
	return mButtonFinalStates[key];
}

