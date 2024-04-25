#pragma once
#include "Object.h"
#include "CharaBase.h"
#include "../Utility/common.h"

using namespace std;

enum class DeerState {
	LEFT = 0,
	RIGHT,
	DEATH
};

class EnemyDeer :
	public Object
{
private:

	DeerState deer_state;

	float DrawTest1, DrawTest2, DrawTest3, DrawTest4;
	int	DrawTest5, DrawTest6;

public:

	EnemyDeer();
	~EnemyDeer();
	void Initialize(Location _location, Erea _erea, int _color_data)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void EnemyDeerMove(void);	
};

