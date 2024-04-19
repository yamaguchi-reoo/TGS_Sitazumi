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
	std::vector<Location> vertices;

	BatState bat_state;
	bool leftwall_flg;

public:
	EnemyBat();
	~EnemyBat();
	void Update()override;
	void Draw()const override;
public:
	void Move();
	void BatArray();

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) {
		return 0;
	}
};

