#include "DxLib.h"
#include "BackGround.h"

BackGround::BackGround()
{
	bg_erea.width = 6000;
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
	Location shift_location = { -_camera_location.x / (stage_erea.width / (bg_erea.width/5)) ,-_camera_location.y / (stage_erea.height / bg_erea.height) };
	for (int i = 0; i < 50; i++)
	{
		DrawBox(shift_location.x, shift_location.y + (i * 25),
			bg_erea.width + shift_location.x, shift_location.y + (i+1) * 25,
				GetColor((i * 3), (i * 3), 255), true);
		//DrawFormatStringF(shift_location.x + (i * 25), shift_location.y+i* 25, 0x00ff00,"%d", i);

	}
	DrawStringF(shift_location.x, 300.0f, "左端", 0x00ff00);
	DrawStringF((bg_erea.width - 50) + shift_location.x, 300.0f, "右端", 0x00ff00);
}