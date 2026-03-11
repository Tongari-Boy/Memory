#pragma once
#include <SDL.h>
#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "Effect.h"
#include "Pool.h"
#include "UI.h"

class Game {
public:

	//===========
	//フィールド
	//===========

	//定数
	static const int SCREEN_W = 800;
	static const int SCREEN_H = 600;

	//ゲームオブジェクト
	Player player;

	//プール
	Pool<Enemy, 5> enemyPool;
	Pool<Attack, 4> attackPool;
	Pool<Effect, 32> effectPool;

	//UI
	UI ui;

	//ゲーム状態
	int score;
	int direction;	//最後に向いた方向
	bool spaceWas;	//前フレームのスペース状態


	//==========
	//メソッド
	//==========

	//初期化
	void init();
	//更新
	void update(const Uint8* keys);
	//描画
	void render(SDL_Renderer* r);

private:
	void updateAttacks();
	void updateEnemies();
	void updateEffects();
	void checkCollisions();

	static bool checkHit(float ax, float ay, int aw, int ah,
		float bx, float by, int bw, int bh);
};