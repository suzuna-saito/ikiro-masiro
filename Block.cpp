//---------------------------------------
//	�u���b�N�֘A
//---------------------------------------
#include "DxLib.h"
#include "System.h"
#include "Block.h"
#include "Map.h"
#include "attack.h"
// �ϐ� ---------------------------------
Object block[1000];

// 1�̃u���b�N�̏�����
// index�ɂ̓u���b�N�̔ԍ����͂���
void InitializeBlock(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY)
{
	// _index�Ԗڂ̃u���b�N��(_x,_y)�̈ʒu�ŏ�����
	block[_index].posX = _x;
	block[_index].posY = _y;

	block[_index].sizeX = _sizeX;
	block[_index].sizeY = _sizeY;

	block[_index].image = _image;
}

// �u���b�N�X�V
void UpdateBlock(int _index)
{
}

//�u���b�N�Ƃ̓����蔻��                      
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
				if ((block[_index].posX + (block[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //�U�����u���b�N�ɓ���������
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

// �u���b�N�`��
// _index�Ԗڂ̃u���b�N��`��
void DrawBlock(int _index)
{
	float drawPosX = block[_index].posX - (block[_index].sizeX / 2.0f) - scrollX;
	float drawPosY = block[_index].posY - (block[_index].sizeY / 2.0f);
	DrawGraph(drawPosX, drawPosY, block[_index].image, true);
}
