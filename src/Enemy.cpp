#include "Enemy.h"
#include <cmath>
#include "Map.h"

void Enemy::init(float startX, float startY,EnemyType t) {
	x = startX;
	y = startY;
    type = t;
    isRushing = false;

    knockbackX = 0;
    knockbackY = 0;
    knockbackTimer = 0;

	active = true;

    switch (type) {
    case ENEMY_NORMAL:
        speed = 1.5f;
        hp = 2;
        size = 20;
        break;
    case ENEMY_RUSH:
        speed = 1.0f;
        hp = 1;
        size = 20;
        break;
    case ENEMY_LARGE:
        speed = 0.8f;
        hp = 4;
        size = 20;
        break;
    } 
}

void Enemy::update(float playerX, float playerY,const Map* map) {
    //ノックバック中は通常AIを無効化
    if (knockbackTimer > 0) {

        knockbackTimer--;

        float nx = x + knockbackX;
        float ny = y + knockbackY;
        knockbackX *= 0.8f;
        knockbackY *= 0.8f;

        //X方向の壁判定
        if (map) {
            int leftCol = (int)(nx) / Map::TILE_SIZE;
            int rightCol = (int)(nx + size -1) / Map::TILE_SIZE;
            int topRow = (int)(y) / Map::TILE_SIZE;
            int botRow = (int)(y + size - 1) / Map::TILE_SIZE;

            if (!map->isWall(leftCol, topRow) &&
                !map->isWall(rightCol, topRow) &&
                !map->isWall(leftCol, botRow) &&
                !map->isWall(rightCol, botRow)) {
                x = nx;
            }
            else {
                knockbackX = 0; //壁に当たったら反発停止
            }
        }
        
        //Y方向の壁判定
        if (map) {
            int leftCol = (int)(x) / Map::TILE_SIZE;
            int rightCol = (int)(x + size - 1) / Map::TILE_SIZE;
            int topRow = (int)(ny) / Map::TILE_SIZE;
            int botRow = (int)(ny + size - 1) / Map::TILE_SIZE;

            if (!map->isWall(leftCol, topRow) &&
                !map->isWall(rightCol, topRow) &&
                !map->isWall(leftCol, botRow) &&
                !map->isWall(rightCol, botRow)) {
                y = ny;
            }
            else {
                knockbackY = 0; //壁に当たったら反発停止
            }
        }
        else {
            x = nx;
            y = ny;
        }
        return;
    }

    //プレイヤー方向への単位ベクトルを計算
    float dx = playerX - x;
    float dy = playerY - y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len == 0) return;

    float currentSpeed = speed;

    //突進タイプの処理(ENEMY_RUSH)
    if (type == ENEMY_RUSH) {
        if (len < 150.0f)    isRushing = true;  //150px以内に入ったら突進        
        if (len > 200.0f)    isRushing = false; //離れたら突進解除
        if (isRushing) currentSpeed = 4.0f;     //突進速度
    }

    float nx = x + (dx / len) * currentSpeed;
    float ny = y + (dy / len) * currentSpeed;

    //衝突判定
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
    switch (type)
    {
    case ENEMY_NORMAL:
        SDL_SetRenderDrawColor(r, 255, 80, 80, 255);//赤
        break;
    case ENEMY_RUSH:
        SDL_SetRenderDrawColor(r, 255, 160, 0, 255);//橙
        break;
    case ENEMY_LARGE:
        SDL_SetRenderDrawColor(r, 180, 0, 255, 255);//紫
        break;
    default:
        break;
    }
    SDL_Rect rect = { (int)x, (int)y, 20, 20 };
    SDL_RenderFillRect(r, &rect);
}

void Enemy::applyKnockback(float dx, float dy) {
    float len = std::sqrt(dx * dx + dy * dy);
    if (len == 0) return;
    knockbackX = (dx / len) * 6.0f;    //反発速度
    knockbackY = (dy / len) * 6.0f;
    knockbackTimer = 10;               //10フレーム反発
}