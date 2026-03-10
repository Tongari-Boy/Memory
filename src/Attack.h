#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Attack :public GameObject {
	float x, y;
	int   w, h;
	int   lifetime;	//•\¦ƒtƒŒ[ƒ€”

	void init(float px, float py, int direction);
	void update();
	void render(SDL_Renderer* r);
};