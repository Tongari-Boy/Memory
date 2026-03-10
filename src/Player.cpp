#include "Player.h"


//èâä˙âª
void Player::init(float startX, float startY) {
    x = startX;
    y = startY;
    speed = 3.0f;
    hp = 3;
    active = true;
}

//ì¸óÕèàóù
void Player::update(const Uint8* keys) {
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    y -= speed;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  y += speed;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  x -= speed;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) x += speed;

    if (x < 0)   x = 0;
    if (x > 780) x = 780;
    if (y < 0)   y = 0;
    if (y > 580) y = 580;
}

//ï`âÊ
void Player::render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 80, 255, 80, 255);
    SDL_Rect rect = { (int)x, (int)y, 20, 20 };
    SDL_RenderFillRect(r, &rect);
}