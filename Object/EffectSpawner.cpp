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
			_g->CreateObject(new Effect(2, i, 30), spawn_location, spawn_erea, RED);
		}
		break;
	case 2:	
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


