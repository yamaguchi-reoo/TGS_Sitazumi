#include "Stage.h"
#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(int _type) : frame(0), old_color(0),inv_flg(false), debug_flg(false), anim(0), fire_anim{ 0 }, wood_anim{ 0 }, hit_flg(false), hit_timer(-1)
{
	//炎
	if (_type == 3 || _type == 6)
	{
		type == FIRE;
	}
	//木
	else if (_type == 4 || _type == 7)
	{
		type == WOOD;
	}
	//水
	else if (_type == 5 || _type == 8)
	{
		type == WATER;
	}
	//それ以外
	else
	{
		type = BLOCK;
	}
	block_type = _type;
	if (block_type > 5)
	{
		can_hit = FALSE;
	}
	else
	{
		can_hit = TRUE;
	}
}

Stage::~Stage()
{

}

void Stage::Initialize(Location _location, Erea _erea, int _color_data)
{
	location = _location;
	erea = _erea;
	//色を交換出来るブロックの設定
	if (_color_data == 1 || _color_data == 3 || _color_data == 4 || _color_data == 5)
	{
		can_swap = TRUE;
		SetColorData(color_data[_color_data]);
	}
	//色を交換出来ないブロックの設定
	else
	{
		can_swap = TRUE;
		color = 0;
	}
	old_color = color;
	//エフェクトのアニメーション用初期定義
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		fire_anim[i].time = GetRand(20);
		/*int rand = GetRand(10);*/
		wood_anim[i].shift = GetRand(10);
		wood_anim[i].shift1.x = (erea.width / 10) * i;
		wood_anim[i].shift2.x = (erea.width / 10) * i;
		wood_anim[i].shift1.y = erea.height;
		wood_anim[i].shift2.y = GetRand(19);
		water_anim[i].shift = (float)GetRand(9)/10;
		water_anim[i].shift1.x = GetRand(8)* (erea.width / 10);
		water_anim[i].shift1.y = GetRand(8)* (erea.height / 10);
		water_anim[i].erea.width = erea.width/3;
		water_anim[i].erea.height = erea.height/3;
	}

}
void Stage::Update(GameMain* _g)
{
	//色が変わったらブロックタイプも変える
	if (old_color != color)
	{
		if (color == WHITE)
		{
			block_type = 1;
		}
		if (color == RED)
		{
			block_type = 3;
		}
		if (color == GREEN)
		{
			block_type = 4;
		}
		if (color == BLUE)
		{
			block_type = 5;
		}
	}
	old_color = color;
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
	//色ブロック
	if (block_type == RED_BLOCK || block_type == GREEN_BLOCK || block_type == BLUE_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (effect_anim[i].time < 0)
			{
				effect_anim[i].shift.x = GetRand(38);
				effect_anim[i].shift.y = GetRand(38);
				effect_anim[i].erea.width = erea.width / 10;
				effect_anim[i].erea.height = erea.height / 10;
				effect_anim[i].time = 30 + GetRand(30);
			}
			else
			{
				effect_anim[i].time--;
				effect_anim[i].erea.width -= 0.05;
				effect_anim[i].erea.height -= 0.05;
			}
		}
	}
	//炎エフェクト用
	if (block_type == FIRE_BLOCK)
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
	if (block_type == WOOD_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (anim < 30)
			{
				wood_anim[i].shift2.x += (wood_anim[i].shift / 50);
			}
			else
			{
				wood_anim[i].shift2.x -= (wood_anim[i].shift / 50);
			}
		}
	}
	//水エフェクト用
	if (block_type == WATER_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (anim <= 30)
			{
				water_anim[i].shift1.y += water_anim[i].shift;
			}
			else
			{
				water_anim[i].shift1.y -= water_anim[i].shift;
			}
		}
	}
}

void Stage::Update()
{
	//色が変わったらブロックタイプも変える
	if (old_color != color)
	{
		if (color == WHITE)
		{
			block_type = 1;
		}
		if (color == RED)
		{
			block_type = 3;
		}
		if (color == GREEN)
		{
			block_type = 4;
		}
		if (color == BLUE)
		{
			block_type = 5;
		}
	}
	old_color = color;
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
	//色ブロック
	if (block_type == RED_BLOCK || block_type == GREEN_BLOCK || block_type == BLUE_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (effect_anim[i].time < 0)
			{
				effect_anim[i].shift.x = GetRand(38);
				effect_anim[i].shift.y = GetRand(38);
				effect_anim[i].erea.width = erea.width / 10;
				effect_anim[i].erea.height = erea.height / 10;
				effect_anim[i].time = 30 + GetRand(30);
			}
			else
			{
				effect_anim[i].time--;
				effect_anim[i].erea.width -= 0.05;
				effect_anim[i].erea.height -= 0.05;
			}
		}
	}
	//炎エフェクト用
	if (block_type == FIRE_BLOCK)
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
	if (block_type == WOOD_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (anim < 30)
			{
				wood_anim[i].shift2.x += (wood_anim[i].shift / 50);
			}
			else
			{
				wood_anim[i].shift2.x -= (wood_anim[i].shift / 50);
			}
		}
	}
	//水エフェクト用
	if (block_type == WATER_BLOCK)
	{
		for (int i = 0; i < ANIM_BLOCK_NUM; i++)
		{
			if (anim <= 30)
			{
				water_anim[i].shift1.y += water_anim[i].shift;
			}
			else
			{
				water_anim[i].shift1.y -= water_anim[i].shift;
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
			//地面(赤、緑、青)
		case RED_BLOCK:
		case GREEN_BLOCK:
		case BLUE_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			//色付きブロックだけエフェクトを出す
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(effect_anim[i].shift.x + local_location.x,
					effect_anim[i].shift.y + local_location.y,
					effect_anim[i].shift.x + effect_anim[i].erea.width + local_location.x,
					effect_anim[i].shift.y + effect_anim[i].erea.height + local_location.y, 0xffffff, true);
			}
			break;
			//地面(白)
		case WHITE_BLOCK:
			DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
			//地面（灰）
		case GRAY_BLOCK:
			DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xaaaaaa, true);
			break;
			//炎
		case FIRE_BLOCK:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xff0000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(fire_anim[i].shift.x + local_location.x,
					fire_anim[i].shift.y + local_location.y,
					fire_anim[i].shift.x + fire_anim[i].erea.width + local_location.x,
					fire_anim[i].shift.y + fire_anim[i].erea.height + local_location.y, 0xff0000, true);
			}
			break;
			//木
		case WOOD_BLOCK:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x00ff00, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawLineAA(wood_anim[i].shift1.x + local_location.x,
					wood_anim[i].shift1.y + local_location.y,
					wood_anim[i].shift2.x + local_location.x,
					wood_anim[i].shift2.y + local_location.y, 0x00ff00, true);
			}
			break;
			//水
		case WATER_BLOCK:
			DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x0000ff, true);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(water_anim[i].shift1.x + local_location.x,
					water_anim[i].shift1.y + local_location.y,
					water_anim[i].shift1.x + water_anim[i].erea.width + local_location.x,
					water_anim[i].shift1.y + water_anim[i].erea.height + local_location.y, 0x0000ee, true);
			}
			break;
			//その他（無）
		default:
			break;
		}
	}
	//Edit用表示
	if (debug_flg == true)
	{
		//敵文字表示用
		switch (block_type)
		{
		case 9:
			//初期スポーン地点を分かりやすく
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xffff00, true);
			DrawStringF(local_location.x, local_location.y, "プレイヤー", text_color[block_type]);
			break;
		case 10:
		case 11:
		case 12:
			DrawStringF(local_location.x, local_location.y,"鹿",text_color[block_type]);
			break;
		case 13:
		case 14:
		case 15:
			DrawStringF(local_location.x, local_location.y, "蝙蝠", text_color[block_type]);
			break;
		case 16:
		case 17:
		case 18:
			DrawStringF(local_location.x, local_location.y, "蛙", text_color[block_type]);
			break;
		default:
			//ブロックなら数字を表示
			DrawFormatStringF(local_location.x, local_location.y, text_color[block_type], "%d", block_type);
			break;
		}
	}
}

void Stage::Finalize()
{

}

void Stage::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	//上から何かがぶつかったなら、ブロックを揺らす
	//if (_type == PLAYER)
	//{
	//	hit_timer = 0;
	//}
}

bool Stage::GetStageCollisionType()
{
	bool col_type = false;
	//１～５は当たり判定有り
	if (block_type >= 1 && block_type <= 5)
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