#pragma once
#include "Game.h"
#include "InputController.h"

class InputController;

typedef enum KEY_CONFIG
{
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_A,
	KEY_B,
	KEY_X,
	KEY_Y,
	KEY_L,
	KEY_R,
	KEY_START,
	KEY_SELECT,

	KEY_TYPE_ENUM_MAX,    // �L�[��ނ̑���
}KEY_CONFIG;

typedef enum KEY_STATE_ENUM
{
	KEY_STATE_OFF = 0,    // ������Ă��Ȃ�
	KEY_STATE_PUSHDOWN,   // ����
	KEY_STATE_PRESSED,    // �����ꑱ���Ă���
	KEY_STATE_PULLUP      // ������
}KEY_STATE_ENUM;

// ���̓N���X
class Input
{
public:

	static Input& InputInstance()                  // �C���X�^���X
	{
		static Input InputInstance;
		return InputInstance;
	}
	
	~Input() {};

	void Update();                                 // ���͏�Ԃ̍X�V
	char GetInput(Uint8 SDL_SCANCODE_key);         // ����L�[�̓��͏�Ԃ̎擾
	KEY_STATE_ENUM GetInput(KEY_CONFIG key);       // �L�[�R���t�B�O���ꂽ�L�[�̏�Ԃ��擾����

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);     // ����L�[�������ꑱ�����Ă����Ԃ��H
	bool IsKeyPullup(Uint8 SDL_SCANCODE_key);      // ����L�[�������グ��ꂽ���H
	bool IsKeyPushdown(Uint8 SDL_SCANCODE_key);    // ����L�[�͉����ꑱ�����Ă��邩�H
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);         // ����L�[�͉�����Ă��Ȃ����H
	bool IsKeyPressed(KEY_CONFIG key);             // ����L�[�������ꑱ�����Ă����Ԃ��H
	bool IsKeyPullup(KEY_CONFIG key);              // ����L�[�������グ��ꂽ���H
	bool IsKeyPushdown(KEY_CONFIG key);            // ����L�[�͉����ꑱ�����Ă��邩�H
	bool IsKeyOff(KEY_CONFIG key);                 // ����L�[�͉�����Ă��Ȃ����H

	InputController* GetControllerInstance();      // �R���g���[���C���X�^���X
	bool ControllerInit();

	Vector2& GetLStick() { return mLStick; }       // L�X�e�B�b�N�̈ړ���
	Vector2& GetRStick() { return mRStick; }       // R�X�e�B�b�N�̈ړ���

	bool IsLStickMove() { return mIsLStickMove; }  // L�X�e�B�b�N���������ꂽ��
	bool IsRStickMove() { return mIsRStickMove; }  // R�X�e�B�b�N���������ꂽ��

private:
	Input(); // �V���O���g��

	KEY_STATE_ENUM mKey[256];                      // �S�L�[��Ԏ擾�p
	char mKeyState[2][256];                        // �L�[��� [����/�ߋ��t���[��][�S�L�[�R�[�h]

	SDL_GameControllerButton mPadConfig[KEY_TYPE_ENUM_MAX];  // �p�b�h�R���t�B�O
	KEY_STATE_ENUM mKeyConfigedKeyState[KEY_TYPE_ENUM_MAX];  // �L�[�R���t�B�O��̃L�[
	SDL_Scancode   mKeyConfig[KEY_TYPE_ENUM_MAX];            // �L�[�R���t�B�O
	
	int  mNowKey, mPrevKey;                        // ���t���[���A�ߋ��t���[���̃C���f�b�N�X
	InputController* mController ;                 // �R���g���[���N���X

	Vector2 mLStick;                               // �A�i���O�R���g���[���[��L�X�e�B�b�N
	Vector2 mRStick;                               // �A�i���O�R���g���[���[��R�X�e�B�b�N
	
	bool mIsLStickMove;
	bool mIsRStickMove;

};

#define INPUT_INSTANCE Input::InputInstance()
