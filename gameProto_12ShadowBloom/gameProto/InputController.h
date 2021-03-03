#pragma once
#include "Game.h"
#include "Input.h"

class Input;

class InputController
{
public:

	InputController();

	bool Initialize(void); // ������
	void Update(void);     // �X�V
	void Release(void);    // �������

	//�R���g���[���ڑ��C�x���g�������C�x���g
	void ReceiveEvent(const SDL_Event& oEvent);

	/// �{�^���̏�Ԏ擾�֐�
	bool IsControllerButtonTriggered(SDL_GameControllerButton iButton) const;  // �������ꂽ�H
	bool IsControllerButtonPressed(SDL_GameControllerButton iButton) const;  // ������Ă���H
	bool IsControllerButtonReleased(SDL_GameControllerButton iButton) const; // �����ꂽ�H
	bool IsControllerAvailable() { return mpGameController != nullptr; }

	/// ���݂̎��̒l���擾
	float GetAxisValue(SDL_GameControllerAxis iAxis) const;
	Vector2& GetLAxisVec() { return mLAxis; }
	Vector2& GetRAxisVec() { return mRAxis; }

	enum KEY_STATE_ENUM GetInput(SDL_GameControllerButton key);

private:
	// �R���g���[�����
	SDL_GameController* mpGameController;
	int                 mWichController;

	// �R���g���[���̏�ԏ��
	Uint8 mButtonStates[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mButtonStatesPrev[SDL_CONTROLLER_BUTTON_MAX];
	KEY_STATE_ENUM mButtonFinalStates[SDL_CONTROLLER_BUTTON_MAX]; // �{�^���̍ŏI���

	float mAxisValues[SDL_CONTROLLER_AXIS_MAX];

	Vector2 mLAxis;
	Vector2 mRAxis;
};
