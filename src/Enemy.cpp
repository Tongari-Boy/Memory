#include "Enemy.h"
#include <cmath>

void Enemy::init(float startX, float startY) {
	x = startX;
	y = startY;
	speed = 1.5f;
	hp = 2;
	active = true;
}

void Enemy::update(float playerX, float playerY) {
    //プレイヤー方向への単位ベクトルを計算
    float dx = playerX - x;
    float dy = playerY - y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len > 0) {
        x += (dx / len) * speed;
        y += (dy / len) * speed;
    }
}

void Enemy::render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 255, 80, 80, 255);
    SDL_Rect rect = { (int)x, (int)y, 20, 20 };
    SDL_RenderFillRect(r, &rect);
}