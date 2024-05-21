#pragma once
#include"BoxCollider.h"
#include"ColorData.h"

#define BLOCK 0			//Objectの種類用
#define PLAYER 1		//Objectの種類用
#define ENEMY 2			//Objectの種類用
#define FIRE 3			//Objectの種類用
#define WOOD 4			//Objectの種類用
#define WATER 5			//Objectの種類用
#define BOSS 6			//Objectの種類用

//仮想クラス
class GameMain;

class Object : public BoxCollider, public ColorData
{
protected:
	int type;		//Objectの種類格納 0=BLOCK 1=PLAYER 2=ENEMY 3=FIRE 4=WOOD 5=WATER 6=EFFECT
	bool can_swap ;	//交換できるオブジェクトか
	bool can_hit;	//当たり判定があるオブジェクトか
	bool searchFlg;	//スローモーション中か（プレイヤー）
	int object_pos;	//ゲームメインで生成された時、オブジェクト配列のどこに格納されたか
public:
	//_location _erea=スポーン座標、大きさ ,_color_data=色情報
	virtual void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos) = 0;

	virtual void Update(GameMain* _g) = 0;

	virtual void Draw()const = 0;

	virtual void Finalize() = 0;


	//何かと当たった時の処理
	virtual void Hit(Object* _object) 
	{
		int a = 0;
	}

	virtual bool SearchColor(Object* ob) = 0;

	//各変数の取得
	int GetObjectType() { return type; }
	bool GetCanSwap() { return can_swap; }
	bool GetCanHit() { return can_hit; }
	bool GetSearchFlg() { return searchFlg;	}
	int GetObjectPos() { return object_pos; }

	//各変数の変更
	void SetObjectType(int _num) { type = _num; }
	void SetCanSwap(bool _flg) { can_swap = _flg; }
	void SetCanHit(bool _flg) { can_hit = _flg; }
	void SetSearchFlg(bool _flg) { searchFlg = _flg; }
	void SetObjectPos(int _num) { object_pos = _num; }
};