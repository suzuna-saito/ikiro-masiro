// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);                      // �e�N�X�`�����t�@�C�����烍�[�h����
	void Unload();                                               // �e�N�X�`���������
	void CreateFromSurface(struct SDL_Surface* surface);         // SDL�T�[�t�F�X����e�N�X�`�����쐬����

	void SetActive();                                            // ���̃e�N�X�`�����A�N�e�B�u(�|���S���`��Ŏg�p�j�ɂ���

	int GetWidth() const { return mWidth; }                      // �e�N�X�`����
	int GetHeight() const { return mHeight; }                    // �e�N�X�`������

	unsigned int GetTextureID() { return mTextureID; }           // �e�N�X�`��ID�Ԃ�
private:
	unsigned int mTextureID;                                     // �e�N�X�`��ID
	int mWidth;                                                  // �e�N�X�`����
	int mHeight;                                                 // �e�N�X�`������
};
#pragma once
