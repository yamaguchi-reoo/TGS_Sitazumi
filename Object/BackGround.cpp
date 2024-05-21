#include "DxLib.h"
#include "BackGround.h"

BackGround::BackGround()
{
	bg_width = 1000;
}

BackGround::~BackGround()
{

}

void BackGround::Initialize(int _stage_wigth)
{
	stage_width = _stage_wigth;
}

void BackGround::Finalize()
{

}

void BackGround::Update()
{

}

void BackGround::Draw(Location _camera_location)const
{
	DrawStringF(-_camera_location.x / (stage_width / bg_width), 300.0f, "左端", 0x00ff00);
}