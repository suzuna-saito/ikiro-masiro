#pragma once
#include "Block.h"
//---------------------------------------
//	�u���b�N�֘A
//---------------------------------------
struct Object;

// �ϐ�(�g�p�錾)-------------------------
extern Object block[];		// �u���b�N�f�[�^

// �֐��錾�i�v���g�^�C�v�錾�j------------
// ���������֐����g���܂���Ƃ�����`

// �u���b�N�̏�����
void InitializeBlock(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);

// �u���b�N�̍X�V
void UpdateBlock(int _index);

// �u���b�N�̕`��
void DrawBlock(int _index);
