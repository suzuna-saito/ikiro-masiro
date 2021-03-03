//---------------------------------------
//	�}�b�v�֘A
//---------------------------------------
#include "DxLib.h"
#include "System.h"
#include "Map.h"
#include "Block.h"
#include "enemy.h"
#include "Grandmother.h"
#include "Player.h"

// �w�i�f�[�^
BgData bgData[BG_DATA_NUM];
BgData title;
BgData gameov;
BgData gameov2;
BgData clear;

// �X�N���[����
float scrollX;

// �u���b�N�̐�
int blockCount = 0;
int enemyCount = 0;
int flyenemyCount = 0;
bool left = true;

int blockImage1;
int blockImage2;
int blockImage3;
int enemyImage1;
int enemyImage2;
int houseImage;
int saveImage;
int saveImage2;
int fireworksImage;
int fireworksImage2;
int fireworksImage3;

//�Z�[�u�|�C���g�܂ōs�������ǂ���
bool SaveFlag;

//�ԉ΂łĂ邩�ǂ���
bool HanabiDrawFlag = false;
bool HanabiDrawFlag2 = false;
bool HanabiDrawFlag3 = false;

////�J�n�摜�v�����[�h
//int startimg = LoadGraph("img/start.png");
//
//void Opening() 
//{
//	DrawString(640, 480, "�Q�[���X�^�[�g�I�iEnter�L�[���v�b�V���j", startimg);
//	if (keyState[KEY_INPUT_RETURN] == 1)
//	{
//		InitializeMap();
//	}
//}

// �}�b�v�̏�����
void InitializeMap()
{
	scrollX = 0;
	// �w�i�f�[�^�̏�����
	int bgImage0 = LoadGraph("Image/bg008.png");
	
	for (int i = 0; i < BG_DATA_NUM; i++)
	{
		bgData[i].image = bgImage0;

		bgData[i].posX = BG_SIZE_X * i;
		bgData[i].posY = 0;

		bgData[i].sizeX = BG_SIZE_X;
		bgData[i].sizeY = BG_SIZE_Y;

	}

	// �u���b�N�̉摜���ꊇ�œǂݍ���
	// �������̒i�K�ŉ摜�̃n���h���i�ۑ��ԍ��j��������΂����̂�
	// ���[�J���ϐ���OK
	blockImage1 = LoadGraph("Image/block1.png");
	blockImage2 = LoadGraph("Image/block2.png");
	blockImage3 = LoadGraph("Image/aitem.png");
	enemyImage1 = LoadGraph("Image/teki0.png");
	enemyImage2 = LoadGraph("Image/teki1.png");
	// �Ƃ̃f�[�^
	houseImage = LoadGraph("Image/house.png");

	


	// �}�b�v�f�[�^�����Ƀu���b�N�̏��������s��
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// �����ԖځA�c���Ԗڂ̃f�[�^���擾
			int type = MAP_DATA[y][x];

			//�@���W������
			float posX = (x * BLOCK_SIZE) + (BLOCK_SIZE / 2.0f);
			float posY = (y * BLOCK_SIZE) + (BLOCK_SIZE / 2.0f);

			//�@Type�����Ăǂ̉摜�ɂ��邩�����߂�(�O���E���h)
			if (type == 1 || type == 2 || type == 3 )
			{
				int graph;
				// �K�v�ȉ摜�������Ă���
				graph = SelectGraph(type);
				InitializeBlock(blockCount, graph, posX, posY, BLOCK_SIZE, BLOCK_SIZE);
				blockCount++;
			}
			//�@Type�����Ăǂ̉摜�ɂ��邩�����߂�(�G)
			if (type == 4 || type == 5)
			{
				int graph;
				// �K�v�ȉ摜�������Ă���
				graph = SelectGraph(type);
				InitializeEnemy(enemyCount, graph, posX, posY, ENEMY_SIZE, ENEMY_SIZE);
				InitializeFlayEnemy(flyenemyCount, graph, posX, posY, ENEMY_SIZE, ENEMY_SIZE);
				if (type == 4)
				{
					++enemyCount;
				}
				if (type == 5)
				{
					++flyenemyCount;
				}
			}

		}
	}
}

float SavePosX = 3580;
float SavePosY = 300;

float SaveSizeX = 43;
float SaveSizeY = 59;

// �Z�[�u�̏�����
void InitializeSave()
{
	SaveFlag = false;

	saveImage = LoadGraph("Image/save.png");
	saveImage2 = LoadGraph("Image/saveOk.png");
}

// �}�b�v�̍X�V
void UpdateMap()
{

}

// �w�i�̍X�V
void UpdateBg()
{
	if (SaveFlag == false)
	{
		IsHit(player.posX, player.posY,
			player.sizeX, player.sizeY,
			SavePosX, SavePosY ,
			SaveSizeX, SaveSizeY);
		{

			if ((player.posX + (player.sizeX / 2.0f) > SavePosX - (SaveSizeX / 2.0f)) &&   //�Z�[�u�ɓ����������ǂ���
				(player.posX - (player.sizeX / 2.0f) < SavePosX + (SaveSizeX / 2.0f)) &&
				(player.posY + (player.sizeY / 2.0f) > SavePosY - (SaveSizeY / 4.0f)) &&
				(player.posY - (player.sizeY / 2.0f) < SavePosY + (SaveSizeY / 2.0f)))
			{
				SaveFlag = true;
			}
		}
	}
}

// �}�b�v�̕`��
void DrawMap()
{
	// �u���b�N���ׂĕ`��
	// �u���b�N�̍��ꂽ����blockCount�ɕۑ�����Ă���
	for (int i = 0; i < blockCount; i++)
	{
		DrawBlock(i);
	}

	for (int i = 0; i < enemyCount; i++)
	{
		DrawEnemy(i);
	}

	for (int i = 0; i < flyenemyCount; i++)
	{
		DrawFlayEnemy(i);
	}
}

// �w�i�̕`��
void DrawBg()
{
	
	for (int i = 0; i < BG_DATA_NUM; i++)
	{
		DrawGraph(bgData[i].posX - scrollX / 4, bgData[i].posY, bgData[i].image, true);
	}

	//�w�i�i�Ɓj�̕`��
	
	float drawPosX = 6130 - (180 / 2.0f) - scrollX;
	float drawPosY = 375 - (171 / 2.0f);
	DrawGraph(drawPosX, drawPosY, houseImage, true);

	//�w�i�i�Z�[�u�|�C���g�j�̕`��
	if (SaveFlag == false)
	{
		float drawPosX2 = SavePosX - (SaveSizeX / 2.0f) - scrollX;
		float drawPosY2 = SavePosY - (SaveSizeY / 2.0f);
		DrawGraph(drawPosX2, drawPosY2, saveImage, true);
	}
	else if (SaveFlag == true)
	{
		float drawPosX2 = SavePosX - (SaveSizeX / 2.0f) - scrollX;
		float drawPosY2 = SavePosY - (SaveSizeY / 2.0f);
		DrawGraph(drawPosX2, drawPosY2, saveImage2, true);
	}

}

int SelectGraph(int _type)
{
	switch (_type)
	{
	case(1):
		return blockImage1;
	case(2):
		return blockImage2;
	case(3):
		return blockImage3;
	case(4):
		return enemyImage1;
	case(5):
		return enemyImage2;
	}
}

void LoadImg()
{
	title.image = LoadGraph("Image/title1.png");//Load�̓��[�v���n�܂�O�ɏ���
	gameov.image = LoadGraph("Image/game_over.png");
	gameov2.image = LoadGraph("Image/game_over2.png");
	clear.image = LoadGraph("Image/clear.png");

	title.posX = BG_SIZE_X;
	title.posY = 0;

	title.sizeX = BG_SIZE_X;
	title.sizeY = BG_SIZE_Y;

	gameov.posX = BG_SIZE_X;
	gameov.posY = 0;

	gameov.sizeX = BG_SIZE_X;
	gameov.sizeY = BG_SIZE_Y;

	clear.posX = BG_SIZE_X;
	clear.posY = 0;
	
	clear.sizeX = BG_SIZE_X;
	clear.sizeY = BG_SIZE_Y;
}

void game_title()
{
	DrawGraph(0, 0, title.image, true);

}

void game_over()
{
	if (grandmother.hpCount <= 0 && player.posX >= 6156)
	{
		DrawGraph(0, 0, gameov2.image, true);
	}
	else if (player.hpCount == 0 || player.posY >= 500)
	{
		DrawGraph(0, 0, gameov.image, true);
	}

}

void game_clear()
{
	if (HanabiDrawFlag == false)
	{
		DrawGraph(0, 0, clear.image, true);
	}

}