//---------------------------------------
//	エネミー関連の関数の実装
//  (宣言はヘッダーが好ましい)
//---------------------------------------
#include <math.h>
#include "DxLib.h"
#include "System.h"
#include "enemy.h"
#include "Map.h"
#include "Block.h"
#include "attack.h"
#include "Player.h"

// 変数 ---------------------------------
Object attack[TAMA_MAX];
int attackCount;

// 攻撃の初期化
void InitializeAttack()
{
	for (int i = 0; i < TAMA_MAX; i++)
	{
		/*flyenemy[_index].Enemy[ENEMY_MAX];*/
		attack[i].posX = 0;
		attack[i].posY = 0;

		attack[i].direction.x = 0.0f;
		attack[i].direction.y = 0.0f;

		attack[i].sizeX = player.sizeX / 2;
		attack[i].sizeY = player.sizeY / 2;

		attack[i].moveSpeed = 2.5f;
		attack[i].velocityX = 0.0f;
		attack[i].drawPosX = 0;
		attack[i].drawPosY = 0;


		attack[i].image = LoadGraph("Image/tama.png");

		attack[i].ShotFlag = false;
	}

	attackCount = 0;
}


// 攻撃更新
void UpdateAttack()
{
	bool AttackBFlag = false;  //発射されたかどうか

	
	//Z クリックを押されたら弾を出す 
	
	if (keyState[KEY_INPUT_Z] == 1 &&
		preKeyState[KEY_INPUT_Z] == 0 )
	{
		//発射ボタンがfalseだったら弾を発射
		if (AttackBFlag == false)
		{
			//画面上に出ていない弾があるか、調べる
			for (int i = 0; i < TAMA_MAX; i++)
			{
				//出ていない弾がある場合はその球を画面に出す
				if (attack[i].ShotFlag == false )
				{
					attack[i].posX = player.posX;
					attack[i].posY = player.posY;

					attack[i].ShotFlag = true;  //弾iは存在しているのでショットフラグをtrueにしてあげる
					if (player.InversionFlag)
					{
						attack[i].velocityX = attack[i].moveSpeed * -1.0f;
					}
					else if (!player.InversionFlag)
					{
						attack[i].velocityX = attack[i].moveSpeed;
					}
					break;  //一つ弾を出したのでループを抜ける
				}
			}
			//弾が発射されたのでAttackBFlagをtrueにしてあげる
			AttackBFlag = true;
		}
		else
		{
			AttackBFlag = false;
		}

		//GetMouseInputLog(MOUSE_INPUT_LEFT ,player.posX , int* ClickY, int LogDelete = TRUE);
	}
	



	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true)
		{
			attack[i].posX += attack[i].velocityX;
		}
	}

	for (int i = 0; i < TAMA_MAX; i++)  
	{
		if (attack[i].velocityX > 0.0f)
		{
			if (attack[i].posX - (attack[i].sizeX / 2.0f) - scrollX > 500 )
			{ 
				attack[i].ShotFlag = false; //弾がある程度離れたらショットフラグをfalseにしてあげる

			}
		}
		else if (attack[i].velocityX < 0.0f)
		{
			if (attack[i].posX < player.posX - 300.0f)
			{
				attack[i].ShotFlag = false; //弾がある程度離れたらショットフラグをfalseにしてあげる

			}
		}
	};

}


// 攻撃描画
void DrawAttack()
{
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true && player.posX <= 6155)
		{
			attack[i].drawPosX = attack[i].posX - (attack[i].sizeX / 2.0f) - scrollX;
			attack[i].drawPosY = attack[i].posY - (attack[i].sizeY / 2);

			DrawGraph(attack[i].drawPosX, attack[i].drawPosY, attack[i].image, true);

		}
	}
	
}

void PushBackAttack()
{
	// 押し戻しがすべて完了するまでループ
	bool isHit = false;

	for (int i = 0; i < TAMA_MAX; i++)
	{
		do
		{
			// とりあえず繰り返しフラグOFF
			isHit = false;

			// 一番近いブロックを探す
			int nearIndex = -1; // 一番近いブロックの番号
			float nearLength = 10000000000; // 一番近い距離

			// 全てのブロックから近いものを探す
			for (int j = 0; j < blockCount; j++)
			{
				// ブロックとプレイヤーの距離計算
				float x = attack[i].posX - block[j].posX;
				float y = attack[i].posY - block[j].posY;
				float length = sqrt(x * x + y * y);

				// 今までの一番近いやつよりも近いブロックがでてきたら情報更新
				if (nearLength > length)
				{
					nearIndex = j;
					nearLength = length;
				}
			}

			// 一番近いやつとの押し戻し
			if (IsHit(attack[i], block[nearIndex]))
			{
				// 当たっていたら繰り返しフラグON
				isHit = true;

				// 当たったブロックをどちらに押し戻すか、横と縦の距離から判別
				float lenX = fabs(attack[i].posX - block[nearIndex].posX);
				float lenY = fabs(attack[i].posY - block[nearIndex].posY);

				if (lenX > lenY) // 左右押し戻し
				{
					// 右か左か?当たった者同士のX座標で比較 
					if (attack[i].posX < block[nearIndex].posX)
					{
						// エネミー左押し戻し
						float width = (attack[i].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
						attack[i].posX = block[nearIndex].posX - width;

						attack[i].moveSpeed *= -1.0f;

					}
					else
					{
						// エネミー右押し戻し
						float width = (attack[i].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
						attack[i].posX = block[nearIndex].posX + width;

						attack[i].moveSpeed *= -1.0f;
					}
				}
				else // 上下押し戻し
				{
					if (attack[i].posY < block[nearIndex].posY)
					{
						// プレイヤー上押し戻し
						float width = (attack[i].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
						attack[i].posY = block[nearIndex].posY - width;

						// 上押し戻しでの速度リセットは降りてきているときだけ機能させる
						if (attack[i].velocityY > 0)attack[i].velocityY = 0;
					}
					else
					{
						// プレイヤー下押し戻し
						float width = (attack[i].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
						attack[i].posY = block[nearIndex].posY + width;


						if (attack[i].velocityY < 0)attack[i].velocityY = 0;
					}
				}

			}

			// 当たっている限り繰り返す
		} while (isHit == true);

		break;
	}
}
