#pragma once
#include "CharaBase.h"
#include <vector>
#include "../Utility/common.h"
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

	//std::vector<Location> vertices;

	BatState bat_state;
	float wing_angle;


	int up;

public:
	EnemyBat();
	~EnemyBat();
	void Initialize(Location _location, Erea _erea, int _color_data)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

public:
	void Move();
	void BatArray();

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) {
		return 0;
	}
};

