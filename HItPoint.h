#pragma once

// Object�\���̂̑O���錾
struct Object;

// �ϐ�(�g�p�錾)-------------------------
extern Object hitPoint[];		// �v���C���[�f�[�^

// �v���C���[�̏�����
void InitializeHP();

// �v���C���[�̍X�V
void UpdateHP();

// �v���C���[�̕`��
void DrawHP(int count);
