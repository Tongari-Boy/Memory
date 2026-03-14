#pragma once
#include <cstddef>
#include <iostream>

class StackAllocator {
public:
    //メモリブロックの先頭ポインタ
    char* buffer;
    size_t bufferSize;
    size_t offset;     // 現在の使用量

    StackAllocator() : buffer(nullptr), bufferSize(0), offset(0) {}

    //初期化：起動時に一括確保
    void init(size_t size) {
        buffer = new char[size];
        bufferSize = size;
        offset = 0;
    }

    //確保：offsetを進めるだけ
    void* alloc(size_t size) {
        if (offset + size > bufferSize) {
            std::cout << "[警告] StackAllocator: メモリ不足！" << std::endl;
            return nullptr;
        }
        void* ptr = buffer + offset;
        offset += size;
        return ptr;
    }

    //マーカー：現在のoffsetを保存する
    size_t getMarker() const {
        return offset;
    }

    //ロールバック：マーカーの位置まで一括解放
    void freeToMarker(size_t marker) {
        if (marker > offset) {
            std::cout << "[警告] 無効なマーカーです！" << std::endl;
            return;
        }
        offset = marker;
    }

    //全解放
    void clear() {
        offset = 0;
    }

    //使用量確認
    size_t usedBytes()  const { return offset; }
    size_t totalBytes() const { return bufferSize; }

    void shutdown() {
        delete[] buffer;
        buffer = nullptr;
    }
};