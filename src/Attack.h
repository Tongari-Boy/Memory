#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Attack :public GameObject {
	float x, y;
	int   w, h;
	int   lifetime;	//表示フレーム数

	bool hasHit;	//ヒット済みフラグ

	void init(float px, float py, int direction);
	void update();
	void render(SDL_Renderer* r);
};