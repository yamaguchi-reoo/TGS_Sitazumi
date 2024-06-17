#pragma once
#include "Object.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../Utility/ResourceManager.h"

class TutorialText
{

private:

	Location in_camera;

	Location Gdraw_stick_location;   //描画するスティックの座標
	Location Gdraw_stick_shift;   //描画するスティックをずらす座標
	float Gstick_angle;          //スティックのアングルを保存
	bool Gbutton_draw[3];           //どの画像を描画するか
	int GNum;
	int GColor;
	int GGNum;
	int frame;

	int stage_height;

public:
	TutorialText();
	~TutorialText();

	void Update(Location camera, Location _p, int height);
	void Draw()const;
	void GDrawPlayer(int xNum, int yNum, int add_y)const;
};


