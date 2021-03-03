// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
#include "Game.h"
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	// SDL�T�[�t�F�X���e�N�X�`������쐬
	SDL_Texture* tex = nullptr;
	SDL_Surface* surf = IMG_Load(fileName.c_str());
	if (!surf)
	{
		printf("�e�N�X�`���ǂݍ��݂Ɏ��s %s", fileName.c_str());
		return false;
	}

	// �T�[�t�F�X����e�N�X�`�������
	tex = SDL_CreateTextureFromSurface(GAMEINSTANCE.GetSDLRenderer(), surf);
	if (!tex)
	{
		printf("�T�[�t�F�X����e�N�X�`���̍쐬�Ɏ��s : %s", fileName.c_str());
		return false;
	}

	// �摜�̕��A�������擾
	mWidth = surf->w;
	mHeight = surf->h;
	channels = surf->format->BytesPerPixel;
	
	// OpenGL�Ƀe�N�X�`���o�^
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, surf->pixels);

	// SDL�T�[�t�F�X�͗p�ς݂Ȃ̂ŉ��
	SDL_FreeSurface(surf);

	// �e�N�X�`���t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;

	// Generate a GL texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, surface->pixels);

	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
