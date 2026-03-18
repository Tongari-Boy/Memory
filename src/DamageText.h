#pragma once

//ダメージ数値表示
struct DamageText {
	float x, y;
	float vy;		//上に浮かぶ速度
	int value;		//ダメージ値
	int lifetime;
};