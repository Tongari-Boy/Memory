#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Effect :public GameObject {
	float x, y;
	float vx, vy;
	int lifetime;
	int maxLifetime;

	void init(float startX, float startY, float velX, float velY);
	void update();
	void render(SDL_Renderer* r);
};