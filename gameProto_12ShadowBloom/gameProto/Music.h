#pragma once
#include <SDL/SDL_mixer.h>
#include <iostream>

class Music
{
public:
	            Music();
	           ~Music();
	void        Play();                                 // ミュージック再生
	void        FadeInMusic(int fadeInTime);            // ミュージックフェードイン
	bool        LoadMusic(const std::string& fileName); // ミュージック読み込み

	static void Stop();                                 // 再生ストップ
	static void FadeOutMusic(int fadeOutTime);          // 再生中のミュージックをフェードアウト
	static bool IsPlaying();                            // 現在ミュージックは再生中か？

private:
	Mix_Music*  mMusic;                                 // 音源データ
}; 