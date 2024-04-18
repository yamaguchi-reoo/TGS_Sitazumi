#include "Stage.h"
#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(float _x, float _y, float _width, float _height, int _type) : frame(0), inv_flg(false), debug_flg(false), anim(0), fire_anim{ 0 }, wood_anim{ 0 }, hit_flg(false), hit_timer(-1)
{
	type = BLOCK;
	location.x = _x;
	location.y = _y;
	erea.height = _height;
	erea.width = _width;
	block_type = _type;
	color = 0;
	//色を交換出来るブロックの設定
	if (block_type == 1 || block_type == 3 || block_type == 4 || block_type == 5)
	{
		can_swap = true;
		SetColorData(color_data[block_type]);
	}
	//色を交換出来ないブロックの設定
	else
	{
		can_swap = false;
	}
	//エフェクトのアニメーション用初期定義
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		fire_anim[i].time = GetRand(40);
		/*int rand = GetRand(10);*/
		wood_anim[i].shift = GetRand(10);
		wood_anim[i].shift1.x = (erea.width / 10) * i;
		wood_anim[i].shift2.x = (erea.width / 10) * i;
		wood_anim[i].shift1.y = erea.height;
		wood_anim[i].shift2.y = GetRand(20);
	}
}

Stage::~Stage()
{

}

void Stage::Update()
{
	//リセット
	hit_flg = false;
	frame++;
	//アニメーション用変数
	if (++anim > 60)
	{
		anim = 0;
	}
	//hit_timerに0が入ったらアニメーション開始
	if (hit_timer >= 0)
	{
		if (++hit_timer > 10)
		{
			//アニメーション終了
			hit_timer = -1;
		}
	}
	//炎エフェクト用
	if (block_type == 6)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (fire_anim[i].time < 0)
			{
				fire_anim[i].shift.x = 0;
				fire_anim[i].shift.y = erea.height;
				fire_anim[i].erea.width = erea.width / 10;
				fire_anim[i].erea.height = erea.height / 10;
				fire_anim[i].shift.x += (erea.width / 10) * GetRand(10);
				fire_anim[i].time = 30 + GetRand(30);
				fire_anim[i].angle = GetRand(3);
			}
			else
			{
				fire_anim[i].shift.x += 0.05 + (fire_anim[i].angle - 1) / 2;
				fire_anim[i].shift.y -= 1;
				fire_anim[i].time--;
				fire_anim[i].erea.width -= 0.05;
				fire_anim[i].erea.height -= 0.05;
			}
		}
	}
	//木エフェクト用
	if (block_type == 7)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (anim < 30)
			{
				wood_anim[i].shift2.x += (wood_anim[i].shift / 50) + 0.1;
			}
			else
			{
				wood_anim[i].shift2.x -= (wood_anim[i].shift / 50) + 0.1;
			}
		}
	}
}

void Stage::Draw()const
{
	if (inv_flg == false)
	{
		switch (block_type)
		{
			//無
		case 0:
			break;
			//地面(白、赤、緑、青)
		case WHITE_BLOCK:
		case RED_BLOCK:
		case GREEN_BLOCK:
		case BLUE_BLOCK:
			if (hit_timer >= 0)
			{
				DrawBoxAA(local_location.x, local_location.y + stage_shift[hit_timer], local_location.x + erea.width, local_location.y + erea.height + stage_shift[hit_timer], color, true);
			}
			else
			{
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			}
			break;
			//地面（灰）
		case GRAY_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xaaaaaa, true);
			break;
			//炎
		case FIRE:
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(fire_anim[i].shift.x + local_location.x,
					fire_anim[i].shift.y + local_location.y,
					fire_anim[i].shift.x + fire_anim[i].erea.width + local_location.x,
					fire_anim[i].shift.y + fire_anim[i].erea.height + local_location.y, 0xff0000, true);
			}
			break;
			//木
		case WOOD:
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawLineAA(wood_anim[i].shift1.x + local_location.x,
					wood_anim[i].shift1.y + local_location.y,
					wood_anim[i].shift2.x + local_location.x,
					wood_anim[i].shift2.y + local_location.y, 0x00ff00, true);
			}
			break;
			//水
		case WATER:
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
		//初期スポーン地点を分かりやすく
		if (block_type == PLAYER_SPAWN_NUM)
		{
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xffff00, true);
		}
		DrawFormatStringF(local_location.x, local_location.y, text_color[block_type], "%d", block_type);
	}
}

void Stage::Hit(Location _location, Erea _erea, int _type)
{
	//上から何かがぶつかったなら、ブロックを揺らす
	if (_type == PLAYER)
	{
		hit_timer = 0;
	}
}

bool Stage::GetStageCollisionType()
{
	bool col_type = false;
	//１、３、４、５は当たり判定有り
	if (block_type == 1 || block_type == 3 || block_type == 4 || block_type == 5)
	{
		col_type = true;
	}
	return col_type;
}

void Stage::SetStageType(int _type)
{
	block_type = _type;
	if (block_type == 1 || block_type == 3 || block_type == 4 || block_type == 5)
	{
		SetColorData(color_data[block_type]);
	}
}