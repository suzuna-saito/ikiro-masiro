#include "HItPoint.h"
#include "System.h"
#include "DxLib.h"
#include "Map.h"

Object hitPoint[PLAYER_HP];

void InitializeHP()
{
	for (int i = 0; i < PLAYER_HP; i++)
	{
		hitPoint[i].posX = 64.0f + i * 64;
		hitPoint[i].posY = 64.0f;
		hitPoint[i].sizeX = PLAYER_SIZE - 5;
		hitPoint[i].sizeY = PLAYER_SIZE;

		hitPoint[i].image = LoadGraph("Image/hp.png");
	}
}

void UpdateHP()
{
}

void DrawHP(int count)
{
	for (int i = 0; i < 0 + count; i++)
	{
		float drawPosX = hitPoint[i].posX - (hitPoint[i].sizeX / 2.0f) /*- scrollX*/;
		float drawPosY = hitPoint[i].posY - (hitPoint[i].sizeY / 2.0f);
		DrawGraph(drawPosX, drawPosY, hitPoint[i].image, true);
	}

}
