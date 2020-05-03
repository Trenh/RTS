#pragma once
#include <SDL_mixer.h>

class SoundEffect
{
public:
	SoundEffect(const char* pFilename);
	SoundEffect();
	~SoundEffect();

	void load();
	void play();
	void destroy();

private:
	const char* filename;
	Mix_Chunk* sound;
};

