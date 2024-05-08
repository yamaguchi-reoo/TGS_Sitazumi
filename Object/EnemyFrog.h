#pragma once
#include "Object.h"


enum class FrogState {
	LEFT_JUMP=0,
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
	int frame;				//フレーム測定
	int timer;				//時間測定
	FrogState frog_state;	//カエルの状態

	bool death_flg;		//生きているか
	Location vector; //カエルのベクトル
	bool stageHitFlg[2][4]; //カエルとステージの当たり判定
	float move[4];			//各方向加速度格納用

	float face_angle;		//顔の向き
	int death_timer;	//死亡演出
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

	void Move(GameMain* _g);		//移動系処理
	void UpdataState(GameMain* _g);	//カエルの状態更新
	bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
};

