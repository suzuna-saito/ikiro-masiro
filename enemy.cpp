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
Object enemy[50];

// エネミーの初期化
void InitializeEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY)
{
	enemy[_index].posX = _x;
	enemy[_index].posY = _y;
	enemy[_index].initPosX = _x;
	enemy[_index].initPosY = _y;

	/*enemy[_index].Enemy[ENEMY_MAX];*/

	enemy[_index].direction.x = 0.0f;
	enemy[_index].direction.y = 0.0f;

	enemy[_index].sizeX = _sizeX;
	enemy[_index].sizeY = _sizeY;
	enemy[_index].isJumpFlag = false;
	enemy[_index].DrawFlag = true;
	enemy[_index].jumpPower = 15.0f;
	enemy[_index].intervalCount = 0;
	enemy[_index].jumpCount = 0;

	enemy[_index].Dmg_count = 0;

	// 配列ナンバーを見て最初左右どちらの方向に進むか決める
	if (_index % 2 == 0)
	{
		enemy[_index].moveSpeed = 1.0f;
	}
	else
	{
		enemy[_index].moveSpeed = -1.0f;
	}

	enemy[_index].image = _image;

}

void SecondInitialize(int _index)
{
	enemy[_index].posX = enemy[_index].initPosX;
	enemy[_index].posY = enemy[_index].initPosY;
	enemy[_index].DrawFlag = true;
}

// エネミー更新
void UpdateEnemy(int _index)
{
	//まず入力時にエネミーがどの方向に進むかを決める
	enemy[_index].velocityX = enemy[_index].moveSpeed;
	//重力
	enemy[_index].velocityY += GRAVITY_ACCEL;

	// エネミーが接地状態だったら
	if (enemy[_index].isGround == true)
	{
		// フレームでカウントをとる
		++enemy[_index].intervalCount;
		// フレームでとっているカウントが30以上だったら
		if (enemy[_index].intervalCount >= 30)
		{
			// Y軸にジャンプ力を足すze
			enemy[_index].velocityY -= enemy[_index].jumpPower;
			enemy[_index].intervalCount = 0;
			//enemy[_index].isJumpFlag = true;
		}
	}


	enemy[_index].posX += enemy[_index].velocityX;
	enemy[_index].posY += enemy[_index].velocityY;

	// 移動制限
	float limitL = 0 + (enemy[_index].sizeX / 2.0f);
	if (limitL > enemy[_index].posX)enemy[_index].posX = limitL;

}

//攻撃との当たり判定がしたい
void HitAttackToEnemy(int _index)
{
	
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true && enemy[_index].DrawFlag == true)
		{
			IsHit(enemy[_index].posX, enemy[_index].posY,
				enemy[_index].sizeX, enemy[_index].sizeY,
				attack[i].posX, attack[i].posY,
				attack[i].sizeX, attack[i].sizeY);
			{
				if ((enemy[_index].posX + (enemy[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //攻撃がエネミーに当たったか
					(enemy[_index].posX - (enemy[_index].sizeX / 2.0f) < attack[i].posX + (attack[i].sizeX / 2.0f)) &&
					(enemy[_index].posY + (enemy[_index].sizeY / 2.0f) > attack[i].posY - (attack[i].sizeY / 2.0f)) &&
					(enemy[_index].posY - (enemy[_index].sizeY / 2.0f) < attack[i].posY + (attack[i].sizeY / 2.0f)))
				{
					enemy[_index].DrawFlag = false;

					attack[i].ShotFlag = false;
				}
			}
		}
		
	}

}



// エネミー描画
void DrawEnemy(int _index)
{
	if (enemy[_index].DrawFlag == true)
	{
		float drawPosX = enemy[_index].posX - (enemy[_index].sizeX / 2.0f) - scrollX;
		float drawPosY = enemy[_index].posY - (enemy[_index].sizeY / 2.0f);

		DrawGraph(drawPosX, drawPosY, enemy[_index].image, true);
	}
}


// エネミーの押し戻し
void PushBackEnemy(int _index)
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
			float x = enemy[_index].posX - block[i].posX;
			float y = enemy[_index].posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// 今までの一番近いやつよりも近いブロックがでてきたら情報更新
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// 一番近いやつとの押し戻し
		if (IsHit(enemy[_index], block[nearIndex]))
		{
			// 当たっていたら繰り返しフラグON
			isHit = true;

			// 当たったブロックをどちらに押し戻すか、横と縦の距離から判別
			float lenX = fabs(enemy[_index].posX - block[nearIndex].posX);
			float lenY = fabs(enemy[_index].posY - block[nearIndex].posY);

			if (lenX > lenY) // 左右押し戻し
			{
				// 右か左か?当たった者同士のX座標で比較 
				if (enemy[_index].posX < block[nearIndex].posX)
				{
					// エネミー左押し戻し
					float width = (enemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					enemy[_index].posX = block[nearIndex].posX - width;

					enemy[_index].moveSpeed *= -1.0f;

				}
				else
				{
					// エネミー右押し戻し
					float width = (enemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					enemy[_index].posX = block[nearIndex].posX + width;

					enemy[_index].moveSpeed *= - 1.0f;
				}
			}
			else // 上下押し戻し
			{
				if (enemy[_index].posY < block[nearIndex].posY)
				{
					// エネミー上押し戻し
					float width = (enemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					enemy[_index].posY = block[nearIndex].posY - width;

					// 上押し戻しでの速度リセットは降りてきているときだけ機能させる
					if (enemy[_index].velocityY > 0)enemy[_index].velocityY = 0;
				}
				else
				{
					// エネミー下押し戻し
					float width = (enemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					enemy[_index].posY = block[nearIndex].posY + width;


					if (enemy[_index].velocityY < 0)enemy[_index].velocityY = 0;
				}
			}

		}

		// 当たっている限り繰り返す
	} while (isHit == true);

	// 接地しているかのチェック
	// フラグのリセット
	enemy[_index].isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = enemy[_index];
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// エネミーの足元とブロックの判定
		if (bool isHit = IsHit(obj, block[i]))
		{
			enemy[_index].isGround = true;
			//enemy[_index].isJumpFlag = false;
		}
	}

}