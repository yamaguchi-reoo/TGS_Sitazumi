#pragma once
#include "../Utility/Vector2D.h"

#define GRAVITY 0.5f;




class Player
{
private:

public:
	Player();
	~Player();

	void Update();
	void Draw() const;
};

