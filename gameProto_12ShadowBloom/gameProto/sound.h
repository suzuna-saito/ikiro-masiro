#pragma once
#include <SDL/SDL_mixer.h>
#include <iostream>

class Sound
{

public:
	Sound();
	~Sound();
	void          Play();       // 再生
	void          Stop();       // ストップ
	bool          LoadSound(const std::string& fileName); // サウンドファイル読み込み
	bool          IsPlaying();  // 現在再生中か？

private:
	Mix_Chunk*    mChunk;        // サウンドデータ
	int           mChannel;      // 再生チャンネル
};
