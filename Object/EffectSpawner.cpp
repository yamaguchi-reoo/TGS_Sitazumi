#include "EffectSpawner.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

EffectSpawner::EffectSpawner() :frame(0), swap_anim_timer(0),swap_se(0)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		effect[i].spawn_flg = false;
		effect[i].timer = 0;						 
		effect[i].location = { 0,0 };				 
		effect[i].local_location = { 0.0 };			 
		effect[i].erea = { 0,0 };					 
		effect[i].effect_type = 0;					 
		effect[i].effect_time = 0;					 
		effect[i].touch_ground = false;				 
		effect[i].angle = 0.0f;						 
		effect[i].speed = 0.0f;						 
		effect[i].color = WHITE;					 
		effect[i].effect_shift[0] = { 0,0 };		 
		effect[i].effect_shift[1] = { 0,0 };		 
		effect[i].shine_timer = 0;					 
		effect[i].rad = 0.0f;						 
	}
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
	swap_anim[0].move_flg = false;
	swap_anim[1].move_flg = false;	
	swap_anim_timer = 0;

	swap_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap.wav");
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
			//輝きエフェクト(移動あり)
			case 4:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
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

				effect[i].rad = (float)(effect[i].timer * M_PI / 180);

				effect[i].effect_shift[0].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[0].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				effect[i].rad = (float)((effect[i].timer+90) * M_PI / 180);

				effect[i].effect_shift[1].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[1].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//オブジェクトが着地した時
			case 5:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//爆発
			case 6:
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

	//色交換エフェクトの更新
	for (int i = 0; i < 2; i++)
	{
		//ローカル座標の更新
		swap_anim[i].local_location.x = swap_anim[i].location.x - _g->GetCameraLocation().x - (SWAP_EFFECT_SIZE / 2);
		swap_anim[i].local_location.y = swap_anim[i].location.y - _g->GetCameraLocation().y - (SWAP_EFFECT_SIZE / 2);
		//エフェクトの移動
		if (swap_anim[i].move_flg == true)
		{
			swap_anim[i].location.x += swap_anim[0].speed * cosf(swap_anim[i].move_rad);
			swap_anim[i].location.y += swap_anim[1].speed * sinf(swap_anim[i].move_rad);
			SpawnEffect(swap_anim[i].location, swap_anim[i].erea, 1, 5, swap_anim[i].color);
		}
		if (--swap_anim[i].timer < SWAP_EFFECT_STOP_TIME)
		{
			swap_anim[i].move_flg = false;
		}
		if (swap_anim[i].timer < SWAP_EFFECT_STOP_TIME && swap_anim[i].timer > 0)
		{
			swap_anim_timer++;
		}
		else
		{
			swap_anim_timer = 0;
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
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.width, effect[i].local_location.y + effect[i].erea.height, effect[i].color, TRUE);
				break;
				//回転四角形直進移動
			case 1:
				ResourceManager::DrawRotaBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].erea.width, effect[i].erea.height, effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].timer * 10, effect[i].color, TRUE);
				break;
				//四角形直進移動(フェードアウト)
			case 2:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255/effect[i].effect_time)));
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.width, effect[i].local_location.y + effect[i].erea.height, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
				//輝き
			case 3:
			case 4:
				DrawLineAA(effect[i].local_location.x - (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[0].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[0].y / 2), effect[i].color, TRUE);
				DrawLineAA(effect[i].local_location.x - (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[1].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[1].y / 2), effect[i].color, TRUE);
				break;
				//着地した時
			case 5:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.width, effect[i].local_location.y + effect[i].erea.height, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
				//爆発した時
			case 6:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
				DrawCircleAA(effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].timer * 15, 100, effect[i].color);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
			default:
				break;
			}
		}
	}

	//色交換エフェクトの描画
	for (int i = 0; i < 2; i++)
	{
		if (swap_anim[i].move_flg == true)
		{
			DrawBoxAA(swap_anim[i].local_location.x, swap_anim[i].local_location.y, swap_anim[i].local_location.x + 40, swap_anim[i].local_location.y + 40, swap_anim[i].color, true);
		}
		if (swap_anim_timer > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - (swap_anim_timer * 20));
			DrawBoxAA(swap_anim[i].local_location.x - (swap_anim_timer * 10), swap_anim[i].local_location.y - (swap_anim_timer * 10), swap_anim[i].local_location.x + swap_anim[i].erea.width + (swap_anim_timer * 5), swap_anim[i].local_location.y + swap_anim[i].erea.height + (swap_anim_timer * 5), swap_anim[i].color, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
}

void EffectSpawner::Finalize()
{

}

void EffectSpawner::SpawnEffect(Location _location, Erea _erea,int _effect_type, int _time, int _color, float _angle)
{
	switch (_effect_type)
	{
	//プレイヤースポーンエフェクト
	case PlayerSpawnEffect:
		for (float j = 0.0f; j < 1; j += 0.125f)
		{
			SpawnParticle(
				_location,
				_erea,
				2,
				_time,
				2,
				_color,
				j
			);
		}
		break;
	//輝き
	case ShineEffect:
		SpawnParticle(
			{ _location.x + GetRand((int)(_erea.width)),_location.y + GetRand((int)(_erea.height))},
			_erea,
			3,
			_time,
			1,
			_color,
			0
		);
		break;
		//塵になって消える
	case DeathEffect:
		SpawnParticle(
			{ _location.x + GetRand((int)(_erea.width)),_location.y + GetRand((int)(_erea.height))},
			_erea,
			4,
			_time,
			1,
			_color,
			(float)(GetRand(100) / 100.0f)
		);
		break;
		//着地エフェクト
	case LandingEffect:
		for (int i = 0; i < 7; i++)
		{
			SpawnParticle(
				{ _location.x + (_erea.width / 2) + GetRand(10),_location.y + _erea.height },
				{ 10,5 },
				5,
				_time,
				5,
				_color,
				(float)((GetRand(12) + (GetRand(1) * 38)) / 100.0f)
			);
		}
		break;
		//爆発エフェクト
	case ExplosionEffect:
		SpawnParticle(
			_location,
			_erea,
			6,
			_time,
			1,
			_color,
			0
		);
		break;
		//ダメージエフェクト
	case DamageEffect:
		SpawnParticle(
			_location,
			_erea,
			6,
			_time,
			1,
			_color,
			_angle
		);
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

int EffectSpawner::Swap(Object* _object1, Object* _object2)
{
	swap_anim[0].location = _object1->GetCenterLocation();
	swap_anim[1].location = _object2->GetCenterLocation();
	swap_anim[0].start = _object1->GetCenterLocation();
	swap_anim[1].start = _object2->GetCenterLocation();
	swap_anim[0].erea = { SWAP_EFFECT_SIZE ,SWAP_EFFECT_SIZE };
	swap_anim[1].erea = { SWAP_EFFECT_SIZE ,SWAP_EFFECT_SIZE };
	swap_anim[0].move_flg = true;
	swap_anim[1].move_flg = true;
	swap_anim[0].move_rad = atan2f(_object2->GetCenterLocation().y - _object1->GetCenterLocation().y,
								   _object2->GetCenterLocation().x - _object1->GetCenterLocation().x);
	swap_anim[1].move_rad = atan2f(_object1->GetCenterLocation().y - _object2->GetCenterLocation().y,
								   _object1->GetCenterLocation().x - _object2->GetCenterLocation().x);
	swap_anim[0].color = _object1->GetColorData();
	swap_anim[1].color = _object2->GetColorData();

	//移動にかかる時間測定
	Location move;
	float g;
	move.x = fabsf(_object1->GetCenterLocation().x - _object2->GetCenterLocation().x);
	move.y = fabsf(_object1->GetCenterLocation().y - _object2->GetCenterLocation().y);
	g = powf(move.x, 2) + powf(move.y, 2);

	swap_anim[0].speed = (int)(sqrtf(g) / SWAP_EFFECT_SPEED);
	swap_anim[1].speed = swap_anim[0].speed;

	swap_anim[0].timer = (int)((sqrtf(g) / swap_anim[0].speed) * 0.9f) + SWAP_EFFECT_STOP_TIME;
	if (swap_anim[0].timer < 0)
	{
		swap_anim[0].timer = 0;
	}
	swap_anim[1].timer = swap_anim[0].timer;

	ResourceManager::SetSoundFreq(DEFAULT_FREQ);
	ResourceManager::StartSound(swap_se);

	return swap_anim[0].timer;
}

void EffectSpawner::SpawnParticle(Location _location, Erea _erea, int _type, int _time, float _speed,int _color, float _angle)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == false)
		{
			ResetEffect(i);
			effect[i].spawn_flg = true;
			effect[i].location = _location;
			effect[i].erea = _erea;
			effect[i].effect_type = _type;
			effect[i].effect_time = _time;
			effect[i].speed = _speed;
			effect[i].color = _color;
			effect[i].angle = _angle;
			break;
		}
	}
}


