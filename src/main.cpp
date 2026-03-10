#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "Pool.h"

//==============
//定数
//==============
const int SCREEN_W = 800;
const int SCREEN_H = 600;


//=============
//メイン
//=============
int main(int argc, char* argv[]) {
	srand((unsigned)time(nullptr));

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

	//敵プールの生成
	Pool<Enemy, 5> enemyPool;

	//画面4隅にランダムでスポーン
	float spawnPoints[4][2] = {
	{50, 50}, {750, 50}, {50, 550}, {750, 550}
	};
	for (int i = 0; i < 4; i++) {
		Enemy* e = enemyPool.alloc();
		if (e) e->init(spawnPoints[i][0], spawnPoints[i][1]);
	}

	bool running = true;
	SDL_Event event;

	while (running) {
		//イベント
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)running = false;

		//Update処理
		
		//プレイヤの更新
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		player.update(keys);

		//敵の更新
		for (int i = 0; i < enemyPool.activeCount_; i++) {
			Enemy& e = enemyPool.pool[enemyPool.activeList[i]];
			e.update(player.x, player.y);
		}

		//描画
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderClear(renderer);

		//プレイヤの描画
		player.render(renderer);

		//敵の描画
		for (int i = 0; i < enemyPool.activeCount_; i++) {
			Enemy& e = enemyPool.pool[enemyPool.activeList[i]];
			e.render(renderer);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}