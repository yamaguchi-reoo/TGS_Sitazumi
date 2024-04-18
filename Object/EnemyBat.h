#pragma once
#include "CharaBase.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include "Object.h"

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
	int x_move;
	Location vertices[32];

public:
	EnemyBat();
	~EnemyBat();
	void Update()override;
	void Draw()const override;
public:
	void Move();
	void BatArray();

	void Hit(Location _location, Erea _erea, int _type)override;
	bool SearchColor(Object* ob) override {
		return false;
	}
};

