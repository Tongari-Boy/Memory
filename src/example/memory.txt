//=========================
//スタックとヒープについて
//=========================

#include <iostream>

int main() {
	
	//スタック(自動管理)
	int a = 10;

	//ヒープ(自分で管理しないとリークする)
	int* p = new int(42);
	std::cout << *p << std::endl;

	delete p;		//これを忘れるとメモリリーク
	p = nullptr;	//解放後にnullptrにする習慣をつける(ダングリングポインタになるのを防ぐ)

	return 0;
}