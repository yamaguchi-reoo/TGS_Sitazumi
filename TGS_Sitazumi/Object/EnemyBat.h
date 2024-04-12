#pragma once
#include "CharaBase.h"

class EnemyBat:
	public CharaBase
{
private:

public:
	EnemyBat();
	~EnemyBat();
	void Update(GameMain* main)override;
	void Draw()const override;
};

