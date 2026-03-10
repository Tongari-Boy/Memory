#include <SDL.h>
#include "Player.h"

//==============
//定数
//==============
const int SCREEN_W = 800;
const int SCREEN_H = 600;


//=============
//メイン
//=============
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"action",
		SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN
	);
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED
	);

	//Playerの生成
	Player player;
	player.init(SCREEN_W / 2.0f, SCREEN_H / 2.0f);

	bool running = true;
	SDL_Event event;

	while (running) {
		//イベント
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)running = false;

		//Update処理
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		player.update(keys);

		//描画
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderClear(renderer);


		//Playerの描画
		player.render(renderer);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}