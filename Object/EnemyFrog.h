#pragma once
#include "Object.h"


enum class FrogState {
	IDLE_LEFT = 0,
	IDLE_RIGHT,
	LEFT_JUMP,
	RIGHT_JUMP,
	DEATH
};

class EnemyFrog :
    public Object
{
public:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
private:
	int frame;				//フレーム測定
	FrogState frog_state;	//カエルの状態

	Location vector; //カエルのベクトル
	bool stageHitFlg[2][4]; //カエルとステージの当たり判定
	float move[4];			//各方向加速度格納用
public:
	EnemyFrog();
	~EnemyFrog();
	void Initialize(Location _location, Erea _erea, int _color_data)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;
	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};

