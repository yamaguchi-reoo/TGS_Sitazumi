#pragma once
#include "BoxCollider.h"

class GameMain;

class EffectSpawner
{
private:
	int frame;		//フレーム測定
	int spawn_order;	//GameMainからエフェクトのスポーン指示を受け取る
	Location spawn_location;	//エフェクトの左上座標
	Erea spawn_erea;

public:
	EffectSpawner();
	~EffectSpawner();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();

	void SpawnEffect(Location _location,Erea _erea,int _effect_type);
};

