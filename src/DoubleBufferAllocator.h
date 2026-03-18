#pragma once
#include <cstddef>
#include <iostream>

class DoubleBufferAllocator {
public:
	//2つのバッファ
	char* buffers[2];
	size_t bufferSize;
	size_t offsets[2];
	int currentBuffer;	//現在書き込み中のバッファ番号

	DoubleBufferAllocator()
		: bufferSize(0), currentBuffer(0) {
		buffers[0] = buffers[1] = nullptr;
		offsets[0] = offsets[1] = 0;
	}

	void init(size_t size) {
		bufferSize = size;
		buffers[0] = new char[size];
		buffers[1] = new char[size];
		offsets[0] = 0;
		offsets[1] = 0;
		currentBuffer = 0;
	}

	//現在のバッファから確保
	void* alloc(size_t size) {
		int cur = currentBuffer;
		if (offsets[cur] + size > bufferSize) {
			std::cout << "[警告]DoubleBufferAllocator:メモリ不足です。" << std::endl;
			return nullptr;
		}
		void* ptr = buffers[cur] + offsets[cur];
		offsets[cur] += size;
		return ptr;
	}

	//フレーム終了時:バッファを切り替え -> 古い方はクリア
	void flip() {
		currentBuffer = 1 - currentBuffer;	//0 -> 1, 1 -> 0
		offsets[currentBuffer] = 0;			//新しい書き込み先を指定
	}

	size_t usedBytes() const { return offsets[currentBuffer]; }
	size_t totalBytes() const { return bufferSize; }

	//全消し
	void shutdown() {
		delete[] buffers[0];
		delete[] buffers[1];
		buffers[0] = buffers[1] = nullptr;
	}
};