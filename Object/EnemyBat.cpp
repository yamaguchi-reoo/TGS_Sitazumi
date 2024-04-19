#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	type = ENEMY;
	leftwall_flg = false;

	can_swap = TRUE;
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


	location = { 850 ,600 };
	erea = { 100,100 };

	bat_state = BatState::LEFT;
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
	DrawBoxAA(local_location.x - 68, local_location.y, local_location.x + erea.width, local_location.y + erea.height, GetColor(255, 255, 255), FALSE);

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

	//↓修正予定
	//耳
	DrawTriangleAA(local_location.x, local_location.y, local_location.x, local_location.y + 25, local_location.x + 12, local_location.y + 12,GetColor(255, 255, 255), TRUE);
	DrawTriangleAA(local_location.x + 30, local_location.y, local_location.x + 30, local_location.y + 25, local_location.x + 18, local_location.y + 12,GetColor(255, 255, 255), TRUE);
	//右翼
	DrawTriangleAA(local_location.x + 27, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 93, local_location.y + 85,GetColor(255, 255, 255), TRUE);
	DrawTriangleAA(local_location.x + 27, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 66, local_location.y + 80,GetColor(255, 255, 255), TRUE);
	DrawTriangleAA(local_location.x + 25, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 45, local_location.y + 75,GetColor(255, 255, 255), TRUE);
	//左翼
	DrawTriangleAA(local_location.x + 3, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 63, local_location.y + 85, GetColor(255, 255, 255), TRUE);
	DrawTriangleAA(local_location.x + 3, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 36, local_location.y + 80, GetColor(255, 255, 255), TRUE);
	DrawTriangleAA(local_location.x + 5, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 15, local_location.y + 75, GetColor(255, 255, 255), TRUE);
	//頭
	DrawQuadrangleAA(local_location.x + 15, local_location.y + 16, local_location.x + 30, local_location.y + 30, local_location.x + 15, local_location.y + 40, local_location.x, local_location.y + 30, GetColor(255, 255, 255), TRUE);
	//胴体
	DrawQuadrangleAA(local_location.x + 15, local_location.y + 45, local_location.x + 30, local_location.y + 68, local_location.x + 15, local_location.y + 95, local_location.x, local_location.y + 68, GetColor(255, 255, 255), TRUE);


	//if (leftwall_flg == true) {
	DrawFormatString(300, 300, 0xfffff, "%f    %f", local_location.x, local_location.y);
	//}
	//配列の各頂点を利用して三角形を描画する
	
}

void EnemyBat::Move()
{
	
	if (bat_state == BatState::LEFT) {
		location.x -= 3;
	}
	if (bat_state == BatState::RIGHT) {
		location.x += 3;
	}
}

void EnemyBat::BatArray()
{

}

void EnemyBat::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	if (_type == BLOCK)
	{
		bat_state = BatState::RIGHT;
	}
}
