//---------------------------------------
//	プレイヤー関連の関数の実装
//  (宣言はヘッダーが好ましい)
//---------------------------------------
#include <math.h>
#include "DxLib.h"
#include "System.h"
#include "Player.h"
#include "Map.h"
#include "Block.h"
#include "enemy.h"
#include "HItPoint.h"

// 変数 ---------------------------------
Object player;

// プレイヤーの初期化
void InitializePlayer()
{
	player.posX = 64;
	player.posY = 0;

	player.initPosX = 64;
	player.initPosY = 0;

	player.direction.x = 0.0f;
	player.direction.y = 0.0f;

	player.sizeX = PLAYER_SIZE -8  ;
	player.sizeY = PLAYER_SIZE;

	player.moveSpeed = 2.0f;

	player.image = LoadGraph("Image/player.png");
	player.DmgImage = LoadGraph("Image/playerDmg.png");

	player.isGround = false;
	player.inputFlag = false;
	player.acceleFlag = false;
	player.DrawFlag = true;
	player.Dmg_Flag = false;
	player.InversionFlag = false;
	//player.HitFlag = false;  //とりあえずｈｐ減るかどうか的なフラグ

	player.Dmg_count = 0;

	player.hpCount = 3;
	player.isJumpFlag = false;
	player.jumpPower = FIRST_JUMP_POWER;
	//jumpFrameCount = 0;
}

void SecondInitializeIsPlayer()
{
	player.posX = 64;
	player.posY = 0;
	player.hpCount = 3;
	player.Dmg_count = 0;
	//player.DrawFlag = true;

	player.velocityX = 0.0f;
	player.velocityY = 0.0f;

	scrollX = 0;
	player.jumpPower = FIRST_JUMP_POWER;

	//jumpFrameCount = 0;
}



// プレイヤー更新
void UpdatePlayer()
{
	//まず入力時にプレイヤーがどの方向に進むかを決める
	if (keyState[KEY_INPUT_RIGHT] == 1 && player.inputFlag == false)
	{
		player.direction.x = player.moveSpeed;
		player.inputFlag = true;
		player.InversionFlag = false;
		//player.velocityX = player.moveSpeed;
	}
	else if (keyState[KEY_INPUT_LEFT] == 1 && player.inputFlag == false)
	{
		player.direction.x = -player.moveSpeed;
		player.inputFlag = true;
		player.InversionFlag = true;
	}
	else
	{
		player.direction.x = 0.0f;
		player.inputFlag = false;
		//player.velocityX= 0;
	}

	if (player.inputFlag == true)
	{

		if (player.acceleFlag == true)
		{
			//ここでmovespeedの値をみて最大速度より大きかったら加速度をゼロにしスピードを最大速度に固定する
			if (player.velocityX > MAX_SPEED)
			{
				player.velocityX = MAX_SPEED;
			}
			else
			{
				player.accelePower += 0.05;
				player.velocityX = player.moveSpeed + player.accelePower;
			}
		}
		else
		{
			player.moveSpeed = 2.0f;
			player.velocityX =  player.moveSpeed;
		}
	}
	else if (player.inputFlag == false)
	{
		player.moveSpeed = 2.0f;
		player.velocityX = 0.0f;
	}

	//死んだらエンターで最初から
	if (keyState[KEY_INPUT_RETURN] == 1 && player.hpCount == 0)
	{

		SecondInitializeIsPlayer();

		for (int i = 0; i < enemyCount; i++)
		{
			SecondInitialize(i);
		}
		for (int i = 0; i < flyenemyCount; i++)
		{
			SecondInitializeIsFlyEnemy(i);
		}

		//scrollX = 0;

	}


	// 重力
	player.velocityY += GRAVITY_ACCEL;

	// ジャンプ
	// 上キーか、スペースキーを押したらジャンプ

	if (player.isGround == true)
	{
		if (keyState[KEY_INPUT_X] == 1 &&
			preKeyState[KEY_INPUT_X] == 0)
		{
			player.isJumpFlag = true;
			player.velocityY = -player.jumpPower;
		}
	}

	if (player.isJumpFlag == true)
	{
		++player.jumpCount;
		if (player.jumpCount <= 30)
		{
			player.velocityY -= 0.2f;
		}
		else
		{
			player.isJumpFlag = false;
			player.velocityY = 0.0f;
			player.jumpCount = 0;
		}
	}

	player.posX += player.velocityX * player.direction.x;
	player.posY += player.velocityY;

	// 移動制限
	float limitL = 0 + (player.sizeX / 2.0f);
	if (limitL > player.posX)player.posX = limitL;    //これなんだっけ…？？

	if (left == true)
	{
		// 必要があれば、プレイヤーの移動した量だけ画面スクロール
		if (player.posX > SCROLL_R + scrollX)
		{
			scrollX += player.moveSpeed;
		}

		// 画面の左の方まで来るとスクロール
		// ただし、左にすすんでいるときだけにする
		if (player.posX < SCROLL_L + scrollX &&
			player.velocityX < 0)
		{
			scrollX += player.velocityX;
		}

		// スクロール値が0より小さくならないように 
		if (scrollX < 0) scrollX = 0;

		//スクロールを止めるタイミング（座標）
		if (player.posX >= 5920 )
		{
			left = false;
		}

	}

	
}

//プレイヤーの当たり判定がしたい
void CheckHitPlayer(int _index)
{
	if (player.Dmg_Flag == false && enemy[_index].DrawFlag == true)
	{
		IsHit(player.posX, player.posY,
			player.sizeX, player.sizeY,
			enemy[_index].posX, enemy[_index].posY,
			enemy[_index].sizeX, enemy[_index].sizeY);
		{

			if ((player.posX + (player.sizeX / 2.0f) > enemy[_index].posX - (enemy[_index].sizeX / 2.0f)) &&   //エネミーの当たり判定
				(player.posX - (player.sizeX / 2.0f) < enemy[_index].posX + (enemy[_index].sizeX / 2.0f)) &&
				(player.posY + (player.sizeY / 2.0f) > enemy[_index].posY - (enemy[_index].sizeY / 4.0f)) &&
				(player.posY - (player.sizeY / 2.0f) < enemy[_index].posY + (enemy[_index].sizeY / 2.0f)))
			{
				player.Dmg_Flag = true;

				enemy[_index].moveSpeed *= -1;

				--player.hpCount;
			}

			return;
		}

	}

	/*{
		player.Dmg_Flag = true;
	
		enemy.moveSpeed *= -1;
	}*/

}

//プレイヤーの当たり判定がしたい
void CheckHitFlyEnemyToPlayer(int _index)
{
	if (player.Dmg_Flag == false && flyenemy[_index].DrawFlag == true)
	{
		IsHit(player.posX, player.posY,
			player.sizeX, player.sizeY,
			flyenemy[_index].posX, flyenemy[_index].posY,
			flyenemy[_index].sizeX, flyenemy[_index].sizeY);
		{

			if ((player.posX + (player.sizeX / 2.0f) > flyenemy[_index].posX - (flyenemy[_index].sizeX / 2.0f)) &&   //フライエネミーの当たり判定
				(player.posX - (player.sizeX / 2.0f) < flyenemy[_index].posX + (flyenemy[_index].sizeX / 2.0f)) &&
				(player.posY + (player.sizeY / 2.0f) > flyenemy[_index].posY - (flyenemy[_index].sizeY / 4.0f)) &&
				(player.posY - (player.sizeY / 2.0f) < flyenemy[_index].posY + (flyenemy[_index].sizeY / 2.0f)))
			{

				player.Dmg_Flag = true;

				flyenemy[_index].moveSpeed *= -1;
				--player.hpCount;
			}

			return;
		}

	}

}


// プレイヤー描画
void DrawPlayer()
{
	float drawPosX = player.posX - (player.sizeX / 2.0f) - scrollX;
	float drawPosY = player.posY - (player.sizeY / 2.0f);
	if (player.DrawFlag == true)
	{
		if (player.Dmg_Flag == true)
		{
			if (player.InversionFlag == true)
			{
				DrawTurnGraph(drawPosX - 8, drawPosY, player.DmgImage, true);

				player.Dmg_count++;
				if (player.Dmg_count >= 60)
				{
					DrawTurnGraph(drawPosX - 8, drawPosY, player.image, true);
					player.Dmg_Flag = false;
					player.Dmg_count = 0;

				}
			}
			else
			{
				DrawGraph(drawPosX, drawPosY, player.DmgImage, true);

				player.Dmg_count++;
				if (player.Dmg_count >= 60)
				{
					DrawGraph(drawPosX, drawPosY, player.image, true);
					player.Dmg_Flag = false;
					player.Dmg_count = 0;

				}
			}
		}
		else if (player.InversionFlag == true)
		{
			DrawTurnGraph(drawPosX-8, drawPosY, player.image, true);
		}
		else
		{
			DrawGraph(drawPosX, drawPosY, player.image, true);
		}

		DrawHP(player.hpCount);

	}

		/*DrawFormatString()*/
}



// プレイヤーの押し戻し
void PushBackPlayer()
{
	// 押し戻しがすべて完了するまでループ
	bool isHit = false;

	do
	{
		// とりあえず繰り返しフラグOFF
		isHit = false;

		// 一番近いブロックを探す
		int nearIndex = -1; // 一番近いブロックの番号
		float nearLength = 10000000000; // 一番近い距離

		// 全てのブロックから近いものを探す
		for (int i = 0; i < blockCount; i++)
		{
			// ブロックとプレイヤーの距離計算
			float x = player.posX - block[i].posX;
			float y = player.posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// 今までの一番近いやつよりも近いブロックがでてきたら情報更新
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// 一番近いやつとの押し戻し
		if (IsHit(player, block[nearIndex]))
		{
			// 当たっていたら繰り返しフラグON
			isHit = true;

			// 当たったブロックをどちらに押し戻すか、横と縦の距離から判別
			float lenX = fabs(player.posX - block[nearIndex].posX);
			float lenY = fabs(player.posY - block[nearIndex].posY);

			if (lenX > lenY) // 左右押し戻し
			{
				// 右か左か?当たった者同士のX座標で比較 
				if (player.posX < block[nearIndex].posX)
				{
					// プレイヤー左押し戻し
					float width = (player.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					player.posX = block[nearIndex].posX - width;
				}
				else
				{
					// プレイヤー右押し戻し
					float width = (player.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					player.posX = block[nearIndex].posX + width;
				}
			}
			else // 上下押し戻し
			{
				if (player.posY < block[nearIndex].posY)
				{
					// プレイヤー上押し戻し
					float width = (player.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					player.posY = block[nearIndex].posY - width;

					// 上押し戻しでの速度リセットは降りてきているときだけ機能させる
					if (player.velocityY > 0)player.velocityY = 0;
				}
				else
				{
					// プレイヤー下押し戻し
					float width = (player.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					player.posY = block[nearIndex].posY + width;


					if (player.velocityY < 0)player.velocityY = 0;
				}
			}
		}

		// 当たっている限り繰り返す
	} while (isHit == true);


	// 接地しているかのチェック
	// フラグのリセット
	player.isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = player;
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// プレイヤーの足元とブロックの判定
		if (bool isHit = IsHit(obj, block[i]))
		{
			player.isGround = true;
		}
	}

	//デバック用
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		player.posY = 30;
		player.velocityY = 0.2f;
	}

}