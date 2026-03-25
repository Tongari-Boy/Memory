#pragma once
#include "GameObject.h"
#include <SDL.h>

//前方宣言(マップの当たり判定に使用)
struct Map;

struct Player : public GameObject {
    float x, y;     //位置座標
    float speed;    //移動速度
    int   hp;
    int   invincibleTimer;  //無敵時間
    bool isAttacking;   //攻撃中フラグ

    //ノックバック処理用変数郡
    float knockbackX;
    float knockbackY;
    int   knockbackTimer;

    void init(float startX, float startY);
    void update(const Uint8* keys,const Map* map);
    void render(SDL_Renderer* r);
    void applyKnockback(float dx, float dy);
};