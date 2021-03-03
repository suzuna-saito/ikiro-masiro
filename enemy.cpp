//---------------------------------------
//	�G�l�~�[�֘A�̊֐��̎���
//  (�錾�̓w�b�_�[���D�܂���)
//---------------------------------------
#include <math.h>
#include "DxLib.h"
#include "System.h"
#include "enemy.h"
#include "Map.h"
#include "Block.h"
#include "attack.h"

// �ϐ� ---------------------------------
Object enemy[50];

// �G�l�~�[�̏�����
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

	// �z��i���o�[�����čŏ����E�ǂ���̕����ɐi�ނ����߂�
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

// �G�l�~�[�X�V
void UpdateEnemy(int _index)
{
	//�܂����͎��ɃG�l�~�[���ǂ̕����ɐi�ނ������߂�
	enemy[_index].velocityX = enemy[_index].moveSpeed;
	//�d��
	enemy[_index].velocityY += GRAVITY_ACCEL;

	// �G�l�~�[���ڒn��Ԃ�������
	if (enemy[_index].isGround == true)
	{
		// �t���[���ŃJ�E���g���Ƃ�
		++enemy[_index].intervalCount;
		// �t���[���łƂ��Ă���J�E���g��30�ȏゾ������
		if (enemy[_index].intervalCount >= 30)
		{
			// Y���ɃW�����v�͂𑫂�ze
			enemy[_index].velocityY -= enemy[_index].jumpPower;
			enemy[_index].intervalCount = 0;
			//enemy[_index].isJumpFlag = true;
		}
	}


	enemy[_index].posX += enemy[_index].velocityX;
	enemy[_index].posY += enemy[_index].velocityY;

	// �ړ�����
	float limitL = 0 + (enemy[_index].sizeX / 2.0f);
	if (limitL > enemy[_index].posX)enemy[_index].posX = limitL;

}

//�U���Ƃ̓����蔻�肪������
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
				if ((enemy[_index].posX + (enemy[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //�U�����G�l�~�[�ɓ���������
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



// �G�l�~�[�`��
void DrawEnemy(int _index)
{
	if (enemy[_index].DrawFlag == true)
	{
		float drawPosX = enemy[_index].posX - (enemy[_index].sizeX / 2.0f) - scrollX;
		float drawPosY = enemy[_index].posY - (enemy[_index].sizeY / 2.0f);

		DrawGraph(drawPosX, drawPosY, enemy[_index].image, true);
	}
}


// �G�l�~�[�̉����߂�
void PushBackEnemy(int _index)
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
			float x = enemy[_index].posX - block[i].posX;
			float y = enemy[_index].posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// ���܂ł̈�ԋ߂�������߂��u���b�N���łĂ�������X�V
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// ��ԋ߂���Ƃ̉����߂�
		if (IsHit(enemy[_index], block[nearIndex]))
		{
			// �������Ă�����J��Ԃ��t���OON
			isHit = true;

			// ���������u���b�N���ǂ���ɉ����߂����A���Əc�̋������画��
			float lenX = fabs(enemy[_index].posX - block[nearIndex].posX);
			float lenY = fabs(enemy[_index].posY - block[nearIndex].posY);

			if (lenX > lenY) // ���E�����߂�
			{
				// �E������?���������ғ��m��X���W�Ŕ�r 
				if (enemy[_index].posX < block[nearIndex].posX)
				{
					// �G�l�~�[�������߂�
					float width = (enemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					enemy[_index].posX = block[nearIndex].posX - width;

					enemy[_index].moveSpeed *= -1.0f;

				}
				else
				{
					// �G�l�~�[�E�����߂�
					float width = (enemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					enemy[_index].posX = block[nearIndex].posX + width;

					enemy[_index].moveSpeed *= - 1.0f;
				}
			}
			else // �㉺�����߂�
			{
				if (enemy[_index].posY < block[nearIndex].posY)
				{
					// �G�l�~�[�㉟���߂�
					float width = (enemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					enemy[_index].posY = block[nearIndex].posY - width;

					// �㉟���߂��ł̑��x���Z�b�g�͍~��Ă��Ă���Ƃ������@�\������
					if (enemy[_index].velocityY > 0)enemy[_index].velocityY = 0;
				}
				else
				{
					// �G�l�~�[�������߂�
					float width = (enemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					enemy[_index].posY = block[nearIndex].posY + width;


					if (enemy[_index].velocityY < 0)enemy[_index].velocityY = 0;
				}
			}

		}

		// �������Ă������J��Ԃ�
	} while (isHit == true);

	// �ڒn���Ă��邩�̃`�F�b�N
	// �t���O�̃��Z�b�g
	enemy[_index].isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = enemy[_index];
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// �G�l�~�[�̑����ƃu���b�N�̔���
		if (bool isHit = IsHit(obj, block[i]))
		{
			enemy[_index].isGround = true;
			//enemy[_index].isJumpFlag = false;
		}
	}

}