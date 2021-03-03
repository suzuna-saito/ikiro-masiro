//---------------------------------------
//	�v���C���[�֘A�̊֐��̎���
//  (�錾�̓w�b�_�[���D�܂���)
//---------------------------------------
#include <math.h>
#include "DxLib.h"
#include "System.h"
#include "Player.h"
#include "Map.h"
#include "Block.h"
#include "enemy.h"
#include "HItPoint.h"

// �ϐ� ---------------------------------
Object player;

// �v���C���[�̏�����
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
	//player.HitFlag = false;  //�Ƃ肠�����������邩�ǂ����I�ȃt���O

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



// �v���C���[�X�V
void UpdatePlayer()
{
	//�܂����͎��Ƀv���C���[���ǂ̕����ɐi�ނ������߂�
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
			//������movespeed�̒l���݂čő呬�x���傫������������x���[���ɂ��X�s�[�h���ő呬�x�ɌŒ肷��
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

	//���񂾂�G���^�[�ōŏ�����
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


	// �d��
	player.velocityY += GRAVITY_ACCEL;

	// �W�����v
	// ��L�[���A�X�y�[�X�L�[����������W�����v

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

	// �ړ�����
	float limitL = 0 + (player.sizeX / 2.0f);
	if (limitL > player.posX)player.posX = limitL;    //����Ȃ񂾂����c�H�H

	if (left == true)
	{
		// �K�v������΁A�v���C���[�̈ړ������ʂ�����ʃX�N���[��
		if (player.posX > SCROLL_R + scrollX)
		{
			scrollX += player.moveSpeed;
		}

		// ��ʂ̍��̕��܂ŗ���ƃX�N���[��
		// �������A���ɂ�����ł���Ƃ������ɂ���
		if (player.posX < SCROLL_L + scrollX &&
			player.velocityX < 0)
		{
			scrollX += player.velocityX;
		}

		// �X�N���[���l��0��菬�����Ȃ�Ȃ��悤�� 
		if (scrollX < 0) scrollX = 0;

		//�X�N���[�����~�߂�^�C�~���O�i���W�j
		if (player.posX >= 5920 )
		{
			left = false;
		}

	}

	
}

//�v���C���[�̓����蔻�肪������
void CheckHitPlayer(int _index)
{
	if (player.Dmg_Flag == false && enemy[_index].DrawFlag == true)
	{
		IsHit(player.posX, player.posY,
			player.sizeX, player.sizeY,
			enemy[_index].posX, enemy[_index].posY,
			enemy[_index].sizeX, enemy[_index].sizeY);
		{

			if ((player.posX + (player.sizeX / 2.0f) > enemy[_index].posX - (enemy[_index].sizeX / 2.0f)) &&   //�G�l�~�[�̓����蔻��
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

//�v���C���[�̓����蔻�肪������
void CheckHitFlyEnemyToPlayer(int _index)
{
	if (player.Dmg_Flag == false && flyenemy[_index].DrawFlag == true)
	{
		IsHit(player.posX, player.posY,
			player.sizeX, player.sizeY,
			flyenemy[_index].posX, flyenemy[_index].posY,
			flyenemy[_index].sizeX, flyenemy[_index].sizeY);
		{

			if ((player.posX + (player.sizeX / 2.0f) > flyenemy[_index].posX - (flyenemy[_index].sizeX / 2.0f)) &&   //�t���C�G�l�~�[�̓����蔻��
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


// �v���C���[�`��
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



// �v���C���[�̉����߂�
void PushBackPlayer()
{
	// �����߂������ׂĊ�������܂Ń��[�v
	bool isHit = false;

	do
	{
		// �Ƃ肠�����J��Ԃ��t���OOFF
		isHit = false;

		// ��ԋ߂��u���b�N��T��
		int nearIndex = -1; // ��ԋ߂��u���b�N�̔ԍ�
		float nearLength = 10000000000; // ��ԋ߂�����

		// �S�Ẵu���b�N����߂����̂�T��
		for (int i = 0; i < blockCount; i++)
		{
			// �u���b�N�ƃv���C���[�̋����v�Z
			float x = player.posX - block[i].posX;
			float y = player.posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// ���܂ł̈�ԋ߂�������߂��u���b�N���łĂ�������X�V
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// ��ԋ߂���Ƃ̉����߂�
		if (IsHit(player, block[nearIndex]))
		{
			// �������Ă�����J��Ԃ��t���OON
			isHit = true;

			// ���������u���b�N���ǂ���ɉ����߂����A���Əc�̋������画��
			float lenX = fabs(player.posX - block[nearIndex].posX);
			float lenY = fabs(player.posY - block[nearIndex].posY);

			if (lenX > lenY) // ���E�����߂�
			{
				// �E������?���������ғ��m��X���W�Ŕ�r 
				if (player.posX < block[nearIndex].posX)
				{
					// �v���C���[�������߂�
					float width = (player.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					player.posX = block[nearIndex].posX - width;
				}
				else
				{
					// �v���C���[�E�����߂�
					float width = (player.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					player.posX = block[nearIndex].posX + width;
				}
			}
			else // �㉺�����߂�
			{
				if (player.posY < block[nearIndex].posY)
				{
					// �v���C���[�㉟���߂�
					float width = (player.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					player.posY = block[nearIndex].posY - width;

					// �㉟���߂��ł̑��x���Z�b�g�͍~��Ă��Ă���Ƃ������@�\������
					if (player.velocityY > 0)player.velocityY = 0;
				}
				else
				{
					// �v���C���[�������߂�
					float width = (player.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					player.posY = block[nearIndex].posY + width;


					if (player.velocityY < 0)player.velocityY = 0;
				}
			}
		}

		// �������Ă������J��Ԃ�
	} while (isHit == true);


	// �ڒn���Ă��邩�̃`�F�b�N
	// �t���O�̃��Z�b�g
	player.isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = player;
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// �v���C���[�̑����ƃu���b�N�̔���
		if (bool isHit = IsHit(obj, block[i]))
		{
			player.isGround = true;
		}
	}

	//�f�o�b�N�p
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		player.posY = 30;
		player.velocityY = 0.2f;
	}

}