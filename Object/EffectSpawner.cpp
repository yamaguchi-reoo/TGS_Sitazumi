#include "EffectSpawner.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

EffectSpawner::EffectSpawner() :frame(0)
{

}

EffectSpawner::~EffectSpawner()
{

}

void EffectSpawner::Initialize()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		ResetEffect(i);
	}
}

void EffectSpawner::Update(GameMain* _g)
{
	frame++;
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == true)
		{
			//ローカル座標の更新
			SetScreenPosition(_g->GetCameraLocation(), i);
			effect[i].timer++;
			switch (effect[i].effect_type)
			{
			//直進移動
			case 0:
			case 1:
			case 2:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//輝きエフェクト
			case 3:
				if (effect[i].timer < effect[i].effect_time / 2)
				{
					effect[i].shine_timer += 2;
				}
				else
				{
					effect[i].shine_timer -= 2;
				}

				effect[i].rad = effect[i].timer * M_PI / 180;

				effect[i].effect_shift[0].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[0].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				effect[i].rad = (effect[i].timer+90) * M_PI / 180;

				effect[i].effect_shift[1].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[1].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
			default:
				break;
			}
		}
	}
}

void EffectSpawner::Draw()const
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == true)
		{
			//各エフェクトの描画
			switch (effect[i].effect_type)
			{
				//四角形直進移動
			case 0:
				DrawBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.width, effect[i].local_location.y + effect[i].erea.height, effect[i].color, TRUE);
				break;
				//回転四角形直進移動
			case 1:
				ResourceManager::DrawRotaBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].erea.width, effect[i].erea.height, effect[i].local_location.x, effect[i].local_location.y, effect[i].timer * 10, effect[i].color, TRUE);
				break;
				//四角形直進移動(フェードアウト)
			case 2:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255/effect[i].effect_time)));
				DrawBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.width, effect[i].local_location.y + effect[i].erea.height, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
			case 3:
				DrawLine(effect[i].local_location.x - (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[0].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[0].y / 2), effect[i].color, true);
				DrawLine(effect[i].local_location.x - (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[1].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[1].y / 2), effect[i].color, true);
				break;
			default:
				break;
			}
		}
	}
}

void EffectSpawner::Finalize()
{

}

void EffectSpawner::SpawnEffect(Location _location, Erea _erea,int _effect_type, int _time, int _color)
{
	switch (_effect_type)
	{
	//プレイヤースポーンエフェクト
	case 0:
		for (float j = 0.0f; j < 1; j += 0.125f)
		{
			for (int i = 0; i < EFFECT_NUM; i++)
			{
				if (effect[i].spawn_flg == false)
				{
					ResetEffect(i);
					effect[i].spawn_flg = true;
					effect[i].location = _location;
					effect[i].erea = _erea;
					effect[i].effect_type = 2;
					effect[i].effect_time = _time;
					effect[i].speed = 2;
					effect[i].angle = j;
					effect[i].color = _color;
					break;
				}
			}
		}
		break;
	//輝き
	case 1:
		for (int i = 0; i < EFFECT_NUM; i++)
		{
			if (effect[i].spawn_flg == false)
			{
				ResetEffect(i);
				effect[i].spawn_flg = true;
				effect[i].location = { _location.x + GetRand(_erea.width),_location.y + GetRand(_erea.height) };	//指定のエリア内でランダムな座標にスポーン
				effect[i].erea = _erea;
				effect[i].effect_type = 3;
				effect[i].effect_time = _time;
				effect[i].color = _color;
				break;
			}
		}
		break;
	case 2:
		break;
	default:
		break;
}
}

void EffectSpawner::SetScreenPosition(Location _world_to_screen,int _num)
{
	effect[_num].local_location.x = effect[_num].location.x - _world_to_screen.x;
	effect[_num].local_location.y = effect[_num].location.y - _world_to_screen.y;
}

void EffectSpawner::ResetEffect(int _num)
{
	effect[_num].spawn_flg = false;
	effect[_num].timer = 0;
	effect[_num].location = { 0,0 };
	effect[_num].local_location = { 0.0 };
	effect[_num].erea = { 0,0 };
	effect[_num].effect_type = 0;
	effect[_num].effect_time = 0;
	effect[_num].touch_ground = false;
	effect[_num].angle = 0.0f;
	effect[_num].speed = 0.0f;
	effect[_num].color = WHITE;
	effect[_num].effect_shift[0] = { 0,0 };
	effect[_num].effect_shift[1] = { 0,0 };
	effect[_num].shine_timer = 0;
	effect[_num].rad = 0.0f;
}


