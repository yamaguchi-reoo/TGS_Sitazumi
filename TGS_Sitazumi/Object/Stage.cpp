#include "Stage.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define DOWN 0	//下加速度用
#define UP 1	//上加速度用
#define RIGHT 2	//右加速度用
#define LEFT 3	//左加速度用

Stage::Stage(float _x, float _y, float _width, float _height, int _type)
{
	location.x = _x;
	location.y = _y;
	erea.height = _height;
	erea.width = _width;
	type = _type;
	inv_flg = false;
	for (int i = 0; i < 4; i++)
	{
		end_flg[i] = false;
	}
	draw_rad[UP] = 0;
	draw_rad[LEFT] = 0.75 * M_PI * 2;
	draw_rad[RIGHT] = 0.25 * M_PI * 2;
	draw_rad[DOWN] = 0.5 * M_PI * 2;
	debug_flg = true;
}

Stage::~Stage()
{

}

void Stage::Update()
{
	if (inv_flg == false) {
		draw_location[UP] = { local_location.x + (erea.width / 2),local_location.y + (erea.height / 2) };
		draw_location[DOWN] = { local_location.x + (erea.width / 2),local_location.y - (erea.height / 2) + erea.height };
		draw_location[LEFT] = { local_location.x + (erea.width / 2),local_location.y + (erea.height / 2) };
		draw_location[RIGHT] = { local_location.x - (erea.width / 2) + erea.width,local_location.y + (erea.height / 2) };
	}
}

void Stage::Draw()const
{
	//Editでの表示
	if (debug_flg == true)
	{
		switch (type)
		{
			//無
		case 0:
			DrawFormatStringF(local_location.x, local_location.y, 0xffffff, "%d", type);
			break;
			//地面
		case 1:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x4C444D, true);
			DrawFormatStringF(local_location.x, local_location.y, 0xB3BBB2, "%d", type);
			break;
			//木
		case 2:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x00ff00, true);
			DrawFormatStringF(local_location.x, local_location.y, 0xff00ff, "%d", type);
			break;
			//岩
		case 3:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x333333, true);
			DrawFormatStringF(local_location.x, local_location.y, 0xCCCCCC, "%d", type);
			break;
			//雲
		case 4:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xdddddd, true);
			DrawFormatStringF(local_location.x, local_location.y, 0x222222, "%d", type);
			break;
			//無
		default:
			DrawFormatStringF(local_location.x, local_location.y, 0xff0000, "%d", type);
			break;
		}
	}
	//ゲームメインでの表示（マップチップ）
	else
	{
		if (inv_flg == false)
		{
			//DrawGraph(local_location.x, local_location.y, stage_img[draw_type], true);
			switch (type)
			{
				//無
			case 0:
				break;
				//地面
			case 1:
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x4C444D, true);
				break;
				//木
			case 2:
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x00ff00, true);
				break;
				//岩
			case 3:
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0x333333, true);
				break;
				//雲
			case 4:
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xdddddd, true);
				break;
				//無
			default:
				break;
			}
			DrawGraphF(local_location.x, local_location.y, stage_img[0], true);
			for (int i = 0; i < 4; i++)
			{
				if (end_flg[i] == true)
				{
#ifdef _DEBUG
					DrawFormatStringF(local_location.x, local_location.y, 0x00ffff, "%d", i);
#endif
					DrawRotaGraphF(draw_location[i].x, draw_location[i].y, 1, draw_rad[i], stage_img[1], true);
				}
			}
		}
	}
}
int Stage::GetStageCollisionType()
{
	int col_type = 0;
	if (type <= 4)
	{
		col_type = type;
	}
	else
	{
		col_type = 0;
	}
	return col_type;
}

void Stage::SetDrawType(int up_type, int left_type, int right_type, int down_type)
{
	//上のブロックのタイプが自分と違う且つ画面端以外なら
	if (type != up_type && up_type != -1)
	{
		end_flg[UP] = true;
	}
	//左のブロックのタイプが自分と違う且つ画面端以外なら
	if (type != left_type && left_type != -1)
	{
		end_flg[LEFT] = true;
	}
	//右のブロックのタイプが自分と違う且つ画面端以外なら
	if (type != right_type && right_type != -1)
	{
		end_flg[RIGHT] = true;
	}
	//下のブロックのタイプが自分と違う且つ画面端以外なら
	if (type != down_type && down_type != -1)
	{
		end_flg[DOWN] = true;
	}

}