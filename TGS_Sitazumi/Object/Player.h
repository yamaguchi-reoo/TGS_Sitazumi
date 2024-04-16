#pragma once
#include "Object.h"
#include "Stage.h"
#include "ColorData.h"

class Player :
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
	Location vector; //プレイヤーのベクトル
	bool stageHitFlg[2][4]; //プレイヤーとステージの当たり判定
	//0の行はステージのブロック一つ一つに対しての判定
	//1の行は1フレームでステージ全体との判定

	Location aimLoc; //照準座標
	Location aimVec; //照準ベクトル
	Location lineLoc; //線の座標

public:
	Player();
	~Player();
	void Update()override;
	void Draw()const override;
	void Hit(Location _location, Erea _erea, int _type)override;

	void MoveActor(); //キャラクターの移動
	void MoveAim(); //照準の移動

	bool CheckCollision(Stage* stage); //ステージとの当たり判定
};

