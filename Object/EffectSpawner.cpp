#include "EffectSpawner.h"
#include "../Scene/GameMain.h"

EffectSpawner::EffectSpawner() :frame(0), spawn_order(0)
{

}

EffectSpawner::~EffectSpawner()
{

}

void EffectSpawner::Initialize()
{

}

void EffectSpawner::Update(GameMain* _g)
{
	frame++;

	switch (spawn_order)
	{
	//何もスポーンさせない
	case 0:		
		break;
		//プレイヤースポーンエフェクト
	case 1:		
		for (float i = 0.0f; i < 1; i += 0.125f)
		{
			_g->CreateObject(new Effect(2, i, 20, 2), spawn_location, spawn_erea, RED);
			_g->CreateObject(new Effect(2, i, 20, 2.5), spawn_location, spawn_erea, RED);
			_g->CreateObject(new Effect(2, i, 20, 3), spawn_location, spawn_erea, RED);
		}
		break;
		//輝き
	case 2:	
		_g->CreateObject(new Effect(3, 0, 20, 2), spawn_location, spawn_erea, WHITE);
		break;
	case 3:	
		break;
	default:
		break;
	}
	//リセット
	spawn_order = 0;
}

void EffectSpawner::Draw()const
{

}

void EffectSpawner::Finalize()
{

}

void EffectSpawner::SpawnEffect(Location _location, Erea _erea,int _effect_type)
{
	spawn_order = _effect_type;
	spawn_location = _location;
	spawn_erea = _erea;
}


