#include "Game.h"
#include <cstdlib>

//ステージデータの定義
const StageData Game::STAGES[Game::MAX_STAGE] = {
	{4,1.5f},
	{4,2.0f},
	{5,2.5f},
};


void Game::init() {

	ui.init("arial.ttf", 24);

	score = 0;
	direction = 1;
	spaceWas = false;
	currentStage = 0;

	//Playerの生成
	player.init(SCREEN_W / 2.0f, SCREEN_H / 2.0f);
	loadStage(currentStage);
	
	//画面4隅に敵をランダムでスポーン
	float spawnPoints[4][2] = {
	{50, 50}, {750, 50}, {50, 550}, {750, 550}
	};
	for (int i = 0; i < 4; i++) {
		Enemy* e = enemyPool.alloc();
		if (e) e->init(spawnPoints[i][0], spawnPoints[i][1]);
	}
}

void Game::update(const Uint8* keys) {
	//向き更新
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    direction = 0;
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  direction = 1;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  direction = 2;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) direction = 3;

	player.update(keys);

	//攻撃生成
	bool spaceNow = keys[SDL_SCANCODE_SPACE];
	if (spaceNow && !spaceWas) {
		Attack* a = attackPool.alloc();
		if (a) a->init(player.x, player.y, direction);
	}
	spaceWas = spaceNow;

	updateAttacks();
	updateEnemies();
	checkCollisions();
	updateEffects();

	//全敵を倒したら次のステージへ
	if (enemyPool.activeCount_ == 0) {
		currentStage++;
		if (currentStage >= MAX_STAGE)
			currentStage = 0;	//全ステージクリアでループ
		loadStage(currentStage);
	}
}

//攻撃更新
void Game::updateAttacks() {
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
}

//敵更新
void Game::updateEnemies() {
	for (int i = 0; i < enemyPool.activeCount_; i++) {
		Enemy& e = enemyPool.pool[enemyPool.activeList[i]];
		e.update(player.x, player.y);
	}
}

//エフェクト更新
void Game::updateEffects() {
	int k = 0;
	while (k < effectPool.activeCount_) {
		Effect& ef = effectPool.pool[effectPool.activeList[k]];
		ef.update();
		if (ef.lifetime <= 0)
			effectPool.free(&ef);
		else
			k++;
	}
}

//衝突判定(攻撃と敵)
void Game::checkCollisions() {
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

				if (e.hp <= 0) {
					score += 100;
					enemyPool.free(&e);
				}else {
					j++;
				}
			}
			else {
				j++;
			}
		}
	}
}

//描画
void Game::render(SDL_Renderer* renderer) {
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

	//UI描画
	ui.drawHP(renderer, player.hp, 3);
	ui.drawScore(renderer, score);
	//ステージ番号を描画
	ui.drawText(renderer,
		"STAGE:" + std::to_string(currentStage + 1),
		10, 70,
		{ 255,255,255,255 }
	);

	SDL_RenderPresent(renderer);
}

//AABB衝突判定
bool Game::checkHit(float ax, float ay, int aw, int ah,
	float bx, float by, int bw, int bh) {
	return ax < bx + bw && ax + aw > bx &&
		ay < by + bh && ay + ah > by;
}

//ステージのロード
void Game::loadStage(int stageIndex) {
	//既存の敵をクリア
	while (enemyPool.activeCount_ > 0) {
		enemyPool.free(&enemyPool.pool[enemyPool.activeList[0]]);
	}

	const StageData& stage = STAGES[stageIndex];
	
	//スポーン位置(最大5箇所)
	float spawnPoints[5][2] = {
		{50,50},{750,50},
		{50,550},{750,550},
		{400,50}
	};

	for (int i = 0; i < stage.enemyCount; i++) {
		Enemy* e = enemyPool.alloc();
		if (e) {
			e->init(spawnPoints[i][0], spawnPoints[i][1]);
			e->speed = stage.enemySpeed;	//速度を上書き
		}
	}
}