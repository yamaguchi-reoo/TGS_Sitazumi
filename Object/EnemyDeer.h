#pragma once
#include "Object.h"
#include "CharaBase.h"
#include "../Utility/common.h"

# define PI 3.14159265359

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

	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};

	DeerState deer_state;
	bool deer_spawn;
	bool deer_death = false;

	float move[4]; //各方向加速度格納用
	bool stageHitFlg[2][4]; //カエルとステージの当たり判定

	float r_x = 0.0f, r_y = 0.0f;

	//デバック用
	float DrawTest1 = 0, DrawTest2 = 0, DrawTest3 = 0, DrawTest4 = 0;
	int	DrawTest5 = 0, DrawTest6 = 0;

public:

	EnemyDeer();
	~EnemyDeer();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void EnemyDeerMove(void);
	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};

