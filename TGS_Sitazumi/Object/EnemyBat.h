#pragma once
#include "CharaBase.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>

enum class BatState {
	IDLE = 0,
	LEFT,
	RIGHT,
	DEATH
};

class EnemyBat:
	public CharaBase
{
private:
	//std::vector<std::vector<int>>vertices;
	int x_move;
	Location vertices[32];

public:
	EnemyBat();
	~EnemyBat();
	void Update(GameMain* main)override;
	void Draw()const override;
public:
	void Move();
	void BatArray();

};

