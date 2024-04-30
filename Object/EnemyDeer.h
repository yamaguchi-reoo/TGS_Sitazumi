#pragma once
#include "Object.h"
#include "CharaBase.h"
#include "../Utility/common.h"

using namespace std;

enum class DeerState {
	GRAVITY = 0,
	IDLE,
	LEFT,
	RIGHT,
	DEATH
};

class EnemyDeer :
	public Object
{
private:

	DeerState deer_state;
	bool deer_spawn;

	float DrawTest1 = 0, DrawTest2 = 0, DrawTest3 = 0, DrawTest4 = 0;
	int	DrawTest5 = 0, DrawTest6 = 0;

public:

	EnemyDeer();
	~EnemyDeer();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void EnemyDeerMove(void);	
};

