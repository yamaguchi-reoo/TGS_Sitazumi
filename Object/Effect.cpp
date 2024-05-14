#include "Effect.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Effect::Effect(int _effect_type, float _angle, int _time, float _speed) :frame(0),effect_type(0),touch_ground(false), angle(0), speed(0), effect_shift{0,0}, shine_timer(0)
{
	effect_type = _effect_type;
	angle = _angle;
	effect_time = _time;
	speed = _speed;
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
	type = EFFECT;
	can_swap = FALSE;
	can_hit = TRUE;
}

void Effect::Update(GameMain* _g)
{
	frame++;
	//各エフェクトの更新
	switch (effect_type)
	{
		//直進移動
	case 0:
	case 1:
	case 2:
		location.x += speed * cosf(angle * (float)M_PI * 2);
		location.y += speed * sinf(angle * (float)M_PI * 2);
		//指定された時間で消える
		if (frame > effect_time)
		{
			_g->DeleteObject(object_pos);
		}
		break;
	case 3:
		if (frame < effect_time / 2)
		{
			shine_timer+=2;
		}
		else
		{
			shine_timer -= 2;
		}
		effect_shift[0].x = (shine_timer * cosf(frame * M_PI / 180)) - (shine_timer * sinf(frame * M_PI / 180));
		effect_shift[0].y = (shine_timer * sinf(frame * M_PI / 180)) + (shine_timer * cosf(frame * M_PI / 180));

		effect_shift[1].x = (shine_timer * cosf((frame + 90) * M_PI / 180)) - (shine_timer * sinf((frame + 90) * M_PI / 180));
		effect_shift[1].y = (shine_timer * sinf((frame + 90) * M_PI / 180)) + (shine_timer * cosf((frame + 90) * M_PI / 180));
		
		//指定された時間で消える
		if (frame > effect_time)
		{
			_g->DeleteObject(object_pos);
		}
	default:
		break;
	}
}

void Effect::Draw()const
{
	//各エフェクトの更新
	switch (effect_type)
	{
	//四角形直進移動
	case 0:
		DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
		break;
	//回転四角形直進移動
	case 1:
		ResourceManager::DrawRotaBox(local_location.x, local_location.y, erea.width, erea.height, local_location.x, local_location.y, frame * 10, color, TRUE);
		break;
	//四角形直進移動(フェードアウト)
	case 2:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (frame * (effect_time/15)));
		DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case 3:
		DrawLine(local_location.x - (effect_shift[0].x / 2), 
				 local_location.y - (effect_shift[0].y / 2), 
				 local_location.x + (effect_shift[0].x / 2), 
				 local_location.y + (effect_shift[0].y / 2), 0x00ff00, true);
		DrawLine(local_location.x - (effect_shift[1].x / 2),
				 local_location.y - (effect_shift[1].y / 2),
			  	 local_location.x + (effect_shift[1].x / 2),
				 local_location.y + (effect_shift[1].y / 2), 0x00ff00, true);	
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