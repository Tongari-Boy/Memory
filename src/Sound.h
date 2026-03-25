#pragma once
#include <SDL_mixer.h>
#include <iostream>

class Sound {
public:
	Mix_Chunk* hitSE;
	Mix_Chunk* dieSE;

	bool init() {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
			std::cout << "MIX_OpenAudioError: "
				<< Mix_GetError() << std::endl;
			return false;
		}

		hitSE = Mix_LoadWAV("hit.wav");
		if (!hitSE)
			std::cout << "hit.wav Error: " << Mix_GetError() << std::endl;

		dieSE = Mix_LoadWAV("enemy_die.wav");
		if (!dieSE)
			std::cout << "enemy_die.wav Error: " << Mix_GetError() << std::endl;

		return true;
	}

	void playHit() {
		if (hitSE) Mix_PlayChannel(-1, hitSE, 0);
	}

	void playDie() {
		if (dieSE) Mix_PlayChannel(-1, dieSE, 0);
	}

	void shutdown() {
		if (hitSE) Mix_FreeChunk(hitSE);
		if (dieSE) Mix_FreeChunk(dieSE);
	}
};