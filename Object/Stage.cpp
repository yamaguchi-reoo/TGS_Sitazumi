#include "Stage.h"
#include"../Utility/ResourceManager.h"
#include"../Scene/GameMain.h"

#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(int _type, int _stage_height) : frame(0), old_color(0),inv_flg(false), debug_flg(false), anim(0), hit_flg(false), hit_timer(-1), weather(0), change_weather_flg(false), delete_fire(0), draw_wood_flg(false), set_respawn_flg(false),respawn_color(WHITE)
{
	//炎
	if (_type == RED_BLOCK || _type == FIRE_BLOCK)
	{
		type = FIRE;
	}
	//木
	else if (_type == GREEN_BLOCK || _type == WOOD_BLOCK)
	{
		type = WOOD;
	}
	//水
	else if (_type == BLUE_BLOCK || _type == WATER_BLOCK)
	{
		type = WATER;
	}
	//それ以外
	else
	{
		type = BLOCK;
	}
	block_type = _type;
	if (_type == PLAYER_RESPAWN_BLOCK || _type == WEATHER_NORMAL || _type == WEATHER_RAIN || _type == WEATHER_FIRE || _type == WEATHER_SEED)
	{
		can_hit = FALSE;
		weather = _type - WEATHER_NORMAL;
	}
	else
	{
		can_hit = TRUE;
	}
	stage_height = _stage_height;
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

	//天気更新ブロックなら、高さをステージの高さいっぱいに変更
	//if (block_type == WEATHER_RAIN || block_type == WEATHER_FIRE || block_type == WEATHER_SEED)
	//{
	//	location.y = 0;
	//	erea.height = stage_height;
	//}
}

void Stage::Update(GameMain* _g)
{
	//EditもUpdateを呼べるようにこの書き方
	Update();

	//中間地点ブロックを輝かせる
	if (block_type == PLAYER_RESPAWN_BLOCK && location.x == _g->GetPlayerRespawnLocation().x && location.y == _g->GetPlayerRespawnLocation().y + PLAYER_HEIGHT)
	{
		_g->SpawnEffect(location, erea, ShineEffect, 14, WHITE);
	}
	else
	{
		//このブロックの見た目の色を変える
		respawn_color = WHITE;
	}
	//天気の更新があったらする
	if (change_weather_flg == true && weather != _g->GetNowWeather())
	{
		_g->SetNowWeather(weather);
		change_weather_flg = false;
	}
	//このステージブロックが火なら、一定時間経過で消す
	if (type == FIRE && can_swap == FALSE && ++delete_fire > 60)
	{
		_g->DeleteObject(object_pos);
	}
	//フラグが立っているなら
	if (set_respawn_flg)
	{
		//プレイヤーリスポーン位置を更新する
		_g->SetPlayerRespawnLocation({ location.x,location.y - PLAYER_HEIGHT });
		//このブロックの見た目の色を変える
		respawn_color = RED;
		//フラグをfalseにする
		set_respawn_flg = false;
	}
}

void Stage::Update()
{
	//リセット
	draw_wood_flg = false;
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
			draw_wood_flg = false;
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
			//地面 (白)
		case WHITE_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
			//地面（灰）
		case GRAY_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width + 1, local_location.y + erea.height + 1, 0xaaaaaa, true);
			//地面内の小石
			DrawBoxAA(local_location.x + 10, local_location.y + 20, local_location.x + 15, local_location.y + 25, 0x999999, true);
			DrawBoxAA(local_location.x + 30, local_location.y + 15, local_location.x + 35, local_location.y + 20, 0x999999, true);
			DrawBoxAA(local_location.x + 25, local_location.y + 35, local_location.x + 30, local_location.y + 40, 0x999999, true);
			break;
			//地面(赤、緑、青)
		case RED_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
		case GREEN_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			DrawBoxAA(local_location.x + 10, local_location.y + 20, local_location.x + 15, local_location.y + 25, 0x00ee00, true);
			DrawBoxAA(local_location.x + 30, local_location.y + 15, local_location.x + 35, local_location.y + 20, 0x00ee00, true);
			DrawBoxAA(local_location.x + 25, local_location.y + 35, local_location.x + 30, local_location.y + 40, 0x00ee00, true);
			break;
		case BLUE_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, true);
			break;
			//ダメージゾーンの描画
		case FIRE_BLOCK:
		case WATER_BLOCK:
			if (debug_flg == false)
			{
				ResourceManager::StageAnimDraw(local_location, type);
			}
		case WOOD_BLOCK:
			if (debug_flg == false)
			{
				if (draw_wood_flg == true)
				{
					DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x84331F, true);
					DrawLineAA(local_location.x + 10, local_location.y + 20, local_location.x + 10, local_location.y + 30, 0x73220E, TRUE);
					DrawLineAA(local_location.x + 27, local_location.y + 30, local_location.x + 27, local_location.y + 40, 0x73220E, TRUE);
				}
				else
				{
					ResourceManager::StageAnimDraw(local_location, type);
				}
			}
			break;
		case PLAYER_RESPAWN_BLOCK:
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, 
						   local_location.x + erea.width, local_location.y + (erea.height / 4), 
						   local_location.x + (erea.width / 2), local_location.y + (erea.height / 2), respawn_color, TRUE);
			DrawBoxAA(local_location.x + (erea.width / 2) - 2, local_location.y, local_location.x + (erea.width / 2) + 2, local_location.y + erea.height, respawn_color, TRUE);
			DrawBoxAA(local_location.x, local_location.y + erea.height - 4, local_location.x + erea.width, local_location.y + erea.height, respawn_color, TRUE);
			break;
			//天気更新ブロックは何も表示しなくても良いかも
		case WEATHER_NORMAL:
		case WEATHER_RAIN:
		case WEATHER_FIRE:
		case WEATHER_SEED:
			DrawStringF(local_location.x, local_location.y, "天気", text_color[block_type]);
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, text_color[block_type], false);
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
		case FIRE_BLOCK:
		case WOOD_BLOCK:
		case WATER_BLOCK:
			DrawStringF(local_location.x, local_location.y, "dmg", text_color[block_type]);
			break;
		case PLAYER_BLOCK:
			//初期スポーン地点を分かりやすく
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xffff00, true);
			DrawStringF(local_location.x, local_location.y, "player", text_color[block_type]);
			break;
		case PLAYER_RESPAWN_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xff00ff, true);
			DrawStringF(local_location.x, local_location.y, "tyuukann", text_color[block_type]);
			break;
		case ENEMY_DEER_RED:
		case ENEMY_DEER_GREEN:
		case ENEMY_DEER_BLUE:
			DrawStringF(local_location.x, local_location.y,"deer",text_color[block_type]);
			break;
		case ENEMY_BAT_RED:
		case ENEMY_BAT_GREEN:
		case ENEMY_BAT_BLUE:
			DrawStringF(local_location.x, local_location.y, "bat", text_color[block_type]);
			break;
		case ENEMY_FROG_RED:
		case ENEMY_FROG_GREEN:
		case ENEMY_FROG_BLUE:
			DrawStringF(local_location.x, local_location.y, "frog", text_color[block_type]);
			break;
		case ENEMY_BOSS:
			DrawStringF(local_location.x, local_location.y, "boss", text_color[block_type]);
			break;
			//天気ゾーン
		case WEATHER_RAIN:
		case WEATHER_FIRE:
		case WEATHER_SEED:
			DrawStringF(local_location.x, local_location.y, "weather", text_color[block_type]);
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
	//プレイヤーに当たった時、自身が天気変更ブロックなら、対応した天気に変更させる
	if (_object->GetObjectType() == PLAYER && (block_type == WEATHER_NORMAL || block_type == WEATHER_RAIN || block_type == WEATHER_FIRE || block_type == WEATHER_SEED))
	{
		change_weather_flg = true;
	}

	//草ブロック同士が当たった場合、座標に応じて描画を切り替える
	if (_object->GetObjectType() == WOOD && this->type == WOOD)
	{
		//当たったオブジェクトより上か下に自分が居るなら、木の描画に切り替える
		if ((_object->GetLocation().y < this->location.y || _object->GetLocation().y > this->location.y + this->erea.height) && _object->GetLocation().x + (_object->GetErea().width / 2) > this->location.x && _object->GetLocation().x + (_object->GetErea().width / 2) < this->location.x + this->erea.width)
		{
			draw_wood_flg = true;
		}
	}

	//火ブロックと溶岩ブロックが当たっている場合は火を消さない
	if (this->type == FIRE && this->can_swap == FALSE && _object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE)
	{
		delete_fire = 0;
	}

	//プレイヤーに当たった時、このブロックがプレイヤーリスポーン位置設定ブロックなら、フラグを立てる
	if (_object->GetObjectType() == PLAYER && block_type == PLAYER_RESPAWN_BLOCK)
	{
		set_respawn_flg = true;
	}
}

void Stage::SetStageType(int _type)
{
	block_type = _type;
	if (block_type < 9)
	{
		SetColorData(color_data[block_type]);
	}
}

void Stage::DrawSolidBody(int _color)const
{
	//int second_color;
	//if (_color == RED)
	//{
	//	second_color = 0xee0000;
	//}
	//if (_color == BLUE)
	//{
	//	second_color = 0x0000ee;
	//}
	//if (_color == GREEN)
	//{
	//	second_color = 0x00ee00;
	//}
	//if (_color == WHITE)
	//{
	//	second_color = 0xeeeeee;
	//}
	//else
	//{
	//	second_color = _color - 0x111111;
	//}

	//DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, _color, true);
	//DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, second_color, false);
	//立体もどき
	/*DrawQuadrangleAA(local_location.x + 20, local_location.y - 20,
		local_location.x + erea.width + 20, local_location.y - 20,
		local_location.x + erea.width, local_location.y,
		local_location.x, local_location.y,
		second_color, TRUE);
	DrawQuadrangleAA(local_location.x + erea.width, local_location.y,
		local_location.x + erea.width + 20, local_location.y - 20,
		local_location.x + erea.width + 20, local_location.y + erea.height - 20,
		local_location.x + erea.width, local_location.y + erea.height,
		second_color, TRUE);
	DrawLine(local_location.x + erea.width, local_location.y, local_location.x + erea.width + 20, local_location.y - 20, _color, TRUE);
	DrawLine(local_location.x + erea.width, local_location.y + erea.height, local_location.x + erea.width + 20, local_location.y - 20 + erea.height, _color, TRUE);*/
}