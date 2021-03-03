#include "AudioManager.h"
#include "Sound.h"
#include "music.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	if (mSounds.size() > 0)
	{
		ShutDown();
	}
}

// �T�E���h�擾
Sound* AudioManager::GetSound(const std::string& fileName)
{
	Sound* sound = nullptr;
	auto iter = mSounds.find(fileName);
	// �T�E���h�t�@�C���o�^����Ă��邩�H
	if (iter != mSounds.end())
	{
		sound = iter->second;
	}
	else
	{
		// �o�^�Ȃ�������V�K�T�E���h�ǂݍ���
		sound = new Sound();
		if (sound->LoadSound(fileName))
		{
			mSounds.emplace(fileName, sound);
		}
		else
		{
			printf("%s �ǂݍ��ݎ��s\n", fileName.c_str());
			delete sound;
			sound = nullptr;
		}
	}
	return sound;
}

void AudioManager::RemoveSound(const std::string& fileName)
{
	// �T�E���h�t�@�C�����폜
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		printf("release: %s\n", iter->first.c_str());
		delete iter->second;
		mSounds.erase(iter);
	}
}

void AudioManager::PlaySound(const std::string& fileName)
{
	//�T�E���h�炷
	mSounds[fileName]->Play();
}

void AudioManager::StopSound(const std::string& fileName)
{
	mSounds[fileName]->Stop();
}

bool AudioManager::IsPlayingSound(const std::string& fileName)
{
	return mSounds[fileName]->IsPlaying();
}

Music* AudioManager::GetMusic(const std::string& fileName)
{
	Music* music = nullptr;
	auto iter = mMusics.find(fileName);

	if (iter != mMusics.end())
	{
		music = iter->second;
	}
	else
	{
		music = new Music();
		if (music->LoadMusic(fileName))
		{
			mMusics.emplace(fileName, music);
		}
		else
		{
			printf("%s �ǂݍ��ݎ��s\n", fileName.c_str());
			delete music;
			music = nullptr;
		}
	}
	return music;
}

void AudioManager::RemoveMusic(const std::string& fileName)
{
	auto iter = mMusics.find(fileName);
	if (iter != mMusics.end())
	{
		printf("release : %s\n", iter->first.c_str());
		delete iter->second;

		mMusics.erase(iter);
	}
}

void AudioManager::PlayFadeInMusic(const std::string& fileName, int fadeInTime)
{
	mMusics[fileName]->FadeInMusic(fadeInTime);
}

void AudioManager::PlayMusic(const std::string& fileName)
{
	mMusics[fileName]->Play();
}

void AudioManager::FadeOutMusic(int fadeOutTime)
{
	Music::FadeOutMusic(fadeOutTime);
}

void AudioManager::StopMusic()
{
	Music::Stop();
}

bool AudioManager::IsPlayingMusic()
{
	return Music::IsPlaying();
}

void AudioManager::ShutDown()
{
	//�T�E���h�j��
	for (auto i : mSounds)
	{
		printf("release : %s\n", i.first.c_str());
		delete i.second;
	}
	//�~���[�W�b�N�j��
	for (auto i : mMusics)
	{
		printf("release : %s\n", i.first.c_str());
		delete i.second;
	}

	mSounds.clear();
	mMusics.clear();
}

