#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "Game.h"

//=============
//メイン
//=============
int main(int argc, char* argv[]) {
	srand((unsigned)time(nullptr));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"action",
		SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		Game::SCREEN_W,Game::SCREEN_H,SDL_WINDOW_SHOWN
	);
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED
	);

	//Gameの生成
	Game game;
	game.init(); 

	bool running = true;
	SDL_Event event;

	while (running) {
		//イベント
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)running = false;

		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		game.update(keys);
		game.render(renderer);

		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}