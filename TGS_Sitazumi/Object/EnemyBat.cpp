#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	type = ENEMY;
	leftwall_flg = false;

	vertices = {
		// 耳
		{850, 300}, {850, 325}, {862, 312},
		{880, 300}, {880, 325}, {868, 312},
		//右翼
		{877, 344}, {917, 310}, {943, 385},
		{877, 344}, {917, 310}, {916, 380},
		{875, 344}, {917, 310}, {895, 375},
		//左翼
		{853, 344}, {816, 310}, {787, 385},
		{853, 344}, {816, 310}, {814, 380},
		{855, 344}, {816, 310}, {835, 375},
		// 頭
		{865, 316}, {880, 330}, {865, 340}, {850, 330},
		// 胴体
		{865, 345}, {880, 368}, {865, 395}, {850, 368},
	};


	location = { 850 ,300 };
	erea = { 100,100 };
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Update()
{
	Move();
	BatArray();
}

void EnemyBat::Draw() const
{
	DrawBox(location.x - 50, location.y, location.x + erea.width, location.y + erea.height, GetColor(255, 255, 255), FALSE);

	//配列の各頂点を利用して三角形を描画する
	//for (int i = 0; i < vertices.size(); i += 3) {
	//	if (i < 22) {
	//		//DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
	//		DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
	//	}
	//	//ひし形の描画
	//	else
	//	{
	//		DrawQuadrangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,vertices[i + 3].x, vertices[i + 3].y,GetColor(255, 255, 255), TRUE);
	//		i++;
	//	}
	//}
	DrawTriangle(location.x, location.y, location.x, location.y + 25, location.x + 16, location.y + 12,GetColor(255, 255, 255), TRUE);

	//配列の各頂点を利用して三角形を描画する
	DrawFormatString(300, 300, 0xfffff, "%f    %f", location.x, location.y);
}

void EnemyBat::Move()
{
	location.x--;

}

void EnemyBat::BatArray()
{

}

void EnemyBat::Hit(Location _location, Erea _erea, int _type)
{

}
