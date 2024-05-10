#pragma once
#include "Vector2D.h"
#include <math.h>


static Vector2D GetUnitVector(Vector2D v) {
	Vector2D tmp;
	float dis;

	dis = sqrtf(powf(v.x, 2) + powf(v.y, 2));
	tmp = v / dis;

	return tmp;
}

static Vector2D GetRotaLocation(Vector2D l, Vector2D base, float ang) {

	//r: 角度(ラジアン)
	//x : 元のX座標
	//y : 元のY座標
	//
	//X = x * cos(r) - y * sin(r)
	//Y = x * sin(r) + y * cos(r)

	Vector2D ret;
	Vector2D tmp = l - base;

	ret.x = tmp.x * cosf(ang) - tmp.y * sinf(ang);
	ret.y = tmp.x * sinf(ang) + tmp.y * cosf(ang);

	ret += base;

	return ret;
}