#include "DxLib.h"
#include "System.h"
#include "Player.h"
#include "Map.h"
#include "enemy.h"
#include "attack.h"
#include "HItPoint.h"
#include "Grandmother.h"
#include "Fireworks.h"
// 変数 ---------------------------------
char keyState[256];		// 現在のキー入力情報
char preKeyState[256];	// １フレーム前のキー入力情報

typedef enum GameRule
{
	TITLE,
	PLAY,
	OVER,
	CLEAR,
	
}GameRule;

GameRule gameRule = TITLE;

bool secondLoopFlag = false;
bool clearFlag = false;
int clearCount = 0;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面をウインドウモードにする.
	ChangeWindowMode(TRUE);

	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	// ゲームの初期化
	Initialize();

	// メインループ
	ActionMain();

	// ＤＸライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

// ゲームの初期化
void Initialize()
{
	// Mapの初期化
	InitializeMap();

	InitializeSave();

	InitializePlayer();

	InitializeGrandmother();

	InitializeAttack();

	InitializeHP();

	InitFireworks();
	//InitializeEnemy();

	LoadImg();

}

//　アクションゲームのメインループ
void ActionMain()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (gameRule == TITLE)
		{
			// 画面のクリア
			ClearDrawScreen();

			game_title();

			if (secondLoopFlag == true)
			{

				SecondInitializeIsPlayer();

				left = true;

				for (int i = 0; i < enemyCount; i++)
				{
					SecondInitialize(i);
				}
				for (int i = 0; i < flyenemyCount; i++)
				{
					SecondInitializeIsFlyEnemy(i);
				}

				SecondInitFireworks();
				//scrollX = 0;
				secondLoopFlag = false;
			}


			// 画面の更新
			ScreenFlip();

			//Update();

			if (CheckHitKey(KEY_INPUT_RETURN))//enterでゲームスタート
			{
				gameRule = PLAY;
				//UpdatePlayer();
				//SecondInitializeIsPlayer();
			}

		}

		if (gameRule == PLAY)   //プレイ
		{
			// 更新
			Update();

			// 画面のクリア
			ClearDrawScreen();

			if (secondLoopFlag == true && SaveFlag == true)  //セーブまで行った場合
			{
				player.posX = 3588;
				player.posY = 0;

				player.hpCount = 3;

				scrollX = 3268;

				left = true;

				for (int i = 0; i < enemyCount; i++)
				{
					SecondInitialize(i);
				}
				for (int i = 0; i < flyenemyCount; i++)
				{
					SecondInitializeIsFlyEnemy(i);
				}

				SecondInitializeIsGrandmother();
				SecondInitFireworks();
				//scrollX = 0;
				secondLoopFlag = false;
			}
			else if (secondLoopFlag == true)
			{
				SecondInitializeIsPlayer();
				SecondInitializeIsGrandmother();

				left = true;

				for (int i = 0; i < enemyCount; i++)
				{
					SecondInitialize(i);
				}
				for (int i = 0; i < flyenemyCount; i++)
				{
					SecondInitializeIsFlyEnemy(i);
				}

				SecondInitFireworks();

				//scrollX = 0;
				secondLoopFlag = false;
			}


			if ((grandmother.hpCount >= 1 && player.posX >= 6156)) //クリア 
			{
				//player.DrawFlag = false;
				player.moveSpeed = 0.0f;  //移動できないようにする

				player.velocityY = 0.0f;  //飛べないようにする



				clearFlag = true;

				if (clearFlag == true)
				{
					++clearCount;
					UpdateFireworks();  //花火

					if (clearCount >= 210)
					{
						gameRule = CLEAR;
					}
				}
			}

			// 描画
			Draw();

			// 画面の更新
			ScreenFlip();
		}


		if (player.hpCount == 0 || player.posY >= 600 || (grandmother.hpCount <= 0 && player.posX >= 6156))  //ゲームオーバー  一通り終わり次第穴に落ちても死ぬようにする
		{
			gameRule = OVER;
			// 画面のクリア
			ClearDrawScreen();

			game_over();

			// 画面の更新
			ScreenFlip();


			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				gameRule = PLAY;
				secondLoopFlag = true;
			}
		}

		if (gameRule == CLEAR)
		{
			clearCount = 0;

			// 画面のクリア
			ClearDrawScreen();

			game_clear();

			SaveFlag = false;

			//UpdatePlayer();

			// 画面の更新
			ScreenFlip();


			//if (CheckHitKey(KEY_INPUT_RETURN))//enterでゲームスタート
			if (CheckHitKey(KEY_INPUT_V))  // v でタイトル
			{
				gameRule = TITLE;
				secondLoopFlag = true;

			}

		}
	}
}

// 更新
void Update()
{
	// 入力情報の更新

	UpdateInput();

	// 背景
	UpdateBg();

	// マップ
	UpdateMap();

	// おばあちゃん
	UpdateGrandmother();

	// プレイヤー
	UpdatePlayer();

	for (int i = 0; i < flyenemyCount; i++)
	{
		PushBackFlyEnemy(i);
	}
	for (int i = 0; i < enemyCount; i++)
	{
		// エネミー
		UpdateEnemy(i);
		 
	}
	for (int i = 0; i < flyenemyCount; i++)
	{
		UpdateFlayEnemy(i);
	}
	
	for (int i = 0; i < enemyCount; i++)
	{
		//プレイヤーの当たり判定
		CheckHitPlayer(i);

		//攻撃との当たり判定
		HitAttackToEnemy(i);

		PushBackEnemy(i);
	}

	for (int i = 0; i < flyenemyCount; i++)
	{
		//プレイヤーの当たり判定
		CheckHitFlyEnemyToPlayer(i);

		//攻撃との当たり判定
		HitAttackToFlyEnemy(i);

		//PushBackEnemy(i);
	}

	UpdateAttack();

	for (int i = 0; i < blockCount; i++)
	{
		//攻撃とブロックの当たり判定
 		HitAttackToBlock(i);
	}
	
	CheckHitGrandmother();

	// 全ての更新が終わった後に押し戻し
	PushBackPlayer();
}

// 入力情報の更新
void UpdateInput()
{
	// 更新する前に１フレーム前のキー情報を残しておく
	memcpy(preKeyState, keyState, sizeof(keyState));

	// 最新のキー情報に更新
	GetHitKeyStateAll(keyState);

	/*GetMouseInputLog(Mouse & MOUSE_INPUT_LEFT  ClickX, int* ClickY, int LogDelete = TRUE)*/

}


// 描画
void Draw()
{
	// 背景
	DrawBg();

	// マップ
	DrawMap();

	// おばあちゃん
	DrawGrandmother();

	// プレイヤー
	DrawPlayer();

	//攻撃
	DrawAttack();

	DrawFireworks();

}
