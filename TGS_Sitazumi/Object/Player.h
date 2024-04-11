#pragma once
#include "CharaBase.h"

class Player :
	public CharaBase
{
private:

public:
	Player();
	~Player();
	void Update(GameMain* main)override;
	void Draw()const override;

	
};

