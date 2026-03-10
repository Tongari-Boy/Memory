#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "Effect.h"
#include "Pool.h"

//==============
//定数
//==============
const int SCREEN_W = 800;
const int SCREEN_H = 600;


//AABB衝突判定
bool checkHit(float ax, float ay, int aw, int ah,
	float bx, float by, int bw, int bh) {
	return ax < bx + bw && ax + aw > bx &&
		ay < by + bh && ay + ah > by;
}


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
	//攻撃プールの生成
	Pool<Attack, 4> attackPool;
	//エフェクトプールの生成
	Pool<Effect, 32> effectPool;
	int score = 0;

	//画面4隅にランダムでスポーン
	float spawnPoints[4][2] = {
	{50, 50}, {750, 50}, {50, 550}, {750, 550}
	};
	for (int i = 0; i < 4; i++) {
		Enemy* e = enemyPool.alloc();
		if (e) e->init(spawnPoints[i][0], spawnPoints[i][1]);
	}

	//攻撃
	int  direction = 1;   //最後に向いた方向
	bool spaceWas = false; //前フレームのスペース状態

	bool running = true;
	SDL_Event event;

	while (running) {
		//イベント
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)running = false;

		//Update処理
		
		//プレイヤの更新
		const Uint8* keys = SDL_GetKeyboardState(nullptr);

		//向き更新
		if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    direction = 0;
		if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  direction = 1;
		if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  direction = 2;
		if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) direction = 3;

		player.update(keys);

		//スペースキーで攻撃（押した瞬間のみ）
		bool spaceNow = keys[SDL_SCANCODE_SPACE];
		if (spaceNow && !spaceWas) {
			Attack* a = attackPool.alloc();
			if (a) a->init(player.x, player.y, direction);
		}
		spaceWas = spaceNow;

		//攻撃更新
		int i = 0;
		while (i < attackPool.activeCount_) {
			Attack& a = attackPool.pool[attackPool.activeList[i]];
			a.update();
			if (a.lifetime <= 0) {
				attackPool.free(&a);
			}
			else {
				i++;
			}
		}

		//敵の更新
		for (int i = 0; i < enemyPool.activeCount_; i++) {
			Enemy& e = enemyPool.pool[enemyPool.activeList[i]];
			e.update(player.x, player.y);
		}

		//衝突判定:攻撃と敵
		for (int a = 0; a < attackPool.activeCount_; a++) {
			Attack& atk = attackPool.pool[attackPool.activeList[a]];
			int j = 0;
			while (j < enemyPool.activeCount_) {
				Enemy& e = enemyPool.pool[enemyPool.activeList[j]];
				if (checkHit(atk.x, atk.y, atk.w, atk.h,
					e.x, e.y, 20, 20)) {
					e.hp--;

					//ヒットエフェクトを4方向に生成
					float cx = e.x + 10;
					float cy = e.y + 10;
					float dirs[4][2] = { {2,0},{-2,0},{0,2},{0,-2} };
					for (int d = 0; d < 4; d++) {
						Effect* ef = effectPool.alloc();
						if (ef) ef->init(cx, cy, dirs[d][0], dirs[d][1]);
					}

					if (e.hp <= 0)
						enemyPool.free(&e);
					else
						j++;
				}
				else {
					j++;
				}
			}
		}

		//エフェクトの更新
		int k = 0;
		while (k < effectPool.activeCount_) {
			Effect& ef = effectPool.pool[effectPool.activeList[k]];
			ef.update();
			if (ef.lifetime <= 0)
				effectPool.free(&ef);
			else
				k++;
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

		//攻撃の描画
		for (int a = 0; a < attackPool.activeCount_; a++) {
			Attack& atk = attackPool.pool[attackPool.activeList[a]];
			atk.render(renderer);
		}

		//エフェクト描画
		for (int k = 0; k < effectPool.activeCount_; k++) {
			Effect& ef = effectPool.pool[effectPool.activeList[k]];
			ef.render(renderer);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}