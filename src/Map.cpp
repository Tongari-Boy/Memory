#include "Map.h"
#include <iostream>

//ステージデータ（0=床 1=壁 2=水）
static const int STAGE_DATA[2][Map::ROWS][Map::COLS] = {
    //ステージ1
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,0,0,0,1,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,1},
        {1,0,0,1,1,0,0,0,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1},
    },
    //ステージ2
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,1,0,0,0,0,1},
        {1,0,2,2,0,1,0,1,0,2,2,0,1},
        {1,0,2,2,0,0,0,0,0,2,2,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,2,2,0,0,0,0,0,2,2,0,1},
        {1,0,2,2,0,1,0,1,0,2,2,0,1},
        {1,0,0,0,0,1,0,1,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1},
    },
};

void Map::load(StackAllocator& alloc, int stageIndex) {
    //スタックアロケータからタイル配列を確保
    tiles = (TileType*)alloc.alloc(sizeof(TileType) * COLS * ROWS);
    if (!tiles) return;

    int idx = stageIndex % 2;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            tiles[r * COLS + c] = (TileType)STAGE_DATA[idx][r][c];

    std::cout << "Map loaded: "
        << sizeof(TileType) * COLS * ROWS
        << " bytes from StackAllocator" << std::endl;
}

void Map::render(SDL_Renderer* r) {
    if (!tiles) return;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            TileType t = tiles[row * COLS + col];
            SDL_Rect rect = { col * TILE_SIZE, row * TILE_SIZE,
                             TILE_SIZE, TILE_SIZE };
            switch (t) {
            case TILE_FLOOR:
                SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
                break;
            case TILE_WALL:
                SDL_SetRenderDrawColor(r, 120, 80, 40, 255);
                break;
            case TILE_WATER:
                SDL_SetRenderDrawColor(r, 40, 80, 180, 255);
                break;
            }
            SDL_RenderFillRect(r, &rect);

            //壁の枠線
            SDL_SetRenderDrawColor(r, 30, 30, 30, 255);
            SDL_RenderDrawRect(r, &rect);
        }
    }
}

bool Map::isWall(int col, int row) const {
    if (col < 0 || col >= COLS || row < 0 || row >= ROWS)
        return true;
    TileType t = tiles[row * COLS + col];
    return t == TILE_WALL || t == TILE_WATER;
}