#include "Stage.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(int _type) : frame(0), old_color(0),inv_flg(false), debug_flg(false), anim(0), hit_flg(false), hit_timer(-1)
{
	//炎
	if (_type == 3 || _type == 6)
	{
		type = FIRE;
	}
	//木
	else if (_type == 4 || _type == 7)
	{
		type = WOOD;
	}
	//水
	else if (_type == 5 || _type == 8)
	{
		type = WATER;
	}
	//それ以外
	else
	{
		type = BLOCK;
	}
	block_type = _type;
	can_hit = TRUE;
}

Stage::~Stage()
{

}

void Stage::Initialize(Location _location, Erea _erea, int _color_data,int _object_pos)
{
	location = _location;
	erea = _erea;
	SetColorData(color_data[_color_data]);
	//色を交換出来るブロックの設定
	if (_color_data == 3 || _color_data == 4 || _color_data == 5)
	{
		can_swap = TRUE;
	}
	//色を交換出来ないブロックの設定
	else
	{
		can_swap = FALSE;
	}
	old_color = color;

	object_pos = _object_pos;
}

void Stage::Update(GameMain* _g)
{
	//EditもUpdateを呼べるようにこの書き方
	Update();
}

void Stage::Update()
{
	//色が変わったらブロックタイプを対応した種類に変える処理
	if (old_color != color)
	{
		if (color == RED)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = RED_BLOCK;
			}
			else
			{
				block_type = FIRE_BLOCK;
			}
			type = FIRE;
		}
		else if (color == GREEN)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = GREEN_BLOCK;
			}
			else
			{
				block_type = WOOD_BLOCK;
			}
			type = WOOD;
		}
		else if (color == BLUE)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = BLUE_BLOCK;
			}
			else
			{
				block_type = WATER_BLOCK;
			}
			type = WATER;
		}
		//更新
		old_color = color;
	}

	//リセット
	hit_flg = false;

	frame++;	
	//hit_timerに0が入ったらアニメーション開始
	if (hit_timer >= 0)
	{
		if (++hit_timer > 10)
		{
			//アニメーション終了
			hit_timer = -1;
		}
	}
	////色ブロック
	//if (block_type == RED_BLOCK || block_type == GREEN_BLOCK || block_type == BLUE_BLOCK)
	//{
	//	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	//	{
	//		if (effect_anim[i].time < 0)
	//		{
	//			effect_anim[i].shift.x = GetRand(38);
	//			effect_anim[i].shift.y = GetRand(38);
	//			effect_anim[i].erea.width = erea.width / 10;
	//			effect_anim[i].erea.height = erea.height / 10;
	//			effect_anim[i].time = 30 + GetRand(30);
	//		}
	//		else
	//		{
	//			effect_anim[i].time--;
	//			effect_anim[i].erea.width -= 0.05;
	//			effect_anim[i].erea.height -= 0.05;
	//		}
	//	}
	//}
}

void Stage::Draw()const
{
	if (inv_flg == false)
	{
		switch (block_type)
		{
			//無
		case NULL_BLOCK:
			break;
			//地面(赤、緑、青)
		case RED_BLOCK:
		case GREEN_BLOCK:
		case BLUE_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			//色付きブロックだけエフェクトを出す
		/*	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(effect_anim[i].shift.x + local_location.x,
					effect_anim[i].shift.y + local_location.y,
					effect_anim[i].shift.x + effect_anim[i].erea.width + local_location.x,
					effect_anim[i].shift.y + effect_anim[i].erea.height + local_location.y, 0xffffff, true);
			}
		*/	break;
			//地面（白）
		case WHITE_BLOCK:
			DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
			//地面（灰）
		case GRAY_BLOCK:
			DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xaaaaaa, true);
			DrawBox(local_location.x+10, local_location.y+20, local_location.x + 15, local_location.y + 25, 0x999999, true);
			DrawBox(local_location.x+30, local_location.y+15, local_location.x + 35, local_location.y + 20, 0x999999, true);
			DrawBox(local_location.x+25, local_location.y+35, local_location.x + 30, local_location.y + 40, 0x999999, true);
			break;
		//ダメージゾーンの描画
		case FIRE_BLOCK:
		case WOOD_BLOCK:
		case WATER_BLOCK:
			ResourceManager::StageAnimDraw(local_location, type);
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
		case PLAYER_BLOCK:
			//初期スポーン地点を分かりやすく
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xffff00, true);
			DrawStringF(local_location.x, local_location.y, "プレイヤー", text_color[block_type]);
			break;
		case ENEMY_DEER_RED:
		case ENEMY_DEER_GREEN:
		case ENEMY_DEER_BLUE:
			DrawStringF(local_location.x, local_location.y,"鹿",text_color[block_type]);
			break;
		case ENEMY_BAT_RED:
		case ENEMY_BAT_GREEN:
		case ENEMY_BAT_BLUE:
			DrawStringF(local_location.x, local_location.y, "蝙蝠", text_color[block_type]);
			break;
		case ENEMY_FROG_RED:
		case ENEMY_FROG_GREEN:
		case ENEMY_FROG_BLUE:
			DrawStringF(local_location.x, local_location.y, "蛙", text_color[block_type]);
			break;
		case ENEMY_BOSS:
			DrawStringF(local_location.x, local_location.y, "ボス", text_color[block_type]);
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

void Stage::Hit(Object* _object)
{
	//上から何かがぶつかったなら、ブロックを揺らす
	//if (_type == PLAYER)
	//{
	//	hit_timer = 0;
	//}
}

void Stage::SetStageType(int _type)
{
	block_type = _type;
	if (block_type < 9)
	{
		SetColorData(color_data[block_type]);
	}
}