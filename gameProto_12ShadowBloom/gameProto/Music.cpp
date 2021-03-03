#include "Music.h"

Music::Music()
	:mMusic(nullptr)
{
}

Music::~Music()
{
	Mix_FreeMusic(mMusic);
}

// �����Đ�
void Music::Play()
{
	if (mMusic)
	{
		Mix_PlayMusic(mMusic, -1); // -1�̓��[�v�Đ��ݒ�
	}
}

// ������fadeInTime���Ԃ����ăt�F�[�h�C���Đ�
void Music::FadeInMusic(int fadeInTime)
{
	if (mMusic)
	{
		Mix_FadeInMusic(mMusic, -1, fadeInTime);
	}
}

// ������ǂݍ���
bool Music::LoadMusic(const std::string& fileName)
{
	mMusic = Mix_LoadMUS(fileName.c_str());
	return mMusic != nullptr;
}

// ������~
void Music::Stop()
{
	Mix_HaltMusic();
}

// ������fadeOutTime�����ăt�F�[�h�A�E�g
void Music::FadeOutMusic(int fadeOutTime)
{
	Mix_FadeOutMusic(fadeOutTime);
}

// �����͍Đ������H
bool Music::IsPlaying()
{
	return Mix_PlayingMusic();
}
