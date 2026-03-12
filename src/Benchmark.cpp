#include <iostream>
#include <windows.h>
#include <cmath>

//計測用のタイマー
struct Timer
{
	LARGE_INTEGER start;
	LARGE_INTEGER freq;

	void begin() {
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
	}

	double endMicros() {
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return (double)(end.QuadPart - start.QuadPart)
			/ freq.QuadPart * 1000000.0;
	}
};

//計測対象のデータ
struct Vec2
{
	float x, y;
};

//===========================
//テスト:newで毎回確保・解放
//===========================
void benchHeap(int count) {
	Timer t;
	t.begin();

	for (int i = 0; i < count; i++) {
		Vec2* v = new Vec2();
		v->x = (float)i;
		v->y = (float)i;
		delete v;
	}

	double micros = t.endMicros();
	std::cout << "[HEAP]" << count << "回:"
		<< micros << "us" << std::endl;
}

//============================
//テスト:プールから確保・解放
//============================
static const int POOL_SIZE = 10000;
static Vec2 pool[POOL_SIZE];
static bool used[POOL_SIZE] = {};

Vec2* poolAlloc() {
	for (int i = 0; i < POOL_SIZE; i++) {
		if (!used[i]) { used[i] = true; return &pool[i]; }
	}
	return nullptr;
}

void poolFree(Vec2* v) {
	int index = v - pool;
	used[index] = false;
}

void benchPool(int count) {
	Timer t;
	t.begin();

	for (int i = 0; i < count; i++) {
		Vec2* v = poolAlloc();
		v->x = (float)i;
		v->y = (float)i;
		poolFree(v);
	}

	double micros = t.endMicros();
	std::cout << "[Pool]  " << count << "回: "
		<< micros << " us" << std::endl;
}

//==========================================================
//テスト:連続アクセス(プール)、断片的アクセス(ヒープ)の比較
//==========================================================

//連続
void benchCacheSequential(int count) {
	float* arr = new float[count];
	for (int i = 0; i < count; i++) arr[i] = (float)i;

	Timer t;
	t.begin();

	float sum = 0;
	for (int i = 0; i < count; i++)
		sum += arr[i]; // 連続アクセス

	double micros = t.endMicros();
	std::cout << "[Sequential] sum=" << sum
		<< " : " << micros << " us" << std::endl;

	delete[] arr;
}

//断片的
void benchCacheRandom(int count) {
	float* arr = new float[count];
	int* idx = new int[count];
	for (int i = 0; i < count; i++) {
		arr[i] = (float)i;
		idx[i] = i;
	}

	// インデックスをシャッフル
	for (int i = count - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int tmp = idx[i]; idx[i] = idx[j]; idx[j] = tmp;
	}

	Timer t;
	t.begin();

	float sum = 0;
	for (int i = 0; i < count; i++)
		sum += arr[idx[i]]; // ランダムアクセス

	double micros = t.endMicros();
	std::cout << "[Random]     sum=" << sum
		<< " : " << micros << " us" << std::endl;

	delete[] arr;
	delete[] idx;
}

//比較
int benchmarkMain() {
	const int COUNT = 100000;

	std::cout << "=== Heap and Pool ===" << std::endl;
	benchHeap(COUNT);
	benchPool(COUNT);

	std::cout << "\n=== Cache Random and Sequential ===" << std::endl;
	benchCacheSequential(COUNT);
	benchCacheRandom(COUNT);

	return 0;
}
