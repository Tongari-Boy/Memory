#include "Player.h"
#include "Map.h"


//初期化
void Player::init(float startX, float startY) {
    x = startX;
    y = startY;
    speed = 3.0f;
    hp = 3;
    invincibleTimer = 0;
    isAttacking = false;
    knockbackX = 0;
    knockbackY = 0;
    knockbackTimer = 0;
    active = true;
}

//入力処理
void Player::update(const Uint8* keys,const Map* map) {
    if (invincibleTimer > 0) invincibleTimer--;

    //ノックバック中
    if (knockbackTimer > 0) {

        knockbackTimer--;

        float nx = x + knockbackX;
        float ny = y + knockbackY;
        knockbackX *= 0.8f;
        knockbackY *= 0.8f;

        int size = 20;

        //X方向の壁判定
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

    if (isAttacking) return;    //攻撃中は移動しない

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

//ノックバック処理
void Player::applyKnockback(float dx, float dy) {
    float len = std::sqrt(dx * dx + dy * dy);
    if (len == 0) return;
    knockbackX = (dx / len) * 5.0f;    //反発速度
    knockbackY = (dy / len) * 5.0f;
    knockbackTimer = 8;               //8フレーム反発
}