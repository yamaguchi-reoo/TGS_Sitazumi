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

	float searchedLen; //交換できるオブジェクトとの距離
	Object* searchedObj; //一番近い色を交換できるオブジェクト
	Object* searchedObjAll[OBJECT_NUM]; //画面内の色交換できるオブジェクト
	int objNum;//オブジェクトの添え字
	int objSelectNum[2];//オブジェクトの添え字
	int objSelectNumTmp;//オブジェクトの添え字
	int posRelation[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];
	int posRelNum[2];
	bool oldStick[4];
	bool searchFlg;
	int swapTimer;		//交換エフェクトの時間測定
	bool oldSearchFlg;
	Location saveVec; //ベクトルの一時保存用

	int fps = 0;
	int fps1 = 0;
	float move[4] = { 0,0,0,0 };
	Location aaa;
public:
	Player();
	~Player();
	void Initialize(Location _location, Erea _erea, int _color_data)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;

	void MoveActor(); //キャラクターの移動
	void MoveAim(); //照準の移動 近くの色の検知

	bool SearchColor(Object* ob) override;
	bool ChangePlayerColor();
	void SelectObject();

	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
	float ThreePointAngle(Location l1, Location l2, Location referenceP) const;

	float GetLength(Location l1, Location l2);

};

