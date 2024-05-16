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
	Object* oldSearchedObj; //前回選択していたオブジェクト
	Object* oldSearchedObjAll[OBJECT_NUM]; //選択していたすべてのオブジェクト
	int oldObjNum;
	int direction; //選択方向
	int oldDirection; //前回の選択方向
	Object* searchedObjAll[OBJECT_NUM]; //画面内の色交換できるオブジェクト
	int objNum;//オブジェクトの添え字
	int objSelectNum[2];//オブジェクトの添え字
	int objSelectNumTmp;//オブジェクトの添え字
	bool searchedObjFlg;//画面内に交換できるオブジェクトがあるかどうか

	int posRelation[720 / 40][1280 / 40];
	int posRelNum[2];
	bool oldStick[4];
	//bool searchFlg;
	//serchFlgの定義をObjectの方に移動して、GameMainで参照出来るようにしました　
	int swapTimer;		//交換エフェクトの時間測定
	bool oldSearchFlg;
	Location saveVec; //ベクトルの一時保存用

	bool damageFlg; //ダメージを受けたとき
	bool damageOldFlg; //ダメージを受けたとき
	static int hp;
	int damageEffectTime = 60;
	bool damageEffectFlg = false;

	int state; //水中とか火の中とかの状態を管理
	bool stateFlg; //一フレーム内かを判定

	int fps = 0;
	int fps1 = 0;
	float move[4] = { 0,0,0,0 };
	Location aaa;

public:
	Player();
	~Player();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;

	void MoveActor(); //キャラクターの移動
	void MoveAim(); //照準の移動 近くの色の検知

	bool SearchColor(Object* ob) override;
	bool ChangePlayerColor();
	void SelectObject();

	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
	float ThreePointAngle(Location l1, Location l2, Location referenceP) const;

	float GetLength(Location l1, Location l2);

};

