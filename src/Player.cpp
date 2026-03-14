#include "Player.h"
#include "Map.h"


//初期化
void Player::init(float startX, float startY) {
    x = startX;
    y = startY;
    speed = 3.0f;
    hp = 3;
    active = true;
}

//入力処理
void Player::update(const Uint8* keys,const Map* map) {
    //ワープ
    float nx = x;
    float ny = y;

    //移動処理
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    ny -= speed;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  ny += speed;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  nx -= speed;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) nx += speed;

    //プレイヤーの四隅でタイル判定
    if (map) {
        int size = 20; //プレイヤーのサイズ

        //X方向の判定
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

    //画面端処理
    if (x < 0)   x = 0;
    if (x > 780) x = 780;
    if (y < 0)   y = 0;
    if (y > 580) y = 580;
}

//描画
void Player::render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 80, 255, 80, 255);
    SDL_Rect rect = { (int)x, (int)y, 20, 20 };
    SDL_RenderFillRect(r, &rect);
}