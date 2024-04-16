#pragma once
#include "../Utility/common.h"
#include "Object.h"

#define ANIM_BLOCK_NUM 10 //エフェクト表示に使うブロックの数

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

//Edit表示用文字色データ
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

//炎エフェクト用
struct FireAnim
{
	Location shift;	//表示位置
	Erea erea;			//大きさ
	int time;			//表示していられる時間
	float angle;		//移動の向き
};
//木エフェクト用
struct WoodAnim
{
	Location shift1;			//実際の位置一点目
	Location shift2;			//実際の位置二点目
	float shift;				//表示位置ずれ用
};
class Stage :
	public Object
{
private:
	int frame;					//フレーム測定用
	int block_type;				//ブロックの種類(0=無 1=白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期スポーン(Editのみ表示))
	bool inv_flg;				//ブロックに触れるか判断
	bool debug_flg;				//EditScene用表示をするかどうか
	int swap_flg;				//色を交換出来るか
	int anim;						//アニメーション用
	FireAnim fire_anim[ANIM_BLOCK_NUM];	//炎アニメーション用ブロック情報格納
	WoodAnim wood_anim[ANIM_BLOCK_NUM];	//木アニメーション用ブロック情報格納
public:
	Stage(float _x, float _y, float _width, float _height, int _type);
	~Stage();
	void Update()override;
	void Draw()const override;
	void Hit(Location _location, Erea _erea, int _type)override;
	//ステージの当たり判定の種類取得(0=当たり判定無し1=当たり判定あり)
	bool GetStageCollisionType();
	//ステージのタイプ設定
	void SetStageType(int _type);
	//ステージのタイプ取得(0=無 1=�白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期リスポーン)
	int GetStageType() { return type; }
	//Edit表示設定
	void SetDebugFlg() { debug_flg = true; }
};

