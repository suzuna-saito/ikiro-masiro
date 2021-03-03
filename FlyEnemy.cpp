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

// 変数 ---------------------------------
Object flyenemy[50];

// フライエネミーの初期化
void InitializeFlayEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY)
{
	flyenemy[_index].posX = _x;
	flyenemy[_index].posY = _y;
	flyenemy[_index].initPosX = _x;
	flyenemy[_index].initPosY = _y;

	/*flyenemy[_index].Enemy[ENEMY_MAX];*/

	flyenemy[_index].direction.x = 0.0f;
	flyenemy[_index].direction.y = 0.0f;

	flyenemy[_index].sizeX = _sizeX-5;
	flyenemy[_index].sizeY = _sizeY;

	flyenemy[_index].DrawFlag = true;

	// 配列ナンバーを見て最初左右どちらの方向に進むか決める
	if (_index % 2 == 0)
	{
		flyenemy[_index].moveSpeed = -1.0f;
	}
	else
	{
		flyenemy[_index].moveSpeed = 1.0f;
	}

	flyenemy[_index].image = _image;
}

void SecondInitializeIsFlyEnemy(int _index)
{
	flyenemy[_index].posX = flyenemy[_index].initPosX;
	flyenemy[_index].posY = flyenemy[_index].initPosY;
	flyenemy[_index].DrawFlag = true;
}

// フライエネミー更新
void UpdateFlayEnemy(int _index)
{

	//まず入力時にフライエネミーがどの方向に進むかを決める
	flyenemy[_index].velocityX = flyenemy[_index].moveSpeed;

	flyenemy[_index].posX += flyenemy[_index].velocityX;

	// 移動制限
	float limitL = 0 + (flyenemy[_index].sizeX / 2.0f);
	if (limitL > flyenemy[_index].posX)flyenemy[_index].posX = limitL;

}

//攻撃との当たり判定がしたい
void HitAttackToFlyEnemy(int _index)
{

	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true && flyenemy[_index].DrawFlag == true)
		{
			IsHit(flyenemy[_index].posX, flyenemy[_index].posY,
				flyenemy[_index].sizeX, flyenemy[_index].sizeY,
				attack[i].posX, attack[i].posY,
				attack[i].sizeX, attack[i].sizeY);
			{
				if ((flyenemy[_index].posX + (flyenemy[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //攻撃がフライエネミーに当たったか
					(flyenemy[_index].posX - (flyenemy[_index].sizeX / 2.0f) < attack[i].posX + (attack[i].sizeX / 2.0f)) &&
					(flyenemy[_index].posY + (flyenemy[_index].sizeY / 2.0f) > attack[i].posY - (attack[i].sizeY / 2.0f)) &&
					(flyenemy[_index].posY - (flyenemy[_index].sizeY / 2.0f) < attack[i].posY + (attack[i].sizeY / 2.0f)))
				{
					flyenemy[_index].DrawFlag = false;

					attack[i].ShotFlag = false;
				}
			}
		}

	}

}


// フライエネミー描画
void DrawFlayEnemy(int _index)
{
	if (flyenemy[_index].DrawFlag == true)
	{
		float drawPosX = flyenemy[_index].posX - (flyenemy[_index].sizeX / 2.0f) - scrollX;  //いらないんじゃね？
		float drawPosY = flyenemy[_index].posY - (flyenemy[_index].sizeY / 2.0f);

		DrawGraph(drawPosX, drawPosY, flyenemy[_index].image, true);
	}
}

void PushBackFlyEnemy(int _index)
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
			float x = flyenemy[_index].posX - block[i].posX;
			float y = flyenemy[_index].posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// 今までの一番近いやつよりも近いブロックがでてきたら情報更新
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// 一番近いやつとの押し戻し
		if (IsHit(flyenemy[_index], block[nearIndex]))
		{
			// 当たっていたら繰り返しフラグON
			isHit = true;

			// 当たったブロックをどちらに押し戻すか、横と縦の距離から判別
			float lenX = fabs(flyenemy[_index].posX - block[nearIndex].posX);
			float lenY = fabs(flyenemy[_index].posY - block[nearIndex].posY);

			if (lenX > lenY) // 左右押し戻し
			{
				// 右か左か?当たった者同士のX座標で比較 
				if (flyenemy[_index].posX < block[nearIndex].posX)
				{
					// エネミー左押し戻し
					float width = (flyenemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					flyenemy[_index].posX = block[nearIndex].posX - width;

					flyenemy[_index].moveSpeed *= -1.0f;

				}
				else
				{
					// エネミー右押し戻し
					float width = (flyenemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					flyenemy[_index].posX = block[nearIndex].posX + width;

					flyenemy[_index].moveSpeed *= -1.0f;
				}
			}
			else // 上下押し戻し
			{
				if (flyenemy[_index].posY < block[nearIndex].posY)
				{
					// プレイヤー上押し戻し
					float width = (flyenemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					flyenemy[_index].posY = block[nearIndex].posY - width;

					// 上押し戻しでの速度リセットは降りてきているときだけ機能させる
					if (flyenemy[_index].velocityY > 0)flyenemy[_index].velocityY = 0;
				}
				else
				{
					// プレイヤー下押し戻し
					float width = (flyenemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					flyenemy[_index].posY = block[nearIndex].posY + width;


					if (flyenemy[_index].velocityY < 0)flyenemy[_index].velocityY = 0;
				}
			}

		}

		// 当たっている限り繰り返す
	} while (isHit == true);

}
