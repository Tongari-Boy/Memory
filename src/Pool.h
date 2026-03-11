#pragma once
#include "GameObject.h"
#include <iostream>

template<typename T,int MAX>
class Pool {
public:
	T	pool[MAX];
	int freeList[MAX];
	int freeCount;
	int activeList[MAX];
	int activeCount_;

	Pool() {
		freeCount		= MAX;
		activeCount_	= 0;
		for (int i = 0; i < MAX; i++) {
			pool[i].active = false;
			freeList[i] = i;
		}
	}

	T* alloc() {
		if (freeCount == 0) {
			std::cout << "[Œx]ƒXƒƒbƒg‚ª–ž”t‚Å‚·" << std::endl;
			return nullptr;
		}
		int index = freeList[--freeCount];
		pool[index].active = true;
		activeList[activeCount_++] = index;
		return &pool[index];
	}

	void free(T* e) {
		if (!e) return;

		if (!e->active) {
			std::cout << "[Œx]“ñdfree‚ðŒŸo" << std::endl;
			return;
		}

		e->active = false;

		int index = e - pool;
		freeList[freeCount++] = index;

		for (int i = 0; i < activeCount_; i++) {
			if (activeList[i] == index) {
				activeList[i] = activeList[--activeCount_];
				break;
			}
		}
	}

	int activeCount() { return activeCount_; }
};