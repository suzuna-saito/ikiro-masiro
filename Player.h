// �C���N���[�h�K�[�h
// �w�b�_�[�t�@�C���̓ǂݍ��݂̏d����h��
#pragma once

// Object�\���̂̑O���錾
struct Object;

//---------------------------------------
//	�v���C���[�֘A�̕ϐ��A�֐��́u�錾�v
//  (�����܂ł�������cpp�t�@�C��)
//---------------------------------------

// �ϐ�(�g�p�錾)-------------------------
extern Object player;		// �v���C���[�f�[�^

// �ϐ�
const float FIRST_JUMP_POWER = 7.5f;

// �֐��錾�i�v���g�^�C�v�錾�j------------
// ���������֐����g���܂���Ƃ�����`

// �v���C���[�̏�����
void InitializePlayer();

//2�T�ڈȍ~�̏������p�֐�
void SecondInitializeIsPlayer();


// �v���C���[�̍X�V
void UpdatePlayer();

// �v���C���[�̕`��
void DrawPlayer();

//�v���C���[�̓����蔻��
void CheckHitPlayer(int _index);
void CheckHitFlyEnemyToPlayer(int _index);

// �v���C���[�̉����߂�
void PushBackPlayer();

// �v���C���[�ƓG������������?
void HitPlayerToEnemy();