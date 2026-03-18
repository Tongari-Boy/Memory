#include "Game.h"
#include <cstdlib>

//ステージデータの定義
const StageData Game::STAGES[Game::MAX_STAGE] = {
	{4,1.5f},
	{4,2.0f},
	{5,2.5f},
};

void Game::init() {

	//スタックアロケータを初期化(4KB確保)
	stackAlloc.init(4096); 
	//ダブルバッファアロケータを初期化(1KB確保)
	frameAlloc.init(1024);

	damageTexts = nullptr;
	damageTextCount = 0;

	ui.init("arial.ttf", 24);

	score = 0;
	direction = 1;
	spaceWas = false;
	currentStage = 0;

	//Playerの生成
	player.init(SCREEN_W / 2.0f, SCREEN_H / 2.0f);

	//ステージのロード、敵のスポーン
	loadStage(currentStage);
}

void Game::update(const Uint8* keys) {
	//向き更新
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    direction = 0;
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  direction = 1;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  direction = 2;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) direction = 3;

	player.update(keys,&map);

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
		e.update(player.x, player.y,&map);
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
				
				//ダメージテキストをスポーン
				spawnDamageText(e.x + 10, e.y, 1);

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

	//マップの描画
	map.render(renderer);

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

	//ダメージテキスト描画
	for (int i = 0; i < damageTextCount; i++) {
		DamageText& dt = damageTexts[i];
		SDL_Color red = { 255,50,50,255 };
		ui.drawText(renderer, "-" + std::to_string(dt.value),
			(int)dt.x, (int)dt.y, red);
	}

	//メモリバジェット用
	renderMemoryDebug(renderer);

	SDL_RenderPresent(renderer);
}

//メモリバジェット用
void Game::renderMemoryDebug(SDL_Renderer* r) {
	int x = 550; //画面右側に表示
	int y = 10;
	int lineH = 40;

	SDL_Color cyan = { 0, 255, 255, 255 };
	ui.drawText(r, "-- MEMORY BUDGET --", x, y, cyan);
	y += 20;

	//各プールの使用量を表示
	ui.drawMemoryBar(r, "Enemy ",
		enemyPool.usedBytes(), enemyPool.totalBytes(), x, y);
	y += lineH;

	ui.drawMemoryBar(r, "Attack",
		attackPool.usedBytes(), attackPool.totalBytes(), x, y);
	y += lineH;

	ui.drawMemoryBar(r, "Effect",
		effectPool.usedBytes(), effectPool.totalBytes(), x, y);
	y += lineH;

	//合計
	size_t totalUsed = enemyPool.usedBytes() +
		attackPool.usedBytes() +
		effectPool.usedBytes();
	size_t totalAll = enemyPool.totalBytes() +
		attackPool.totalBytes() +
		effectPool.totalBytes();

	SDL_Color yellow = { 255, 255, 0, 255 };
	ui.drawText(r,
		"TOTAL: " + std::to_string(totalUsed) +
		" / " + std::to_string(totalAll) + " bytes",
		x, y, yellow);

	ui.drawMemoryBar(r, "Frame ",
		frameAlloc.usedBytes(), frameAlloc.totalBytes(), x, y);
	y += lineH;
}

//AABB衝突判定
bool Game::checkHit(float ax, float ay, int aw, int ah,
	float bx, float by, int bw, int bh) {
	return ax < bx + bw && ax + aw > bx &&
		ay < by + bh && ay + ah > by;
}

//ステージのロード
void Game::loadStage(int stageIndex) {
	//前のステージのマップメモリを解放
	stackAlloc.freeToMarker(stageMarker);

	//現在位置をマーカーとして保存
	stageMarker = stackAlloc.getMarker();

	//マップをスタックアロケータから確保
	map.load(stackAlloc, stageIndex);

	//既存の敵をクリア
	while (enemyPool.activeCount_ > 0) {
		enemyPool.free(&enemyPool.pool[enemyPool.activeList[0]]);
	}

	const StageData& stage = STAGES[stageIndex];
	
	//床タイルの取得
	int floorCols[130], floorRows[130], floorCount = 0;
	map.getFloorTiles(floorCols, floorRows, floorCount, 130);

	//敵タイプをステージに応じて変える
	EnemyType types[5] = {
		ENEMY_NORMAL, ENEMY_NORMAL,
		ENEMY_RUSH,
		ENEMY_LARGE,
		ENEMY_RUSH
	};

	for (int i = 0; i < stage.enemyCount; i++) {
		Enemy* e = enemyPool.alloc();
		if (!e) break;

		//プレイヤーから離れた床タイルを探す
		int attempts = 0;
		int col, row;
		do {
			int idx = rand() % floorCount;
			col = floorCols[idx];
			row = floorRows[idx];
			attempts++;
		} while (attempts < 20 &&
			std::abs(col * Map::TILE_SIZE - player.x) < 200 &&
			std::abs(row * Map::TILE_SIZE - player.y) < 200);

		e->init(
			col * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f,
			row * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f,
			types[i % 5]
		);
		e->speed *= stage.enemySpeed / 1.5f;
	}
}


void Game::spawnDamageText(float x, float y, int value) {
	DamageText* dt = (DamageText*)frameAlloc.alloc(sizeof(DamageText));
	if (!dt) {
		std::cout << "alloc failed!" << std::endl;
		return;
	}
	dt->x = x;
	dt->y = y;
	dt->vy = -1.5f;
	dt->value = value;
	dt->lifetime = 40;

	//先頭ポインタを更新
	if (damageTextCount == 0)
		damageTexts = dt;

	damageTextCount++;
}

//シャットダウン
void Game::shutdown() {
	frameAlloc.shutdown();
	stackAlloc.shutdown();
	ui.shutdown();
}