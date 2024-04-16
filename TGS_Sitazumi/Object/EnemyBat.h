#pragma once
#include "Object.h"

class EnemyBat:
	public Object
{
private:

	//int KeepMouseX[10], KeepMouseY[10];
	int number;

	int frame;
public:
	EnemyBat();
	~EnemyBat();
	void Update()override;
	void Draw()const override;
	void Hit(Location _location, Erea _erea, int _type)override;
};

