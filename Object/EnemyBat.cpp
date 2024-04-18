#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	type = ENEMY;
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
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Update()
{
	if (++x_move > 60) {
		x_move = 0;
	}
	Move();
	BatArray();
}

void EnemyBat::Draw() const
{
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

void EnemyBat::BatArray()
{
	//const int vertex_count = 32;
	Location vertices[32] = {
		// 耳
		{850, 300}, {850, 325}, {862, 312},
		{880, 300}, {880, 325}, {868, 312},
		// 頭
		{865, 316}, {880, 330}, {865, 340}, {850, 330},
		// 胴体
		{865, 345}, {880, 368}, {865, 395}, {850, 368},
		//右翼
		{877, 344}, {917, 310}, {943, 385},
		{877, 344}, {917, 310}, {916, 380},
		{875, 344}, {917, 310}, {895, 375},
		//左翼
		{853, 344}, {816, 310}, {787, 385},
		{853, 344}, {816, 310}, {814, 380},
		{855, 344}, {816, 310}, {835, 375},
	};
}

void EnemyBat::Hit(Location _location, Erea _erea, int _type)
{

}
