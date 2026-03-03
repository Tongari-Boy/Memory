//=================================
//オブジェクトプールの設計や扱い方
//=================================

#include <iostream>

struct Enemy {
	float x;		//4バイト
	float y;		//4バイト
	int hp;			//4バイト
	bool active;	//1バイト(trueなら使用中、falseなら空きスロット)
};

//無駄な3バイトをなくす構造
struct EnemyOptimized {
	float x;      // 4バイト
	float y;      // 4バイト
	int   hp;     // 4バイト
	bool  active; // 1バイト
	// ↑この後に他のboolやcharを追加すれば無駄がなくなる
	bool visible;
	bool isDead;
	char id;
};

class EnemyPool {

public:
	static const int MAX = 5;	//スロット数
	Enemy pool[MAX];			//ヒープではなく、ここで全部まとめて持つ

	//コンストラクタ
	EnemyPool() {
		//全スロットを空き状態で初期化
		for (int i = 0; i < MAX; i++) {
			pool[i].active = false;
		}
	}

	//空きスロットを探して返す
	Enemy* spawn(float x, float y, int hp) {
		for (int i = 0; i < MAX; i++) {
			if (!pool[i].active) {
				pool[i].x = x;
				pool[i].y = y;
				pool[i].hp = hp;
				pool[i].active = true;

				return &pool[i];
			}
		}
		//上限に達したら警告
		std::cout << "[警告]スロットが満杯です!!!!" << std::endl;
		return nullptr;	//空きがない場合はnullptrを返す
	}

	//スロットを返却する
	void despawn(Enemy* e) {
		if (e == nullptr) return;	//nullチェック(2重despawnによるダングリングポインタを防ぐ)
		if (!e->active) {
			std::cout << "[警告]2重despawnを検出!!!" << std::endl;
			return;
		}

		//ようやくスロットを返却
		e->active = false;
	}

	//状態を表示
	void debugPrint() {
		for (int i = 0; i < MAX; i++) {
			std::cout << "slot[" << i << "] "
				<< (pool[i].active ? "ACTIVE" : "EMPTY ")
				<< std::endl;
		}
	}
};

//int main(int argc, char* argv[])
int example() {
	EnemyPool pool;

	std::cout << "--- 初期状態 ---" << std::endl;
	pool.debugPrint();

	//敵を3体生成
	Enemy* e1 = pool.spawn(1.0f, 2.0f, 100);
	Enemy* e2 = pool.spawn(3.0f, 4.0f, 80);
	Enemy* e3 = pool.spawn(5.0f, 6.0f, 60);

	std::cout << "\n--- 3体生成後 ---" << std::endl;
	pool.debugPrint();

	//e2を倒す
	pool.despawn(e2);
	//2重despawn
	pool.despawn(e2);

	std::cout << "\n--- e2を倒した後 ---" << std::endl;
	pool.debugPrint();

	//新しい敵を生成（e2のスロットが再利用される）
	Enemy* e4 = pool.spawn(7.0f, 8.0f, 120);

	std::cout << "\n--- 新しい敵生成後 ---" << std::endl;
	pool.debugPrint();

	//nullチェック
	Enemy* e = pool.spawn(1.0f, 2.0f, 100);
	if (e == nullptr) {
		std::cout << "生成失敗！" << std::endl;
		return 1;
	}

	return 0;
}