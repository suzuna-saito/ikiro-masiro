// �C���N���[�h�K�[�h
// �w�b�_�[�t�@�C���̓ǂݍ��݂̏d����h��
#pragma once

// Object�\���̂̑O���錾
struct Object;

//---------------------------------------
//	�U���֘A�̕ϐ��A�֐��́u�錾�v
//  (�����܂ł�������cpp�t�@�C��)
//---------------------------------------

// �ϐ�(�g�p�錾)-------------------------
extern Object attack[TAMA_MAX];		// �U���f�[�^
extern int attackCount;

// �֐��錾�i�v���g�^�C�v�錾�j------------
// ���������֐����g���܂���Ƃ�����`

// �U���̏�����
void InitializeAttack();

// �U���̍X�V
void UpdateAttack();

// �U���̕`��
void DrawAttack();

// �U���̓����蔻��
void CheckHitAttack(int _index);

// �U���̉����߂�
void PushBackAttack();

// �U���ƓG������������?
void HitAttackToEnemy(int _index);
void HitAttackToFlyEnemy(int _index);

//�U���ƃu���b�N������������
void HitAttackToBlock(int _index);
