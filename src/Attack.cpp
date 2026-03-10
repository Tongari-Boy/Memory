#include "Attack.h"

//direction: 0=è„ 1=â∫ 2=ç∂ 3=âE
void Attack::init(float px, float py, int direction) {
	w = 20;
	h = 20;
	lifetime = 10;
	active = true;

	switch (direction)
	{
	case 0:x = px; y = py - 24; break;	//è„
	case 1:x = px; y = py + 24; break;	//â∫
	case 2:x = px - 24; y = py; break;	//ç∂
	case 3:x = px + 24; y = py; break;	//âE

	}
}

void Attack::update() {
	if (!active) return;
	lifetime--;
}

void Attack::render(SDL_Renderer* r) {
	if (!active) return;
	SDL_SetRenderDrawColor(r, 255, 255, 100, 255); //â©êFÇ≈ï`âÊ
	SDL_Rect rect = { (int)x, (int)y, w, h };
	SDL_RenderFillRect(r, &rect);
}