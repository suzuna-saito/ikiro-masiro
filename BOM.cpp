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

// 変数 ---------------------------------
Object bom;

// ボムの初期化
void Initializebom()
{
	bom.posX = bom.posX;
	bom.posY = bom.posY;

	bom.sizeX = PLAYER_SIZE - 5;
	bom.sizeY = PLAYER_SIZE;

	bom.image = LoadGraph("Image/bom.png");

	bom.BomFlag = false;
	bom.Dmg_count = 0;

	bom.bom[BOM_MAX] = 0;
}



//　ボム更新
void Updatebom()
{
	//スペースキーでボムフラグをオンにする
	if (keyState[KEY_INPUT_SPACE] == 1 && bom.inputFlag == false)
	{
		bom.BomFlag = true;
	}

	// 重力
	bom.velocityY += GRAVITY_ACCEL;

	bom.posX += bom.velocityX * bom.direction.x;
	bom.posY += bom.velocityY;

}
//ボムの当たり判定がしたい
void CheckHitbom()
{
	IsHit(bom.posX, bom.posY,
		bom.sizeX, bom.sizeY,
		enemy.posX, enemy.posY,
		enemy.sizeX, enemy.sizeY);
	{

		if ((bom.posX + (bom.sizeX / 2.0f) > enemy.posX - (enemy.sizeX / 2.0f)) &&
			(bom.posX - (bom.sizeX / 2.0f) < enemy.posX + (enemy.sizeX / 2.0f)) &&
			(bom.posY + (bom.sizeY / 2.0f) > enemy.posY - (enemy.sizeY / 2.0f)) &&
			(bom.posY - (bom.sizeY / 2.0f) < enemy.posY + (enemy.sizeY / 2.0f)))
		{
			{
				bom.Dmg_Flag = true;

				enemy.moveSpeed *= -1;
			}
		}

		return;
	}

	/*{
		bom.Dmg_Flag = true;

		enemy.moveSpeed *= -1;
	}*/

}


// プレイヤー描画
void DrawPlayer()
{
	float drawPosX = bom.posX - (bom.sizeX / 2.0f) - scrollX;
	float drawPosY = bom.posY - (bom.sizeY / 2.0f);

	//スペースキーで爆弾表示

	if (keyState[KEY_INPUT_SPACE] == 1)
	{
		bom.BomFlag = true;
	}

	if (bom.BomFlag == true)
	{
		//画面上に出ていないボムがあるか、ボムの数だけ繰り返して調べる
		for (int i = 0; i < BOM_MAX; i++)
		{
			bom.bom[i];


		}

		//		{
		//			bom.timer = 100;

		//			ボムが画面上に出ていない場合はそのボムを画面に出す
		//			if (bom.bom[i] == 0)
		//			{
		//				if (bom.timer >= 0)
		//				{
		//					DrawGraph(drawPosX, drawPosY, bom.image2, TRUE);
		//				}

		//				ボムiは現時点をもって存在するので、存在状態を保持する変数に１を代入する
		//				bom.bom[i] = 1;

		//				bom.timer--;

		//				if (bom.timer <= 0)
		//				{
		//					一つ弾を出したので弾を出すループから抜けます
		//					break;
		//				}
		//			}
		//		}

		//	}
		//}*/
	}


	DrawGraph(drawPosX, drawPosY, bom.image, true);

	if (bom.Dmg_Flag == true)
	{
		DrawGraph(drawPosX, drawPosY, bom.dmg_image, true);

		bom.Dmg_count++;
		if (bom.Dmg_count >= 30)
		{
			DrawGraph(drawPosX, drawPosY, bom.image, true);
		}
	}
	bom.Dmg_Flag = false;
	bom.Dmg_count = 0;
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
			float x = bom.posX - block[i].posX;
			float y = bom.posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// 今までの一番近いやつよりも近いブロックがでてきたら情報更新
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// 一番近いやつとの押し戻し
		if (IsHit(bom, block[nearIndex]))
		{
			// 当たっていたら繰り返しフラグON
			isHit = true;

			// 当たったブロックをどちらに押し戻すか、横と縦の距離から判別
			float lenX = fabs(bom.posX - block[nearIndex].posX);
			float lenY = fabs(bom.posY - block[nearIndex].posY);

			if (lenX > lenY) // 左右押し戻し
			{
				// 右か左か?当たった者同士のX座標で比較 
				if (bom.posX < block[nearIndex].posX)
				{
					// プレイヤー左押し戻し
					float width = (bom.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					bom.posX = block[nearIndex].posX - width;
				}
				else
				{
					// プレイヤー右押し戻し
					float width = (bom.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					bom.posX = block[nearIndex].posX + width;
				}
			}
			else // 上下押し戻し
			{
				if (bom.posY < block[nearIndex].posY)
				{
					// プレイヤー上押し戻し
					float width = (bom.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					bom.posY = block[nearIndex].posY - width;

					// 上押し戻しでの速度リセットは降りてきているときだけ機能させる
					if (bom.velocityY > 0)bom.velocityY = 0;
				}
				else
				{
					// プレイヤー下押し戻し
					float width = (bom.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					bom.posY = block[nearIndex].posY + width;


					if (bom.velocityY < 0)bom.velocityY = 0;
				}
			}
		}

		// 当たっている限り繰り返す
	} while (isHit == true);


	// 接地しているかのチェック
	// フラグのリセット
	bom.isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = bom;
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// プレイヤーの足元とブロックの判定
		if (bool isHit = IsHit(obj, block[i]))
		{
			bom.isGround = true;
		}
	}

	//デバック用
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		bom.posY = 30;
		bom.velocityY = 0.2f;
	}

}