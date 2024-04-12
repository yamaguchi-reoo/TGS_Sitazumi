#pragma once
#include "../Object/CharaBase.h"

class EnemyDeer :
	public CharaBase
{
private:

	int KeepMouseX[10], KeepMouseY[10];
	int number;

	int akusel;

public:

	EnemyDeer();
	~EnemyDeer();
	void Update(GameMain* main)override;
	void Draw()const override;
};

