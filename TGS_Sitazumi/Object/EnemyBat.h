#pragma once
#include "CharaBase.h"

class EnemyBat:
	public CharaBase
{
private:

	//int KeepMouseX[10], KeepMouseY[10];
	int number;

	int frame;
public:
	EnemyBat();
	~EnemyBat();
	void Update(GameMain* main)override;
	void Draw()const override;
};

