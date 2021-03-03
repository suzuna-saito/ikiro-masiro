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
Object flyenemy[50];

// �t���C�G�l�~�[�̏�����
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

	// �z��i���o�[�����čŏ����E�ǂ���̕����ɐi�ނ����߂�
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

// �t���C�G�l�~�[�X�V
void UpdateFlayEnemy(int _index)
{

	//�܂����͎��Ƀt���C�G�l�~�[���ǂ̕����ɐi�ނ������߂�
	flyenemy[_index].velocityX = flyenemy[_index].moveSpeed;

	flyenemy[_index].posX += flyenemy[_index].velocityX;

	// �ړ�����
	float limitL = 0 + (flyenemy[_index].sizeX / 2.0f);
	if (limitL > flyenemy[_index].posX)flyenemy[_index].posX = limitL;

}

//�U���Ƃ̓����蔻�肪������
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
				if ((flyenemy[_index].posX + (flyenemy[_index].sizeX / 2.0f) > attack[i].posX - (attack[i].sizeX / 2.0f)) &&   //�U�����t���C�G�l�~�[�ɓ���������
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


// �t���C�G�l�~�[�`��
void DrawFlayEnemy(int _index)
{
	if (flyenemy[_index].DrawFlag == true)
	{
		float drawPosX = flyenemy[_index].posX - (flyenemy[_index].sizeX / 2.0f) - scrollX;  //����Ȃ��񂶂�ˁH
		float drawPosY = flyenemy[_index].posY - (flyenemy[_index].sizeY / 2.0f);

		DrawGraph(drawPosX, drawPosY, flyenemy[_index].image, true);
	}
}

void PushBackFlyEnemy(int _index)
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
			float x = flyenemy[_index].posX - block[i].posX;
			float y = flyenemy[_index].posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// ���܂ł̈�ԋ߂�������߂��u���b�N���łĂ�������X�V
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// ��ԋ߂���Ƃ̉����߂�
		if (IsHit(flyenemy[_index], block[nearIndex]))
		{
			// �������Ă�����J��Ԃ��t���OON
			isHit = true;

			// ���������u���b�N���ǂ���ɉ����߂����A���Əc�̋������画��
			float lenX = fabs(flyenemy[_index].posX - block[nearIndex].posX);
			float lenY = fabs(flyenemy[_index].posY - block[nearIndex].posY);

			if (lenX > lenY) // ���E�����߂�
			{
				// �E������?���������ғ��m��X���W�Ŕ�r 
				if (flyenemy[_index].posX < block[nearIndex].posX)
				{
					// �G�l�~�[�������߂�
					float width = (flyenemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					flyenemy[_index].posX = block[nearIndex].posX - width;

					flyenemy[_index].moveSpeed *= -1.0f;

				}
				else
				{
					// �G�l�~�[�E�����߂�
					float width = (flyenemy[_index].sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					flyenemy[_index].posX = block[nearIndex].posX + width;

					flyenemy[_index].moveSpeed *= -1.0f;
				}
			}
			else // �㉺�����߂�
			{
				if (flyenemy[_index].posY < block[nearIndex].posY)
				{
					// �v���C���[�㉟���߂�
					float width = (flyenemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					flyenemy[_index].posY = block[nearIndex].posY - width;

					// �㉟���߂��ł̑��x���Z�b�g�͍~��Ă��Ă���Ƃ������@�\������
					if (flyenemy[_index].velocityY > 0)flyenemy[_index].velocityY = 0;
				}
				else
				{
					// �v���C���[�������߂�
					float width = (flyenemy[_index].sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					flyenemy[_index].posY = block[nearIndex].posY + width;


					if (flyenemy[_index].velocityY < 0)flyenemy[_index].velocityY = 0;
				}
			}

		}

		// �������Ă������J��Ԃ�
	} while (isHit == true);

}
