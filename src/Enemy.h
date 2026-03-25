#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Map;

//敵タイプ
enum EnemyType
{
	ENEMY_NORMAL = 0,
	ENEMY_RUSH = 1,
	ENEMY_LARGE = 2
};

struct Enemy : public GameObject {
	float x, y;
	float speed;
	int	  hp;
	int	  size;
	EnemyType type;
	bool isRushing;
	float knockbackX;     //反発速度X
	float knockbackY;     //反発速度Y
	int   knockbackTimer; //反発時間

	void init(float startX, float startY,EnemyType t = ENEMY_NORMAL);
	void update(float playerX, float playerY,const Map* map);
	void render(SDL_Renderer* r);
	void applyKnockback(float dx, float dy);
};