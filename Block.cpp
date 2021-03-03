//---------------------------------------
//	ブロック関連
//---------------------------------------
#include "DxLib.h"
#include "System.h"
#include "Block.h"
#include "Map.h"
#include "attack.h"
// 変数 ---------------------------------
Object block[1000];

// 1個のブロックの初期化
// indexにはブロックの番号がはいる
void InitializeBlock(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY)
{
	// _index番目のブロックを(_x,_y)の位置で初期化
	block[_index].posX = _x;
	block[_index].posY = _y;

	block[_index].sizeX = _sizeX;
	block[_index].sizeY = _sizeY;

	block[_index].image = _image;
}

// ブロック更新
void UpdateBlock(int _index)
{
}

//ブロックとの当たり判定                      
void HitAttackToBlock(int _index)
{

	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true)
		{
			IsHit(block[_index].posX, block[_index].posY,
				block[_index].sizeX, block[_index].sizeY,
				attack[i].posX, attack[i].posY,
				attack[i].sizeX, attack[i].sizeY);
			{
				if ((block[_index].posX + (block[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //攻撃がブロックに当たったか
					(block[_index].posX - (block[_index].sizeX / 2.0f) < attack[i].posX + (attack[i].sizeX / 2.0f)) &&
					(block[_index].posY + (block[_index].sizeY / 2.0f) > attack[i].posY - (attack[i].sizeY / 2.0f)) &&
					(block[_index].posY - (block[_index].sizeY / 2.0f) < attack[i].posY + (attack[i].sizeY / 2.0f)))
				{

					attack[i].ShotFlag = false;
				}
			}
		}

	}
}

// ブロック描画
// _index番目のブロックを描画
void DrawBlock(int _index)
{
	float drawPosX = block[_index].posX - (block[_index].sizeX / 2.0f) - scrollX;
	float drawPosY = block[_index].posY - (block[_index].sizeY / 2.0f);
	DrawGraph(drawPosX, drawPosY, block[_index].image, true);
}
