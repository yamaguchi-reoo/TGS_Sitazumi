#pragma once
#include "../Utility/common.h"

#include "BoxCollider.h"
#include "ColorData.h"

#define ANIM_BLOCK_NUM 10 //アニメーションに使うブロックの数
//ColorData格納用
static int color_data[6]
{
	0,
	WHITE,
	0,
	RED,
	GREEN,
	BLUE
};

//デバッグ表示用色情報
static int text_color[10]
{
	0xffffff,
	0x000000,
	0x000000,
	0x00ffff,
	0xff00ff,
	0xffff00,
	0xff0000,
	0x00ff00,
	0x0000ff,
	0x00ff00,
};

//アニメーション用
struct ANIM
{
	Location location;	//座標
	Erea erea;			//大きさ
	int time;			//削除までの時間
	float angle;		//進行方向
};
class Stage :
	public BoxCollider, public ColorData
{
private:
	int frame;					//フレーム測定
	int type;					//箱の種類(0=白 1=灰 2=赤 3=緑 4=青 5=炎 6=木 7=水)
	bool inv_flg;				//何も描画しないか判断
	bool debug_flg;				//箱の種類を表示するか
	int swap_flg;				//色の交換が出来るブロックかどうか
	int anim;						//アニメーション用
	ANIM effect_anim[ANIM_BLOCK_NUM];	//アニメーション用座標保管
public:
	Stage(float _x, float _y, float _width, float _height, int _type);
	~Stage();
	void Update();
	void Draw()const;

	//種類を取得(0=白 1=灰 2=赤 3=緑 4=青 5=炎 6=木 7=水)
	int GetStageCollisionType();
	//種類を設定
	void SetStageType(int _type);
	//種類を取得
	int GetStageType() { return type; }
	//箱の種類を表示する
	void SetDebugFlg() { debug_flg = true; }
};

