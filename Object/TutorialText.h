#pragma once
#include "Object.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../Utility/ResourceManager.h"

class TutorialText
{

private:

	Location in_camera;

	Location Gdraw_stick_shift;   //描画するスティックをずらす座標
	float Gstick_angle;          //スティックのアングルを保存
	bool Gbutton_draw[4];           //どの画像を描画するか
	int GNum;
	int GColor;															 
	int GGNum;															 
	int frame;
	
	int p_swap_x;
	int e_swap_x;

	bool cosor_flg;

	int stage_height;
	int p_c;
	int f_c;

	int add_x, add_y;


	float circleAng;//カーソルの回転
																		 
public:																	 
	TutorialText();														 
	~TutorialText();													 
																		 
	void Update(Location camera, Location _p, int height);				 
	void Draw()const;
	void GDrawPlayer(int xNum, int yNum, int add_x, int add_y, int _c)const;
	void GDrawFrag()const;
	void GDrawCircle(bool f)const;

	//引数:もとになる座標、回転させたい座標、回転させたい角度
	Location RotationLocation(Location BaseLoc, Location Loc, float r) const;
};


