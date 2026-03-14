#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Map;

struct Enemy : public GameObject {
	float x, y;
	float speed;
	int	  hp;

	void init(float startX, float startY);
	void update(float playerX, float playerY,const Map* map);
	void render(SDL_Renderer* r);
};