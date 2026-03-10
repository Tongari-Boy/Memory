#include "Effect.h"

void Effect::init(float startX, float startY, float velX, float velY) {
    x = startX;
    y = startY;
    vx = velX;
    vy = velY;
    lifetime = 20;
    maxLifetime = 20;
    active = true;
}

void Effect::update() {
    if (!active) return;
    x += vx;
    y += vy;
    lifetime--;
}

void Effect::render(SDL_Renderer* r) {
    if (!active) return;

    //lifetimeに応じて明るさを変える（フェードアウト）
    float ratio = (float)lifetime / (float)maxLifetime;
    Uint8 alpha = (Uint8)(255 * ratio);

    SDL_SetRenderDrawColor(r, 255, 200, 0, alpha);
    SDL_Rect rect = { (int)x, (int)y, 6, 6 };
    SDL_RenderFillRect(r, &rect);
}