#include "Stage.h"
#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(float _x, float _y, float _width, float _height, int _type): frame(0),inv_flg(false), debug_flg(false), anim(0), effect_anim{0}
{
	location.x = _x;
	location.y = _y;
	erea.height = _height;
	erea.width = _width;
	type = _type;	
	color = 0;
	//色を交換出来るブロックの設定
	if (type == 1 || type == 3 || type == 4 || type == 5)
	{
		swap_flg = true;
		SetColorData(color_data[type]);
	}
	//色を交換出来ないブロックの設定
	else
	{
		swap_flg = false;
	}
	//エフェクトのアニメーション用初期定義
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		effect_anim[i].time = GetRand(40);
	}

}

Stage::~Stage()
{

}

void Stage::Update()
{
	frame++;
	//アニメーション用変数
	if (++anim > 60)
	{
		anim = 0;
	}
	//炎エフェクト用
	if (type == 6)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (effect_anim[i].time < 0)
			{
				effect_anim[i].location.x = local_location.x;
				effect_anim[i].location.y = local_location.y + erea.height;
				effect_anim[i].erea.width = erea.width / 10;
				effect_anim[i].erea.height = erea.height / 10;
				effect_anim[i].location.x += (erea.width / 10) * GetRand(10);
				effect_anim[i].time = 30 + GetRand(30);
				effect_anim[i].angle = GetRand(3);
			}
			else
			{
				effect_anim[i].location.x += 0.05 + (effect_anim[i].angle-1)/2;
				effect_anim[i].location.y -= 1;
				effect_anim[i].time--;
				effect_anim[i].erea.width -= 0.05;
				effect_anim[i].erea.height -= 0.05;
			}
		}
	}
}

void Stage::Draw()const
{
	if (inv_flg == false)
	{
		switch (type)
		{
			//無
		case 0:
			break;
			//地面(白、赤、緑、青)
		case 1:
		case 3:
		case 4:
		case 5:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
			//地面（灰）
		case 2:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xaaaaaa, true);
			break;
			//炎
		case 6:
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(effect_anim[i].location.x, effect_anim[i].location.y, effect_anim[i].location.x + effect_anim[i].erea.width, effect_anim[i].location.y + effect_anim[i].erea.height, 0xff0000, true);
			}
			break;
			//木
		case 7:
			DrawStringF(local_location.x, local_location.y, "��", 0x00ff00);
			break;
			//水
		case 8:
			DrawStringF(local_location.x, local_location.y, "��", 0x0000ff);
			break;
			//その他（無）
		default:
			break;
		}
	}
	//Edit用表示
	if (debug_flg == true)
	{
		//初期スポーン地点を分かりやすく4
		if (type == PLAYER_SPAWN_NUM)
		{
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xffff00, true);
		}
		DrawFormatStringF(local_location.x, local_location.y, text_color[type], "%d", type);
	}
}
int Stage::GetStageCollisionType()
{
	int col_type = 0;
	//１～５は当たり判定有り
	if (type > 0 && type <= 5)
	{
		col_type = 1;
	}
	return col_type;
}

void Stage::SetStageType(int _type) 
{ 
	type = _type; 
	if (type == 1 || type == 3 || type == 4 || type == 5)
	{
		SetColorData(color_data[type]);
	}
}