#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;

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

}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Initialize(Location _location, Erea _erea, int _color_data)
{
	location = _location;
	erea = _erea;
	color = _color_data;
}

void EnemyBat::Update()
{
	/*Move();
	BatArray();*/
}

void EnemyBat::Draw() const
{
	//配列の各頂点を利用して三角形を描画する
	for (int i = 0; i < vertices.size(); i += 3) {
		if (i < 22) {
			//DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
			DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
		}
		//ひし形の描画
		else
		{
			DrawQuadrangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,vertices[i + 3].x, vertices[i + 3].y,GetColor(255, 255, 255), TRUE);
			i++;
		}
	}

	//配列の各頂点を利用して三角形を描画する
	DrawFormatString(300, 300, 0xfffff, "%f    %f", local_location.x, local_location.y);
}

void EnemyBat::Finalize()
{

}

void EnemyBat::Move()
{
}

void EnemyBat::BatArray()
{

}

void EnemyBat::Hit(Location _location, Erea _erea, int _type, int _color_data)
{

}
