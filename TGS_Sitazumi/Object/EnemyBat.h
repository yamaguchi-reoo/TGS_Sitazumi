#pragma once
#include "CharaBase.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include "Object.h"

#define NUM_VERTICES 32 //頂点の数

enum class BatState {
	IDLE = 0,
	LEFT,
	RIGHT,
	DEATH
};

class EnemyBat:
	public Object
{
private:
	//std::vector<std::vector<int>>vertices;
	//Location vertices[32];

	// Location構造体の配列を定義
	Location vertices[NUM_VERTICES] = {
		// 耳
		{850, 300}, {850, 325}, {862, 312},
		{880, 300}, {880, 325}, {868, 312},
		//右翼
		{877, 344}, {917, 310}, {943, 385},
		{877, 344}, {917, 310}, {916, 380},
		{875, 344}, {917, 310}, {895, 375},
		//左翼
		{853, 344}, {816, 310}, {787, 385},
		{853, 344}, {816, 310}, {814, 380},
		{855, 344}, {816, 310}, {835, 375},
		// 頭
		{865, 316}, {880, 330}, {865, 340}, {850, 330},
		// 胴体
		{865, 345}, {880, 368}, {865, 395}, {850, 368},
	};

public:
	EnemyBat();
	~EnemyBat();
	void Update()override;
	void Draw()const override;
public:
	void Move();
	void BatArray();

	void Hit(Location _location, Erea _erea, int _type)override;
};

