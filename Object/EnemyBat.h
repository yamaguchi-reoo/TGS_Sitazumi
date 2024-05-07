#pragma once
#include "CharaBase.h"
#include <vector>
#include "../Utility/common.h"
#include "Object.h"
#include "../Scene/GameMain.h"


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
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
	Location vector; //コウモリのベクトル
	bool stageHitFlg[2][4]; //カエルとステージの当たり判定
	float move[4];//各方向加速度格納用

	BatState bat_state;	//コウモリの状態
	float wing_angle;	//羽の動かす用
	int up;		//sin波で移動する用

public:
	EnemyBat();
	~EnemyBat();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

public:
	void Move();

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) {
		return 0;
	}

	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};

