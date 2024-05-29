#pragma once
#include "Object.h"
#define EFFECT_NUM	100		//エフェクトの最大数

enum EffectList {
	PlayerSpawnEffect = 0,
	ShineEffect,
	DeathEffect,
	LandingEffect
};

class GameMain;

struct EffectAnim{
	int timer = 0;					//経過時間
	Location location = { 0,0 };			//座標
	Location local_location = { 0,0 };	//ローカル座標
	Erea erea = { 0,0 };					//大きさ
	bool spawn_flg = false;		//エフェクトがスポーンしているかどうか
	int  effect_type = 0;			//エフェクトの種類
	bool touch_ground = false;			//地面に触れたか
	float angle = 0.0f;				//移動方向
	int effect_time = 0;			//消えるまでの時間測定用
	float speed = 0.0f;				//移動速度
	int color = 0;					//色
	Location effect_shift[2] = {0,0};	//輝きエフェクトのずれ格納用
	int shine_timer = 0;			//輝きエフェクトの時間測定用
	float rad = 0.0f;					//輝きエフェクトの回転方向
};

struct SwapAnim
{
	bool move_flg;          //移動するか判断
	Location start;          //オブジェクトの座標（アニメーション開始地点）
	Erea erea;               //オブジェクトのサイズ
	Location location;      //交換エフェクト用変数
	Location local_location;	//ローカル座標
	float move_rad;        //交換エフェクト移動用
	int color;             //色情報
	int timer;              //エフェクト時間
	int speed;              //エフェクト速度
};

class EffectSpawner
{
private:
	SwapAnim swap_anim[2];  //交換エフェクト用
	int swap_anim_timer;    //交換エフェクト時間用
	EffectAnim effect[EFFECT_NUM];
	int frame;		//フレーム測定

	//SE格納
	int swap_se;
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

	//交換アニメーション
	int Swap(Object* _object1, Object* _object2);

	//エフェクトを構成するパーティクルをスポーンさせる _location=座標 _erea=大きさ _effect_type=パーティクルの種類 _time=消えるまでのフレーム数 _speed=速度 _color=エフェクトの色 _angle=進行方向
	void SpawnParticle(Location _location, Erea _erea, int _effect_type, int _time, float _speed, int _color, float _angle);

};

