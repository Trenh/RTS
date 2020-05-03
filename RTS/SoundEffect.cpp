#include "SoundEffect.h"


SoundEffect::SoundEffect(const char* pFilename)
	:filename(pFilename), sound(nullptr)
{
}

SoundEffect::SoundEffect()
{
}


SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(sound);
}

void SoundEffect::load() {
	sound = Mix_LoadWAV(filename);
}

void SoundEffect::play() {
	Mix_PlayChannel(-1, sound, 0);
}
