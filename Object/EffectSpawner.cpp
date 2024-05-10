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
	case 1:		
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


