//---------------------------------------
//	プレイヤー関連の関数の実装
//  (宣言はヘッダーが好ましい)
//---------------------------------------
#include <math.h>
#include "DxLib.h"
#include "System.h"
#include "Player.h"
#include "attack.h"
#include "Map.h"
// 変数 ---------------------------------
Object grandmother;
bool preInversionFlag;
// おばあちゃんの初期化
void InitializeGrandmother()
{
	grandmother.posX = 6080;
	grandmother.posY = 400;

	grandmother.velocityY = 0;

	grandmother.sizeX = 55;
	grandmother.sizeY = 96;

	grandmother.jumpPower = 3.0f;

	grandmother.moveSpeed = 0.5f;

	grandmother.intervalCount = 0;

	grandmother.hpCount = 4;

	grandmother.image = LoadGraph("Image/grandmother.png");
	grandmother.DmgImage = LoadGraph("Image/Dmg.png");
	grandmother.DmgImage2 = LoadGraph("Image/Dmg2.png");
	grandmother.DmgImage3 = LoadGraph("Image/Dmg3.png");
	grandmother.EndImage = LoadGraph("Image/obb_dmg.png");

	grandmother.DrawFlag = true;
	grandmother.InversionFlag = false;
	grandmother.NotChangeFlag = false;
	preInversionFlag = false;
}

void SecondInitializeIsGrandmother()
{
	grandmother.posX = 6080;
	grandmother.posY = 400;

	grandmother.hpCount = 4;

	grandmother.moveSpeed = 0.5f;

	grandmother.intervalCount = 0;

	preInversionFlag = false;
}

// おばあちゃん更新
void UpdateGrandmother()
{
	if (player.posX == grandmother.posX)
	{
		preInversionFlag = false;
	}
	else
	{
		preInversionFlag = true;
	}

	if (preInversionFlag)
	{
		// プレイヤーが一定の位置を超えたら
		if ((grandmother.posX - player.posX) <= 300 && (grandmother.posX - player.posX) >= 0)
		{
			grandmother.InversionFlag = false;

			grandmother.posX -=grandmother.moveSpeed;

			grandmother.intervalCount++;

			// フレームでとっているカウントが30以上だったら
			if (grandmother.intervalCount >= 3)
			{
				// Y軸にジャンプ力を足すze
				grandmother.velocityY -= grandmother.jumpPower;
				grandmother.intervalCount = 0;
				//enemy[_index].isJumpFlag = true;
			}
		}
		else if ((player.posX - grandmother.posX) >= 0)
		{
			grandmother.posX += grandmother.moveSpeed;
			grandmother.InversionFlag = true;
		}
		if (player.posX + 10.0f <= grandmother.posX >= player.posX - 10.0f)
		{
			grandmother.InversionFlag = false;
		}

	}


	/*enemy[_index].posX += enemy[_index].velocityX;
	enemy[_index].posY += enemy[_index].velocityY;*/

	// 移動制限
	float limitL = 0 + (grandmother.sizeX / 2.0f);
	if (limitL > grandmother.posX)grandmother.posX = limitL;

}

//おばあちゃんの当たり判定
void CheckHitGrandmother()
{
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (attack[i].ShotFlag == true && grandmother.hpCount >= 0 && player.posX <= 6155)
		{
			IsHit(grandmother.posX, grandmother.posY,
				grandmother.sizeX, grandmother.sizeY,
				attack[i].posX, attack[i].posY,
				attack[i].sizeX, attack[i].sizeY);
			{
				if ((grandmother.posX + (grandmother.sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //攻撃がおばあちゃんに当たったか
					(grandmother.posX - (grandmother.sizeX / 2.0f) < attack[i].posX + (attack[i].sizeX / 2.0f)) &&
					(grandmother.posY + (grandmother.sizeY / 2.0f) > attack[i].posY - (attack[i].sizeY / 2.0f)) &&
					(grandmother.posY - (grandmother.sizeY / 2.0f) < attack[i].posY + (attack[i].sizeY / 2.0f)))
				{
					attack[i].ShotFlag = false;

					--grandmother.hpCount;
				} 
			}
		}

	}

	/*{
		grandmother.Dmg_Flag = true;

		enemy.moveSpeed *= -1;
	}*/

}

// おばあちゃん描画
void DrawGrandmother()
{
	float drawPosX = grandmother.posX - (grandmother.sizeX / 2.0f) - scrollX;
	float drawPosY = grandmother.posY - (grandmother.sizeY / 2.0f);

	float drawPos2X = grandmother.posX - (100 / 2.0f) - scrollX;

	float drawPos2Y = grandmother.posY - (55 / 2.0f);

	if (grandmother.hpCount == 4)      //攻撃をくらってないとき
	{
		if (grandmother.InversionFlag == true)
		{
			DrawTurnGraph(drawPosX , drawPosY, grandmother.image, true);
		}
		else
		{
			DrawGraph(drawPosX, drawPosY, grandmother.image, true);
		}
	}

	else if (grandmother.hpCount == 3)           //1回攻撃を食らったとき
	{
		if (grandmother.InversionFlag == true)
		{
			grandmother.posX = grandmother.posX ;

			DrawTurnGraph(drawPos2X , drawPosY, grandmother.image, true);
		}
		else
		{
			DrawGraph(drawPos2X, drawPosY, grandmother.DmgImage, true);
		}
	}

	else if (grandmother.hpCount == 2)            //2回攻撃を食らったとき
	{
		if (grandmother.InversionFlag == true)
		{
			DrawTurnGraph(drawPos2X , drawPosY, grandmother.image, true);
		}
		else
		{
			DrawGraph(drawPos2X, drawPosY, grandmother.DmgImage2, true);
		}
	}

	else if (grandmother.hpCount == 1)            //3回攻撃を食らったとき
	{
		if (grandmother.InversionFlag == true)
		{
			DrawTurnGraph(drawPos2X , drawPosY, grandmother.image, true);
		}
		else
		{
			DrawGraph(drawPos2X, drawPosY, grandmother.DmgImage3, true);
		}
	}

}
