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

	KEY_TYPE_ENUM_MAX,    // キー種類の総数
}KEY_CONFIG;

typedef enum KEY_STATE_ENUM
{
	KEY_STATE_OFF = 0,    // 押されていない
	KEY_STATE_PUSHDOWN,   // 押下
	KEY_STATE_PRESSED,    // 押され続けている
	KEY_STATE_PULLUP      // 押離し
}KEY_STATE_ENUM;

// 入力クラス
class Input
{
public:

	static Input& InputInstance()                  // インスタンス
	{
		static Input InputInstance;
		return InputInstance;
	}
	
	~Input() {};

	void Update();                                 // 入力状態の更新
	char GetInput(Uint8 SDL_SCANCODE_key);         // あるキーの入力状態の取得
	KEY_STATE_ENUM GetInput(KEY_CONFIG key);       // キーコンフィグされたキーの状態を取得する

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);     // あるキーが押され続けられている状態か？
	bool IsKeyPullup(Uint8 SDL_SCANCODE_key);      // あるキーが押し上げられたか？
	bool IsKeyPushdown(Uint8 SDL_SCANCODE_key);    // あるキーは押され続けられているか？
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);         // あるキーは押されていないか？
	bool IsKeyPressed(KEY_CONFIG key);             // あるキーが押され続けられている状態か？
	bool IsKeyPullup(KEY_CONFIG key);              // あるキーが押し上げられたか？
	bool IsKeyPushdown(KEY_CONFIG key);            // あるキーは押され続けられているか？
	bool IsKeyOff(KEY_CONFIG key);                 // あるキーは押されていないか？

	InputController* GetControllerInstance();      // コントローラインスタンス
	bool ControllerInit();

	Vector2& GetLStick() { return mLStick; }       // Lスティックの移動量
	Vector2& GetRStick() { return mRStick; }       // Rスティックの移動量

	bool IsLStickMove() { return mIsLStickMove; }  // Lスティックが動かされたか
	bool IsRStickMove() { return mIsRStickMove; }  // Rスティックが動かされたか

private:
	Input(); // シングルトン

	KEY_STATE_ENUM mKey[256];                      // 全キー状態取得用
	char mKeyState[2][256];                        // キー状態 [現在/過去フレーム][全キーコード]

	SDL_GameControllerButton mPadConfig[KEY_TYPE_ENUM_MAX];  // パッドコンフィグ
	KEY_STATE_ENUM mKeyConfigedKeyState[KEY_TYPE_ENUM_MAX];  // キーコンフィグ後のキー
	SDL_Scancode   mKeyConfig[KEY_TYPE_ENUM_MAX];            // キーコンフィグ
	
	int  mNowKey, mPrevKey;                        // 現フレーム、過去フレームのインデックス
	InputController* mController ;                 // コントローラクラス

	Vector2 mLStick;                               // アナログコントローラーのLスティック
	Vector2 mRStick;                               // アナログコントローラーのRスティック
	
	bool mIsLStickMove;
	bool mIsRStickMove;

};

#define INPUT_INSTANCE Input::InputInstance()
