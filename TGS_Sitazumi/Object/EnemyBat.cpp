#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	x_move = 0;
	local_location.x = 877;
	local_location.y = 300;
	erea.width = 120;
	erea.height = 100;
	//vertices = {
	//	{850,300,850,325,862,312},			//耳
	//	{880,300,880,325,868,312},			//耳
	//	{865,316,880,330,865,340,850,330},  //頭
	//	{865,345,880,368,865,395,850,330},	//胴体
	//	{877,344,917,310,943,385},			//右翼
	//	{877,344,917,310,916,380},
	//	{875,344,917,310,895,375},
	//	{853,334,816,310,787,385},			//左翼
	//	{853,334,816,310,814,380},
	//	{855,334,816,310,835,375},
	//};
	vertices = {

	};
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Update(GameMain* main)
{
	if (++x_move > 60) {
		x_move = 0;
	}
	Move();
}

void EnemyBat::Draw() const
{
	//DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height,GetColor(255,255,255),FALSE);
	//DrawFormatString(200, 200, GetColor(255, 255, 225), "%d   %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	////耳
	//DrawTriangle(850 + x_move, 300, 850 + x_move, 325, 862 + x_move, 312, GetColor(255, 255, 255), TRUE);
	//DrawTriangle(880 + x_move, 300, 880 + x_move, 325, 868 + x_move, 312, GetColor(255, 255, 255), TRUE);
	////頭
	//DrawQuadrangle(865 + x_move, 316, 880 + x_move, 330, 865 + x_move, 340, 850 + x_move, 330, GetColor(255, 255, 255), TRUE);
	////胴体
	//DrawQuadrangle(865 + x_move, 345, 880 + x_move, 368, 865 + x_move, 395, 850 + x_move, 368, GetColor(255, 255, 255), TRUE);
	////右翼
	//DrawTriangle(877 + x_move, 344, 917 + x_move, 310, 943 + x_move, 385, GetColor(255, 255, 255), TRUE);
	//DrawTriangle(877 + x_move, 344, 917 + x_move, 310, 916 + x_move, 380, GetColor(255, 255, 255), TRUE);
	//DrawTriangle(875 + x_move, 344, 917 + x_move, 310, 895 + x_move, 375, GetColor(255, 255, 255), TRUE);
	////左翼
	//DrawTriangle(853 + x_move, 344, 816 + x_move, 310, 787 + x_move, 385, GetColor(255, 255, 255), TRUE);
	//DrawTriangle(853 + x_move, 344, 816 + x_move, 310, 814 + x_move, 380, GetColor(255, 255, 255), TRUE);
	//DrawTriangle(855 + x_move, 344, 816 + x_move, 310, 835 + x_move, 375, GetColor(255, 255, 255), TRUE);

		//耳
	DrawTriangle(850, 300, 850, 325, 862, 312, GetColor(255, 255, 255), TRUE);
	DrawTriangle(880, 300, 880, 325, 868, 312, GetColor(255, 255, 255), TRUE);
	//頭
	DrawQuadrangle(865, 316, 880, 330, 865, 340, 850, 330, GetColor(255, 255, 255), TRUE);
	//胴体
	DrawQuadrangle(865, 345, 880, 368, 865, 395, 850, 368, GetColor(255, 255, 255), TRUE);
	//右翼
	DrawTriangle(877, 344, 917, 310, 943, 385, GetColor(255, 255, 255), TRUE);
	DrawTriangle(877, 344, 917, 310, 916, 380, GetColor(255, 255, 255), TRUE);
	DrawTriangle(875, 344, 917, 310, 895, 375, GetColor(255, 255, 255), TRUE);
	//左翼
	DrawTriangle(853, 344, 816, 310, 787, 385, GetColor(255, 255, 255), TRUE);
	DrawTriangle(853, 344, 816, 310, 814, 380, GetColor(255, 255, 255), TRUE);
	DrawTriangle(855, 344, 816, 310, 835, 375, GetColor(255, 255, 255), TRUE);
}

void EnemyBat::Move()
{

}

