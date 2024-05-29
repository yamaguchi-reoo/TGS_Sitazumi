#include "DxLib.h"
#include "BackGround.h"

BackGround::BackGround():stage_erea{0.0}
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
	for (int i = 0; i < 50; i++)
	{
		DrawBoxAA(shift_location.x, shift_location.y + (i * 25),
			bg_erea.width + shift_location.x, shift_location.y + (i + 1) * 25.5f,
			GetColor((i * 3), (i * 3), 200), true);
		//DrawFormatStringF(shift_location.x + (i * 25), shift_location.y+i* 25, 0x00ff00,"%d", i);

	}
	DrawMountain(shift_location, 0.5f);
	DrawMountain({shift_location.x - 300, shift_location.y-50}, 0.7f);
#ifdef _DEBUG
	DrawStringF(shift_location.x, 300.0f, "左端", 0x00ff00);
	DrawStringF((bg_erea.width - 50) + shift_location.x, 300.0f, "右端", 0x00ff00);
#endif
}

void BackGround::DrawWood(Location _shift_location, float move_speed)const
{

}

void BackGround::DrawMountain(Location _shift_location, float move_speed)const
{
	Location shift_location = { _shift_location.x * move_speed,_shift_location.y * move_speed };
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - 200, 200, 50, 0x00aa00, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - 200, 200, 50, 0x000000, FALSE);
	DrawBoxAA(SCREEN_WIDTH + shift_location.x - 200,
		SCREEN_HEIGHT - shift_location.y - 200,
		SCREEN_WIDTH + shift_location.x + 200,
		SCREEN_HEIGHT - shift_location.y,
		0x00aa00, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x - 200,
		SCREEN_HEIGHT - shift_location.y - 200,
		SCREEN_WIDTH + shift_location.x - 200,
		SCREEN_HEIGHT - shift_location.y,
		0x000000, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x + 200,
		SCREEN_HEIGHT - shift_location.y - 200,
		SCREEN_WIDTH + shift_location.x + 200,
		SCREEN_HEIGHT - shift_location.y,
		0x000000, TRUE);
#ifdef _DEBUG
	DrawStringF(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - 200, "山", 0x000000);
#endif
}