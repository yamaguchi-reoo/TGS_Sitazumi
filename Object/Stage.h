#pragma once
#include "../Utility/common.h"
#include "Object.h"

//StageとEditで使う識別用
enum BlockList	{
	NULL_BLOCK = 0,
	WHITE_BLOCK,
	GRAY_BLOCK,
	RED_BLOCK,
	GREEN_BLOCK,
	BLUE_BLOCK,
	FIRE_BLOCK,	
	WOOD_BLOCK,	
	WATER_BLOCK	,
	PLAYER_BLOCK,	
	ENEMY_DEER_RED,	
	ENEMY_DEER_GREEN,
	ENEMY_DEER_BLUE	,
	ENEMY_BAT_RED,	
	ENEMY_BAT_GREEN	,
	ENEMY_BAT_BLUE,	
	ENEMY_FROG_RED,	
	ENEMY_FROG_GREEN,	
	ENEMY_FROG_BLUE	,
	ENEMY_BOSS,

};

//ColorData格納用
static int color_data[9]
{
	0,
	WHITE,
	0,
	RED,
	GREEN,
	BLUE,
	RED,
	GREEN,
	BLUE
};

//Edit表示用文字色データ
static int text_color[20]
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

	0xffffff,	//ボス
};

//ブロックアニメーション用
static int stage_shift[10]
{
	3,5,5,4,4,3,3,2,2,1
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
	bool hit_flg;						//何かが当たった時用
	int hit_timer;						//何かが当たった時のアニメーション用

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
	//ステージのタイプ設定
	void SetStageType(int _type);
	//ステージのタイプ取得(0=無 1=�白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期リスポーン)
	int GetStageType() { return type; }
	//Edit表示設定
	void SetDebugFlg() { debug_flg = true; }
};

