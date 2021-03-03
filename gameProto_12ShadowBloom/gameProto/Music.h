#pragma once
#include <SDL/SDL_mixer.h>
#include <iostream>

class Music
{
public:
	            Music();
	           ~Music();
	void        Play();                                 // �~���[�W�b�N�Đ�
	void        FadeInMusic(int fadeInTime);            // �~���[�W�b�N�t�F�[�h�C��
	bool        LoadMusic(const std::string& fileName); // �~���[�W�b�N�ǂݍ���

	static void Stop();                                 // �Đ��X�g�b�v
	static void FadeOutMusic(int fadeOutTime);          // �Đ����̃~���[�W�b�N���t�F�[�h�A�E�g
	static bool IsPlaying();                            // ���݃~���[�W�b�N�͍Đ������H

private:
	Mix_Music*  mMusic;                                 // �����f�[�^
}; 