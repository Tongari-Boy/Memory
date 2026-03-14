#include "Enemy.h"
#include <cmath>
#include "Map.h"

void Enemy::init(float startX, float startY) {
	x = startX;
	y = startY;
	speed = 1.5f;
	hp = 5;
	active = true;
}

void Enemy::update(float playerX, float playerY,const Map* map) {
    //プレイヤー方向への単位ベクトルを計算
    float dx = playerX - x;
    float dy = playerY - y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len == 0) return;

    float nx = x + (dx / len) * speed;
    float ny = y + (dy / len) * speed;

    int size = 20;

    //X方向の判定
    if (map) {
        int leftCol = (int)(nx) / Map::TILE_SIZE;
        int rightCol = (int)(nx + size - 1) / Map::TILE_SIZE;
        int topRow = (int)(y) / Map::TILE_SIZE;
        int botRow = (int)(y + size - 1) / Map::TILE_SIZE;

        if (!map->isWall(leftCol, topRow) &&
            !map->isWall(rightCol, topRow) &&
            !map->isWall(leftCol, botRow) &&
            !map->isWall(rightCol, botRow)) {
            x = nx;
        }

        //Y方向の判定
        leftCol = (int)(x) / Map::TILE_SIZE;
        rightCol = (int)(x + size - 1) / Map::TILE_SIZE;
        topRow = (int)(ny) / Map::TILE_SIZE;
        botRow = (int)(ny + size - 1) / Map::TILE_SIZE;

        if (!map->isWall(leftCol, topRow) &&
            !map->isWall(rightCol, topRow) &&
            !map->isWall(leftCol, botRow) &&
            !map->isWall(rightCol, botRow)) {
            y = ny;
        }
    }
    else {
        x = nx;
        y = ny;
    }
}

void Enemy::render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 255, 80, 80, 255);
    SDL_Rect rect = { (int)x, (int)y, 20, 20 };
    SDL_RenderFillRect(r, &rect);
}