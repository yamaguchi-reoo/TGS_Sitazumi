#pragma once
#include "Object.h"
#include <vector>
#include <ctime>
#include "EnemyBat.h"
#include "BossAttackFire.h"
#include "BossAttackWater.h"
#include "BossAttackWood.h"

enum class BossState {
	MOVE = 0,
	ATTACK,
	DEATH
};

class Boss :
	public Object
{
private:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
	Location vector; //ボスのベクトル
	bool stageHitFlg[2][4]; //bossとステージの当たり判定
	float move[4];//各方向加速度格納用

	BossState boss_state;// 現在のボスの状態

	int barrier_num; // バリアの数

	bool damage_flg;				//ダメージを受けたとき
	int damage_effect_time = 60;	//ダメージエフェクトの持続時間
	bool damage_effect_flg = false; // ダメージエフェクトのフラグ

	//Location direction;			// 移動方向
	Location target_direction;	// 目標方向

	float speed;				// 移動速度;
	float angle;
	float direction;


	int state_change_time;		// 状態変更のタイミング

	std::vector<Location>vertices;

	bool f = false;
	bool oldF = false;
	int cnt = 0;
	int attack = 0;
	int attack_num = 0;
	bool side = false;

public:
	Boss();
	~Boss();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	// ボスの移動処理
	void Move(GameMain* _g);

	// ヒット時の処理
	void Hit(Object* _object)override;

	bool SearchColor(Object* ob) {
		return 0;
	}
	//ステージとの当たり判定
	bool CheckCollision(Location l, Erea e);

	//プレイヤーとボスの距離計算
	float DistanceCalc(Location pos1, Location pos2);

	//ボスのバリア
	void barrier();

	void BossAtack(GameMain *_g);

	// ランダムな方向に移動する設定
	void SetRandMove();;
};