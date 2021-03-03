#include <math.h>
#include "DxLib.h"
#include "FireWorks.h"
#include "Map.h"

int FireworksImage;
int FireworksImage2;
int FireworksImage3;

bool DrawFireworksFlag;
bool DrawFireworks2Flag;
bool DrawFireworks3Flag;

int Draw_count = 0;

void InitFireworks()
{
	FireworksImage = LoadGraph("Image/hanabi.png");
	FireworksImage2 = LoadGraph("Image/hanabi2.png");
	FireworksImage3 = LoadGraph("Image/hanabi3.png");

	DrawFireworksFlag = false;
	DrawFireworks2Flag = false;
	DrawFireworks3Flag = false;
}

void SecondInitFireworks()
{
	DrawFireworksFlag = false;
	DrawFireworks2Flag = false;
	DrawFireworks3Flag = false;
	Draw_count = 0;
}

void UpdateFireworks()
{

	Draw_count++;

	if (Draw_count >= 1 && Draw_count <= 60)
	{
		DrawFireworksFlag = true;
	}
	else if (Draw_count >= 60 && Draw_count <= 120)
	{
		DrawFireworksFlag = false;
		DrawFireworks2Flag = true;
	}
	else if (Draw_count >= 120 && Draw_count <= 180)
	{
		DrawFireworks2Flag = false;
		DrawFireworks3Flag = true;
	}

}

void DrawFireworks()
{
	float drawPosX = 6150 - (64 / 2.0f) - scrollX;
	float drawPosY = 300 - (64 / 2.0f);

	float drawPosX2 = 6100 - (70 / 2.0f) - scrollX;
	float drawPosY2 = 270 - (70 / 2.0f);

	float drawPosX3 = 6140 - (100 / 2.0f) - scrollX;
	float drawPosY3 = 230 - (100 / 2.0f);
	
	if (DrawFireworksFlag == true)
	{
		DrawGraph(drawPosX, drawPosY, FireworksImage, true); //‰Ô‰Î1
	}
	else if (DrawFireworks2Flag == true)
	{
		DrawGraph(drawPosX2, drawPosY2, FireworksImage2, true); //‰Ô‰Î2
	}
	else if (DrawFireworks3Flag == true)
	{
		DrawGraph(drawPosX3, drawPosY3, FireworksImage3, true); //‰Ô‰Î3
	}
}