#include "DxLib.h"
#include "BackGround.h"

BackGround::BackGround():stage_erea{0.0}, now_stage(0), is_clear(false)
{
	bg_erea.width = 12000;
	bg_erea.height = 2000;
}

BackGround::~BackGround()
{

}

void BackGround::Initialize(Erea _stage_erea)
{
	stage_erea = _stage_erea;

	font_handle[0] = CreateFontToHandle("Franklin Gothic", 48, -1, -1);
	font_handle[1] = CreateFontToHandle("Wingdings", 48, -1, -1);
	bg_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, TRUE);
}

void BackGround::Finalize()
{

}

void BackGround::Update()
{

}

void BackGround::Draw(Location _camera_location)const
{
	Location shift_location = { -_camera_location.x / (stage_erea.width / (bg_erea.width / 12)) ,-_camera_location.y / (stage_erea.height / (bg_erea.height / 10)) };
	int r = 0, g = 0, b = 0;
	float bg_color = (_camera_location.x / 80);


	for (int i = 0; i < 50; i++)
	{
		if (now_stage == 2 && is_clear == false)
		{
			r = 0;
			g = 0;
			b = 0;
		}
		else
		{
			if ((i * 3) + 50 - (int)bg_color > 255)
			{
				r = 255;
				g = 255;
			}
			else if ((i * 3) + 50 - (int)bg_color < 0)
			{
				r = 0;
				g = 0;
			}
			else
			{
				r = (i * 3) + 50 - (int)bg_color;
				g = (i * 3) + 50 - (int)bg_color;
			}
			if (255 - (int)bg_color < 0)
			{
				b = 0;
			}
			else
			{
				b = 255 - (int)bg_color;
			}
		}
		//青空
		DrawBoxAA(shift_location.x, shift_location.y + (i * 25),
			bg_erea.width + shift_location.x, shift_location.y + (i + 1) * 26,
			GetColor(r, g, b), true);
	}

	if (now_stage != 2)
	{
		//後でランダムな形で生成するようにする
		DrawMountain({ shift_location.x - 200, shift_location.y - 50 }, { 300,300 }, 0.1f);
		DrawMountain({ shift_location.x - 6400, shift_location.y }, { 310,300 }, 0.1f);
		DrawMountain({ shift_location.x - 8400, shift_location.y + 50 }, { 200,300 }, 0.1);
		DrawMountain(shift_location, { 150,150 }, 0.2f);
		DrawMountain({ shift_location.x - 500, shift_location.y - 50 }, { 140,70 }, 0.3f);
		DrawMountain({ shift_location.x - 500, shift_location.y - 50 }, { 140,70 }, 0.3f);
		DrawMountain({ shift_location.x - 400, shift_location.y - 100 }, { 140,70 }, 0.35f);
		DrawMountain({ shift_location.x - 300, shift_location.y - 50 }, { 110,150 }, 0.5f);
	}

	//GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_handle);	//背景をハンドルに保存
	//ClearDrawScreen();													// 画面の初期化
	//GraphFilter(bg_handle, DX_GRAPH_FILTER_GAUSS, 16, 300);			//保存した背景にぼかしをかける
	//DrawGraph(0, 0, bg_handle, TRUE);
	//SetDrawScreen(DX_SCREEN_BACK);

	for (int i = 0; i < 50; i++)
	{
		if (GetRand((int)bg_color) > 100)
		{
			if (GetRand(1) == 1)
			{
				int rand = GetRand(35);
				DrawFormatStringToHandle(rand * 40, i * 40,GetColor(GetRand(1) * 100, GetRand(1) * 100, GetRand(1) * 100),font_handle[1],"%d",GetRand(9));
			}
			else
			{
				int rand = GetRand(35);
				DrawBox(rand * 40, i * 40,
					rand * 40 + 40, i * 40 + 40,
					GetColor(GetRand(1) * 100, GetRand(1) * 100, GetRand(1) * 100), TRUE);
			}
		}
	}

#ifdef _DEBUG
	DrawStringF(shift_location.x, 300.0f, "左端", 0x00ff00);
	DrawStringF((bg_erea.width - 50) + shift_location.x, 300.0f, "右端", 0x00ff00);
#endif
}

void BackGround::DrawWood(Location _shift_location, float move_speed)const
{

}

void BackGround::DrawMountain(Location _shift_location,Erea _erea ,float move_speed)const
{

	Location shift_location = { _shift_location.x * move_speed,_shift_location.y * move_speed };
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.height, _erea.width, 50, 0x007700, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.height, _erea.width, 50, 0x000000, FALSE);
	DrawBoxAA(SCREEN_WIDTH + shift_location.x - _erea.width,
		SCREEN_HEIGHT - shift_location.y - _erea.height,
		SCREEN_WIDTH + shift_location.x + _erea.width,
		SCREEN_HEIGHT - shift_location.y,
		0x007700, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x - _erea.width,
		SCREEN_HEIGHT - shift_location.y - _erea.height,
		SCREEN_WIDTH + shift_location.x - _erea.width,
		SCREEN_HEIGHT - shift_location.y,
		0x000000, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x + _erea.width,
		SCREEN_HEIGHT - shift_location.y - _erea.height,
		SCREEN_WIDTH + shift_location.x + _erea.width,
		SCREEN_HEIGHT - shift_location.y,
		0x000000, TRUE);

#ifdef _DEBUG
	DrawStringF(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.height, "山", 0x000000);
#endif
}