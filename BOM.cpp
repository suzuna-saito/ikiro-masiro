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

// �ϐ� ---------------------------------
Object bom;

// �{���̏�����
void Initializebom()
{
	bom.posX = bom.posX;
	bom.posY = bom.posY;

	bom.sizeX = PLAYER_SIZE - 5;
	bom.sizeY = PLAYER_SIZE;

	bom.image = LoadGraph("Image/bom.png");

	bom.BomFlag = false;
	bom.Dmg_count = 0;

	bom.bom[BOM_MAX] = 0;
}



//�@�{���X�V
void Updatebom()
{
	//�X�y�[�X�L�[�Ń{���t���O���I���ɂ���
	if (keyState[KEY_INPUT_SPACE] == 1 && bom.inputFlag == false)
	{
		bom.BomFlag = true;
	}

	// �d��
	bom.velocityY += GRAVITY_ACCEL;

	bom.posX += bom.velocityX * bom.direction.x;
	bom.posY += bom.velocityY;

}
//�{���̓����蔻�肪������
void CheckHitbom()
{
	IsHit(bom.posX, bom.posY,
		bom.sizeX, bom.sizeY,
		enemy.posX, enemy.posY,
		enemy.sizeX, enemy.sizeY);
	{

		if ((bom.posX + (bom.sizeX / 2.0f) > enemy.posX - (enemy.sizeX / 2.0f)) &&
			(bom.posX - (bom.sizeX / 2.0f) < enemy.posX + (enemy.sizeX / 2.0f)) &&
			(bom.posY + (bom.sizeY / 2.0f) > enemy.posY - (enemy.sizeY / 2.0f)) &&
			(bom.posY - (bom.sizeY / 2.0f) < enemy.posY + (enemy.sizeY / 2.0f)))
		{
			{
				bom.Dmg_Flag = true;

				enemy.moveSpeed *= -1;
			}
		}

		return;
	}

	/*{
		bom.Dmg_Flag = true;

		enemy.moveSpeed *= -1;
	}*/

}


// �v���C���[�`��
void DrawPlayer()
{
	float drawPosX = bom.posX - (bom.sizeX / 2.0f) - scrollX;
	float drawPosY = bom.posY - (bom.sizeY / 2.0f);

	//�X�y�[�X�L�[�Ŕ��e�\��

	if (keyState[KEY_INPUT_SPACE] == 1)
	{
		bom.BomFlag = true;
	}

	if (bom.BomFlag == true)
	{
		//��ʏ�ɏo�Ă��Ȃ��{�������邩�A�{���̐������J��Ԃ��Ē��ׂ�
		for (int i = 0; i < BOM_MAX; i++)
		{
			bom.bom[i];


		}

		//		{
		//			bom.timer = 100;

		//			�{������ʏ�ɏo�Ă��Ȃ��ꍇ�͂��̃{������ʂɏo��
		//			if (bom.bom[i] == 0)
		//			{
		//				if (bom.timer >= 0)
		//				{
		//					DrawGraph(drawPosX, drawPosY, bom.image2, TRUE);
		//				}

		//				�{��i�͌����_�������đ��݂���̂ŁA���ݏ�Ԃ�ێ�����ϐ��ɂP��������
		//				bom.bom[i] = 1;

		//				bom.timer--;

		//				if (bom.timer <= 0)
		//				{
		//					��e���o�����̂Œe���o�����[�v���甲���܂�
		//					break;
		//				}
		//			}
		//		}

		//	}
		//}*/
	}


	DrawGraph(drawPosX, drawPosY, bom.image, true);

	if (bom.Dmg_Flag == true)
	{
		DrawGraph(drawPosX, drawPosY, bom.dmg_image, true);

		bom.Dmg_count++;
		if (bom.Dmg_count >= 30)
		{
			DrawGraph(drawPosX, drawPosY, bom.image, true);
		}
	}
	bom.Dmg_Flag = false;
	bom.Dmg_count = 0;
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
			float x = bom.posX - block[i].posX;
			float y = bom.posY - block[i].posY;
			float length = sqrt(x * x + y * y);

			// ���܂ł̈�ԋ߂�������߂��u���b�N���łĂ�������X�V
			if (nearLength > length)
			{
				nearIndex = i;
				nearLength = length;
			}
		}

		// ��ԋ߂���Ƃ̉����߂�
		if (IsHit(bom, block[nearIndex]))
		{
			// �������Ă�����J��Ԃ��t���OON
			isHit = true;

			// ���������u���b�N���ǂ���ɉ����߂����A���Əc�̋������画��
			float lenX = fabs(bom.posX - block[nearIndex].posX);
			float lenY = fabs(bom.posY - block[nearIndex].posY);

			if (lenX > lenY) // ���E�����߂�
			{
				// �E������?���������ғ��m��X���W�Ŕ�r 
				if (bom.posX < block[nearIndex].posX)
				{
					// �v���C���[�������߂�
					float width = (bom.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					bom.posX = block[nearIndex].posX - width;
				}
				else
				{
					// �v���C���[�E�����߂�
					float width = (bom.sizeX / 2.0f) + (block[nearIndex].sizeX / 2.0f);
					bom.posX = block[nearIndex].posX + width;
				}
			}
			else // �㉺�����߂�
			{
				if (bom.posY < block[nearIndex].posY)
				{
					// �v���C���[�㉟���߂�
					float width = (bom.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					bom.posY = block[nearIndex].posY - width;

					// �㉟���߂��ł̑��x���Z�b�g�͍~��Ă��Ă���Ƃ������@�\������
					if (bom.velocityY > 0)bom.velocityY = 0;
				}
				else
				{
					// �v���C���[�������߂�
					float width = (bom.sizeY / 2.0f) + (block[nearIndex].sizeY / 2.0f);
					bom.posY = block[nearIndex].posY + width;


					if (bom.velocityY < 0)bom.velocityY = 0;
				}
			}
		}

		// �������Ă������J��Ԃ�
	} while (isHit == true);


	// �ڒn���Ă��邩�̃`�F�b�N
	// �t���O�̃��Z�b�g
	bom.isGround = false;

	for (int i = 0; i < blockCount; i++)
	{
		Object obj;
		obj = bom;
		obj.sizeY += 10;
		/*obj.sizeX -= 10;*/

		// �v���C���[�̑����ƃu���b�N�̔���
		if (bool isHit = IsHit(obj, block[i]))
		{
			bom.isGround = true;
		}
	}

	//�f�o�b�N�p
	if (CheckHitKey(KEY_INPUT_B) == 1)
	{
		bom.posY = 30;
		bom.velocityY = 0.2f;
	}

}