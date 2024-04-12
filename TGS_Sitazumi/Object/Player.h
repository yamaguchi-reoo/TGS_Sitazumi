#pragma once
#include "CharaBase.h"
#include "Stage.h"
#include "ColorData.h"

class Player :
	public CharaBase , public ColorData
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
							//0の行は1つのブロックに当たっているか
							//1の行はステージ全体に対しての当たっているか

	Location aimLoc; //照準の座標
	Location aimVec; //照準の方向
	Location lineLoc; //線の終端座標
	
public:
	Player();
	~Player();
	void Update(GameMain* main)override;
	void Draw()const override;

	void MoveActor();
	void MoveAim();

	bool CheckCollision(Stage* stage);
};

