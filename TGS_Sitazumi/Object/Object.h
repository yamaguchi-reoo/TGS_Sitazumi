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
	int type;	//Objectの種類格納
public:

	virtual void Update() = 0;

	virtual void Draw()const = 0;

	//何かと当たった時の処理 location=当たった相手の座標 erea=当たった相手の大きさ type=当たった相手の種類(0=ステージ 1=プレイヤー 2=敵 3=炎 4=木 5=水)
	virtual void Hit(Location _location,Erea _erea,int _type) = 0;

	int GetObjectType() { return type; }
};