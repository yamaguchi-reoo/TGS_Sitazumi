#pragma once
#include "Object.h"

#define JUMP_COOLDOWN 40	//蛙の次のジャンプまでの時間

enum class FrogState {
	RIGHT_IDOL=0,
	LEFT_IDOL,
	LEFT_JUMP,
	RIGHT_JUMP,
	DEATH
};

class EnemyFrog :
    public Object
{
public:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
private:
	int timer;				//時間測定
	FrogState frog_state;	//カエルの状態

	bool death_flg;		//生きているか
	Location vector; //カエルのベクトル
	bool stageHitFlg[2][4]; //カエルとステージの当たり判定
	float move[4];			//各方向加速度格納用
	Location old_location;	//ジャンプ前の座標格納

	int jump_cooldown_timer; //次のジャンプまでの時間測定
	int jump_timer;			 //ジャンプしている間の時間測定
	float face_angle;		//顔の向き
	int death_timer;	//死亡演出
	bool effect_once;	//エフェクトを一回だけ出す用

	int jump_se;		//ジャンプSE
	int damage_se[3];	//被ダメージSE格納

public:
	EnemyFrog();
	~EnemyFrog();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;
	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void JumpFrogDraw(Location location,float _angle)const;		//ジャンプ中カエルの描画
	void IdolFrogDraw(Location location,bool _direction)const;		//通常カエルの描画
	void Move(GameMain* _g);		//移動系処理
	void UpdataState(GameMain* _g);	//カエルの状態更新
	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};

