#include "Stage.h"

Stage::Stage(float _x, float _y, float _width, float _height, int _type): inv_flg(false), debug_flg(false)
{
	location.x = _x;
	location.y = _y;
	erea.height = _height;
	erea.width = _width;
	type = _type;	
	color = 0;
	//色を交換できるステージブロックの設定
	if (type == 1 || type == 3 || type == 4 || type == 5)
	{
		swap_flg = true;
		SetColorData(color_data[type]);
	}
	//色を交換できないステージブロックの設定
	else
	{
		swap_flg = false;
	}
}

Stage::~Stage()
{

}

void Stage::Update()
{
	//現在の色情報に応じてブロックの色を変える
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
			//地面(灰)
		case 2:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, 0xaaaaaa, true);
			break;
			break;
			//炎
		case 6:
			DrawStringF(local_location.x, local_location.y, "炎", 0xff0000);
			break;
			//木
		case 7:
			DrawStringF(local_location.x, local_location.y, "木", 0x00ff00);
			break;
			//水
		case 8:
			DrawStringF(local_location.x, local_location.y, "水", 0x0000ff);
			break;
			//無
		default:
			break;
		}
	}
	//Editでの表示
	if (debug_flg == true && type < 6)
	{
		DrawFormatStringF(local_location.x, local_location.y, text_color[type], "%d", type);
	}
}
int Stage::GetStageCollisionType()
{
	int col_type = 0;
	if (type <= 5)
	{
		col_type = type;
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