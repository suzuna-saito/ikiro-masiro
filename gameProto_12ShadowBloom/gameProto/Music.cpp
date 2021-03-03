#include "Music.h"

Music::Music()
	:mMusic(nullptr)
{
}

Music::~Music()
{
	Mix_FreeMusic(mMusic);
}

// 音源再生
void Music::Play()
{
	if (mMusic)
	{
		Mix_PlayMusic(mMusic, -1); // -1はループ再生設定
	}
}

// 音源をfadeInTime時間かけてフェードイン再生
void Music::FadeInMusic(int fadeInTime)
{
	if (mMusic)
	{
		Mix_FadeInMusic(mMusic, -1, fadeInTime);
	}
}

// 音源を読み込み
bool Music::LoadMusic(const std::string& fileName)
{
	mMusic = Mix_LoadMUS(fileName.c_str());
	return mMusic != nullptr;
}

// 音源停止
void Music::Stop()
{
	Mix_HaltMusic();
}

// 音源をfadeOutTimeかけてフェードアウト
void Music::FadeOutMusic(int fadeOutTime)
{
	Mix_FadeOutMusic(fadeOutTime);
}

// 音源は再生中か？
bool Music::IsPlaying()
{
	return Mix_PlayingMusic();
}
