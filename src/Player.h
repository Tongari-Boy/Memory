#pragma once
#include "GameObject.h"
#include <SDL.h>

struct Player : public GameObject {
    float x, y;     //ˆÊ’uÀ•W
    float speed;    //ˆÚ“®‘¬“x
    int   hp;

    void init(float startX, float startY);
    void update(const Uint8* keys);
    void render(SDL_Renderer* r);
};