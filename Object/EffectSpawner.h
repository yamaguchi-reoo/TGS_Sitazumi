#pragma once
#include "BoxCollider.h"
#define EFFECT_NUM	100		//エフェクトの最大数

class GameMain;

struct EffectAnim{
	int timer;					//経過時間
	Location location;			//座標
	Location local_location;	//ローカル座標
	Erea erea;					//大きさ
	bool spawn_flg = false;		//エフェクトがスポーンしているかどうか
	int  effect_type;			//エフェクトの種類
	bool touch_ground;			//地面に触れたか
	float angle;				//移動方向
	int effect_time;			//消えるまでの時間測定用
	float speed;				//移動速度
	int color;					//色
	Location effect_shift[2];	//輝きエフェクトのずれ格納用
	int shine_timer;			//輝きエフェクトの時間測定用
	float rad;					//輝きエフェクトの回転方向
};

class EffectSpawner
{
private:
	EffectAnim effect[EFFECT_NUM];
	int frame;		//フレーム測定

public:
	EffectSpawner();
	~EffectSpawner();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();

	//エフェクトをスポーンさせる _location=座標 _erea=大きさ _effect_type=エフェクトの種類 _time=消えるまでのフレーム数 _color=エフェクトの色
	void SpawnEffect(Location _location,Erea _erea,int _effect_type,int _time,int _color);

	//ローカル座標の更新
	void SetScreenPosition(Location _world_to_screen, int _num);

	//エフェクト構造体のリセット
	void ResetEffect(int _num);
};

