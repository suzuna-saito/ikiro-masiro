// �C���N���[�h�K�[�h
// �w�b�_�[�t�@�C���̓ǂݍ��݂̏d����h��
#pragma once

// Object�\���̂̑O���錾
struct Object;

//---------------------------------------
//	�G�l�~�[�֘A�̕ϐ��A�֐��́u�錾�v
//  (�����܂ł�������cpp�t�@�C��)
//---------------------------------------

// �ϐ�(�g�p�錾)-------------------------
extern Object enemy[50];		// �G�l�~�[�f�[�^
extern Object flyenemy[50];		// �t���C�G�l�~�[�f�[�^

// �֐��錾�i�v���g�^�C�v�錾�j------------
// ���������֐����g���܂���Ƃ�����`

// �G�l�~�[�̏�����
void InitializeEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);
void InitializeFlayEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);

//2�T�ڈȍ~�̏������p�֐�
void SecondInitialize(int _index);
void SecondInitializeIsFlyEnemy(int _index);
// �G�l�~�[�̍X�V
void UpdateEnemy(int _index);
void UpdateFlayEnemy(int _index);

// �G�l�~�[�̕`��
void DrawEnemy(int _index);
void DrawFlayEnemy(int _index);

// �G�l�~�[�̉����߂�
void PushBackEnemy(int _index);
void PushBackFlyEnemy(int _index);
