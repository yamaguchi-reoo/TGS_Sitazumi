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
	//DrawQuadrangle
	//é®
	DrawTriangle(850, 300, 850, 325, 862, 312, GetColor(255, 255, 255), TRUE);
	DrawTriangle(880, 300, 880, 325, 868, 312, GetColor(255, 255, 255), TRUE);
	//ì™
	DrawQuadrangle(865, 316, 880, 330, 865, 340, 850, 330, GetColor(255, 255, 255), TRUE);
	//ì∑ëÃ
	DrawQuadrangle(865, 345, 880, 368, 865, 395, 850, 368, GetColor(255, 255, 255), TRUE);
	//âEóÉ
	DrawTriangle(877, 344, 917, 310, 943, 385, GetColor(255, 255, 255), TRUE);
	DrawTriangle(877, 344, 917, 310, 916, 380, GetColor(255, 255, 255), TRUE);
	DrawTriangle(875, 344, 917, 310, 895, 375, GetColor(255, 255, 255), TRUE);
	//ç∂óÉ
	/*DrawTriangle(853, 344, 917, 310, 943, 385, GetColor(255, 255, 255), TRUE);
	DrawTriangle(853, 344, 917, 310, 916, 380, GetColor(255, 255, 255), TRUE);
	DrawTriangle(853, 344, 917, 310, 895, 375, GetColor(255, 255, 255), TRUE);*/


}
