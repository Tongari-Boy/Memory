#include "UI.h"
#include <iostream>

bool UI::init(const char* fontPath, int fontSize) {
	if (TTF_Init() != 0) {
		std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
		return false;
	}
	font = TTF_OpenFont(fontPath, fontSize);
	if (!font) {
		std::cout << "Font Error:" << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}

void UI::drawText(SDL_Renderer* r, const std::string& text,
				  int x,int y,SDL_Color color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) return;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(r,surface);
	SDL_FreeSurface(surface);
	if (!texture) return;

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	SDL_Rect rect = { x,y,w,h };
	SDL_RenderCopy(r, texture, nullptr, &rect);
	SDL_DestroyTexture(texture);
}

void UI::drawHP(SDL_Renderer* r, int hp, int maxHp) {
	SDL_Color white = { 255,255,255,255 };
	std::string text = "HP:";
	for (int i = 0; i < maxHp; i++) {
		text += (i < hp) ? "[*]" : "[ ]";
	}
	drawText(r, text, 10, 10, white);
}

void UI:: drawScore(SDL_Renderer* r, int score) {
	SDL_Color yellow = { 255,255,0,255 };
	std::string text = "SCORE:" + std::to_string(score);
	drawText(r, text, 10, 40, yellow);
}

void UI::shutdown() {
	if (font) TTF_CloseFont(font);
	TTF_Quit();
}