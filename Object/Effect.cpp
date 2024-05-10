#include "Effect.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Effect::Effect(int _effect_type, float _angle, int _time) :frame(0),effect_type(0),touch_ground(false), angle(0)
{
	effect_type = _effect_type;
	angle = _angle;
	effect_time = _time;
	type = EFFECT;
	can_swap = FALSE;
	can_hit = FALSE;
}

Effect::~Effect()
{

}

void Effect::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	object_pos = _object_pos;
}

void Effect::Update(GameMain* _g)
{
	frame++;
	//各エフェクトの更新
	switch (effect_type)
	{
		//直進移動
	case 0:
		location.x += 2 * cosf(angle * (float)M_PI * 2);
		location.y += 2 * sinf(angle * (float)M_PI * 2);
		//指定された時間で消える
		if (frame > effect_time)
		{
			_g->DeleteObject(object_pos);
		}
		break;
		//回転移動
	case 1:
		location.x += 2 * cosf(angle * (float)M_PI * 2);
		location.y += 2 * sinf(angle * (float)M_PI * 2);
		//指定された時間で消える
		if (frame > effect_time)
		{
			_g->DeleteObject(object_pos);
		}
		break;
	default:
		break;
	}
}

void Effect::Draw()const
{
	//各エフェクトの更新
	switch (effect_type)
	{
	case 0:
		DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
		break;
	case 1:
		ResourceManager::DrawRotaBox(local_location.x, local_location.y, erea.width, erea.height, local_location.x, local_location.y, frame * 10, color, TRUE);
		break;
	default:
		break;
	}
}

void Effect::Finalize()
{

}

void Effect::Hit(Object* _object)
{
	touch_ground = true;
}