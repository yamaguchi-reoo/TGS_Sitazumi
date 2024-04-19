#pragma once
#include"BoxCollider.h"
#include"ColorData.h"

#define BLOCK 0			//Objectの種類用
#define PLAYER 1		//Objectの種類用
#define ENEMY 2			//Objectの種類用
#define FIRE 3			//Objectの種類用
#define WOOD 4			//Objectの種類用
#define WATER 5			//Objectの種類用

//仮想クラス
class GameMain;

class Object : public BoxCollider, public ColorData
{
protected:
	int type;		//Objectの種類格納
	bool can_swap;	//交換できるオブジェクトか
	bool can_hit;	//当たり判定あるオブジェクトか
public:
	//_location _erea=スポーン座標、大きさ ,_color_data=色情報
	virtual void Initialize(Location _location, Erea _erea, int _color_data) = 0;

	virtual void Update() = 0;

	virtual void Draw()const = 0;

	virtual void Finalize() = 0;


	//何かと当たった時の処理 _location=当たった相手の座標 _erea=当たった相手の大きさ _type=当たった相手の種類(0=ステージ 1=プレイヤー 2=敵 3=炎 4=木 5=水) _color_data=当たった相手も色情報
	virtual void Hit(Location _location,Erea _erea,int _type,int _color_data) = 0;

	virtual bool SearchColor(Object* ob) = 0;

	//各値の取得
	int GetObjectType() { return type; }

	bool GetCanSwap() { return can_swap; }

	bool GetCanHit() { return can_hit; }
};