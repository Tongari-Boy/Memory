#pragma once
#include <SDL.h>
#include "StackAllocator.h"

// タイルの種類
enum TileType {
    TILE_FLOOR = 0, //床（通行可能）
    TILE_WALL = 1, //壁（通行不可）
    TILE_WATER = 2, //水（通行不可）
};

struct Map {
    static const int TILE_SIZE = 64;
    static const int COLS = 13; //横タイル数
    static const int ROWS = 10; //縦タイル数

    TileType* tiles; //スタックアロケータから確保

    void load(StackAllocator& alloc, int stageIndex);
    void render(SDL_Renderer* r);
    bool isWall(int col, int row) const;
    void getFloorTiles(int* colsOut, int* rowsOut, int& countOut, int maxCount) const;
};