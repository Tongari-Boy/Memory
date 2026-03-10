//=======================================
//オブジェクがメモリ上でどう並んでいるか
//=======================================

#include <iostream>

struct Enemy {
	float x;		//4バイト
	float y;		//4バイト
	int hp;			//4バイト
	bool active;	//1バイト
};

int main() {
	Enemy e;
	e.x = 1.0f;
	e.y = 2.0f;
	e.hp = 100;
	e.active = true;

	//各メンバのアドレスを確認
	std::cout << "Enemy Size	:" << sizeof(Enemy) << "bytes" << std::endl;
	std::cout << "x address		:" << &e.x << std::endl;
	std::cout << "y address		:" << &e.y << std::endl;
	std::cout << "hp address	:" << &e.hp << std::endl;
	std::cout << "active address:" << (void*)&e.active << std::endl;

	return 0;
}