#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	number = 0;
	frame = 0;
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Update(GameMain* main)
{
	frame++;
	if (frame > 60)
	{
		frame = 0;
	}
}

void EnemyBat::Draw() const
{
	DrawFormatString(200, 200, GetColor(255, 255, 225), "%d   %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//耳
	DrawTriangle(850, 300, 850, 325, 862, 312, GetColor(255, 255, 255), FALSE);
	DrawTriangle(880, 300, 880, 325, 868, 312, GetColor(255, 255, 255), FALSE);
	//頭
	DrawQuadrangle(865, 316, 880, 330, 865, 340, 850, 330, GetColor(255, 255, 255), FALSE);
	//胴体
	DrawQuadrangle(865, 345, 880, 368, 865, 395, 850, 368, GetColor(255, 255, 255), FALSE);
	//右翼
	DrawTriangle(877, 344, 917, 310, 943, 385, GetColor(255, 255, 255), FALSE);
	DrawTriangle(877, 344, 917, 310, 916, 380, GetColor(255, 255, 255), FALSE);
	DrawTriangle(875, 344, 917, 310, 895, 375, GetColor(255, 255, 255), FALSE);
	//左翼
	DrawTriangle(853, 344, 816, 310, 787, 385, GetColor(255, 255, 255), FALSE);
	DrawTriangle(853, 344, 816, 310, 814, 380, GetColor(255, 255, 255), FALSE);
	DrawTriangle(855, 344, 816, 310, 835, 375, GetColor(255, 255, 255), FALSE);
}
