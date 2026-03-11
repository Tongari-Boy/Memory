#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UI {
public:
	TTF_Font* font;

	bool init(const char* fontPath, int fontSize);
	void drawText(SDL_Renderer* r, const std::string& text,
				  int x, int y, SDL_Color color);
	void drawHP(SDL_Renderer* r, int hp, int maxHp);
	void drawScore(SDL_Renderer* r, int score);
	void shutdown();
};
