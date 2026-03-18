#pragma once
#include <SDL.h>
#include <cmath>
#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "Effect.h"
#include "Pool.h"
#include "UI.h"
#include "Map.h"
#include "StackAllocator.h"
#include "DamageText.h"
#include "DoubleBufferAllocator.h"


//ステージデータ
struct StageData {
	int enemyCount;
	float enemySpeed;
};

class Game {
public:

	//===========
	//フィールド
	//===========

	//定数
	static const int SCREEN_W = 800;
	static const int SCREEN_H = 600;
	static const int MAX_STAGE = 3;	//ステージ数

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
	int currentStage;

	Map            map;
	StackAllocator stackAlloc;
	size_t         stageMarker;   //ステージ開始時のマーカー

	DoubleBufferAllocator frameAlloc;
	DamageText* damageTexts;	//現フレームのダメージ表示
	int damageTextCount;

	//==========
	//メソッド
	//==========

	//初期化
	void init();
	//更新
	void update(const Uint8* keys);
	//描画
	void render(SDL_Renderer* r);
	//メモリバジェット用
	void renderMemoryDebug(SDL_Renderer* r);
	//シャットダウン
	void shutdown();


private:
	void updateAttacks();
	void updateEnemies();
	void updateEffects();
	void checkCollisions();
	void loadStage(int stageIndex);
	void spawnDamageText(float x, float y, int value);

	static bool checkHit(float ax, float ay, int aw, int ah,
		float bx, float by, int bw, int bh);

	//ステージデータ一覧
	static const StageData STAGES[MAX_STAGE];
};