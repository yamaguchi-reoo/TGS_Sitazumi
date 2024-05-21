#pragma once
#include "Object.h"
#include <vector>
#include "EnemyBat.h"

enum class BossState {
	IDLE = 0,
	LEFT,
	RIGHT,
	DEATH
};

class Boss :
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
	Location vector; //ボスのベクトル
	bool stageHitFlg[2][4]; //コウモリとステージの当たり判定
	float move[4];//各方向加速度格納用

	BossState boss_state;


	//std::vector<Location>vertices;
	bool hit;

public:
	Boss();
	~Boss();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Move(float dx, float dy);

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) {
		return 0;
	}
	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};