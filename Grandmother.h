// �C���N���[�h�K�[�h
// �w�b�_�[�t�@�C���̓ǂݍ��݂̏d����h��
#pragma once

// Object�\���̂̑O���錾
struct Object;

//---------------------------------------
//	���΂������֘A�̕ϐ��A�֐��́u�錾�v
//  (�����܂ł�������cpp�t�@�C��)
//---------------------------------------

// �ϐ�(�g�p�錾)-------------------------
extern Object	grandmother;		// ���΂������f�[�^



// �֐��錾�i�v���g�^�C�v�錾�j------------
// ���������֐����g���܂���Ƃ�����`

// ���΂������̏�����
void InitializeGrandmother();

// ���ڈȍ~�̂��΂������̏�����
void SecondInitializeIsGrandmother();

//���΂������X�V
void UpdateGrandmother();

// ���΂������̕`��
void DrawGrandmother();

//���΂������̓����蔻��
void CheckHitGrandmother();