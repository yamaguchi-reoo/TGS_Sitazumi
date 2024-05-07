#pragma once
#include "../Utility/common.h"
#include "Object.h"

#define ANIM_BLOCK_NUM 10 //エフェクト表示に使うブロックの数

//StageとEditで使う識別用
#define WHITE_BLOCK 1
#define GRAY_BLOCK 2
#define RED_BLOCK	3	
#define GREEN_BLOCK 4	
#define BLUE_BLOCK 5	
#define FIRE_BLOCK 6	
#define WOOD_BLOCK 7	
#define WATER_BLOCK 8	

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
static int text_color[19]
{
	0xffffff,	//無
	0x000000,	//白ブロック
	0x000000,	//灰ブロック
	0x00ffff,	//赤ブロック
	0xff00ff,	//緑ブロック

	0xffff00,	//青ブロック
	0xff0000,	//赤エリア
	0x00ff00,	//緑エリア
	0x0000ff,	//青エリア
	0x00ff00,	//プレイヤー初期位置

	0xff0000,	//赤鹿
	0x00ff00,	//緑鹿
	0x0000ff,	//青鹿
	0xff0000,	//赤蝙蝠
	0xff00ff,	//緑蝙蝠

	0x0000ff,	//青蝙蝠
	0xff0000,	//赤蛙
	0x00ff00,	//緑蛙
	0x0000ff,	//青蛙
};

//ブロックアニメーション用
static int stage_shift[10]
{
	3,5,5,4,4,3,3,2,2,1
};

//炎エフェクト用
struct FireAnim
{
	Location shift;		//表示位置
	Erea erea;			//大きさ
	int time;			//表示していられる時間
	float angle;		//移動の向き
};

//木エフェクト用
struct WoodAnim
{
	Location shift1;	//表示位置一点目
	Location shift2;	//表示位置二点目
	float shift;		//表示位置ずれ用
};

//水エフェクト用
struct WaterAnim
{
	Location shift1;	//表示位置
	Erea erea;			//大きさ
	float shift;		//表示位置ずれ用
};

class Stage :
	public Object
{
private:
	int frame;							//フレーム測定用
	int old_color;						//一フレーム前の色データ格納
	int block_type;						//ブロックの種類(0=無 1=白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期スポーン(Editのみ表示))
	bool inv_flg;						//ブロックに触れるか判断
	bool debug_flg;						//EditScene用表示をするかどうか
	int anim;							//アニメーション用
	FireAnim fire_anim[ANIM_BLOCK_NUM];	//炎アニメーション用ブロック情報格納
	WoodAnim wood_anim[ANIM_BLOCK_NUM];	//木アニメーション用ブロック情報格納
	WaterAnim water_anim[ANIM_BLOCK_NUM];//水アニメーション用ブロック情報格納
	bool hit_flg;						//何かが当たった時用
	int hit_timer;						//何かが当たった時のアニメーション用

	FireAnim effect_anim[ANIM_BLOCK_NUM];	//色交換可能ブロックの強調表示
public:
	Stage(int _type);
	~Stage();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Update();	//Edit用アップデート
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}
	//ステージの当たり判定の種類取得(false=当たり判定無しtrue=当たり判定あり)
	bool GetStageCollisionType();
	//ステージのタイプ設定
	void SetStageType(int _type);
	//ステージのタイプ取得(0=無 1=�白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期リスポーン)
	int GetStageType() { return type; }
	//Edit表示設定
	void SetDebugFlg() { debug_flg = true; }
};

