#pragma once
#include "CharaBase.h"
#include "Stage.h"

class Player :
	public CharaBase
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
	Location vector; //ベクトル、もしかしらキャラベースに移動かも
	bool stageHitFlg[2][4]; //上下左右どこと当たっているかを配列に格納 
	
public:
	Player();
	~Player();
	void Update(GameMain* main)override;
	void Draw()const override;

	void Move();

	bool CheckCollision(Stage* stage);
};

