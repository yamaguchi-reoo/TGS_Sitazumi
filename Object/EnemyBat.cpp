#include"EnemyBat.h"
#include"../Utility/KeyInput.h"
#include<math.h>

#define PI 3.141592654f

#define ENEMY_SPEED 2

EnemyBat::EnemyBat()
{
	type = ENEMY;
	wing_angle = 0.0f;
	can_swap = TRUE;
	can_hit = TRUE;

	up = 0;
	/*location = { 790 ,600 };

	erea = { 100,100 };*/

	bat_state = BatState::LEFT;
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = { 590 ,400 };//x座標 ,y座標
	erea = { 100,150 };		//高さ、幅
	color = _color_data;

	object_pos = _object_pos;
}

void EnemyBat::Update(GameMain* _g)
{
	// 羽の角度を変化させる
	wing_angle = sin(PI * 2 / 120 * up) * 30; // 30度の振れ幅で周期的に変化させる
	Move();
}

void EnemyBat::Draw() const
{
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, GetColor(255, 255, 255), FALSE);

	//各頂点をlocal_locationに置き換えた
	const std::vector<Location> vertices = {
		// 耳
		{local_location.x + 60, local_location.y}, {local_location.x + 60, local_location.y + 25}, {local_location.x + 72, local_location.y + 12},
		{local_location.x + 90, local_location.y}, {local_location.x + 90, local_location.y + 25}, {local_location.x + 78, local_location.y + 12},
		//右翼
		{local_location.x + 87, local_location.y + 44}, {local_location.x + 127, local_location.y + 10}, {local_location.x + 153, local_location.y + 85},
		{local_location.x + 87, local_location.y + 44}, {local_location.x + 127, local_location.y + 10}, {local_location.x + 126, local_location.y + 80},
		{local_location.x + 85, local_location.y + 44}, {local_location.x + 127, local_location.y + 10}, {local_location.x + 105, local_location.y + 75},
		//左翼
		{local_location.x + 63, local_location.y + 44}, {local_location.x + 26, local_location.y + 10}, {local_location.x - 3, local_location.y + 85},
		{local_location.x + 63, local_location.y + 44}, {local_location.x + 26, local_location.y + 10}, {local_location.x + 24, local_location.y + 80},
		{local_location.x + 65, local_location.y + 44}, {local_location.x + 26, local_location.y + 10}, {local_location.x + 45, local_location.y + 75},
		// 頭
		{local_location.x + 75, local_location.y + 16}, {local_location.x + 90, local_location.y + 30}, {local_location.x + 75, local_location.y + 40}, {local_location.x + 60, local_location.y + 30},
		// 胴体
		{local_location.x + 75, local_location.y + 45}, {local_location.x + 90, local_location.y + 68}, {local_location.x + 75, local_location.y + 95}, {local_location.x + 60, local_location.y + 68},
	};


	//配列の各頂点を利用して三角形を描画する
	for (int i = 0; i < vertices.size(); i += 3) {
		//耳
		if (i < 5) {
			//DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
			DrawTriangleAA(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,color, TRUE);
		}
		//右羽
		else if (i > 5 && i < 14) {
			// 羽の動き
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x , vertices[i + 2].y + wing_angle, color, TRUE);
		}
		//左羽
		else if (i > 14 && i < 23) {
			// 羽の動き
			DrawTriangleAA(vertices[i].x, vertices[i].y , vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x , vertices[i + 2].y + wing_angle, color, TRUE);
		}
		//ひし形の描画
		else
		{
			DrawQuadrangleAA(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,vertices[i + 3].x, vertices[i + 3].y, color, TRUE);
			i++;
		}
	}
	////耳
	//DrawTriangleAA(local_location.x, local_location.y, local_location.x, local_location.y + 25, local_location.x + 12, local_location.y + 12,GetColor(255, 255, 255), TRUE);
	//DrawTriangleAA(local_location.x + 30, local_location.y, local_location.x + 30, local_location.y + 25, local_location.x + 18, local_location.y + 12,GetColor(255, 255, 255), TRUE);
	////右翼
	//DrawTriangleAA(local_location.x + 27, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 93, local_location.y + 85,GetColor(255, 255, 255), TRUE);
	//DrawTriangleAA(local_location.x + 27, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 66, local_location.y + 80,GetColor(255, 255, 255), TRUE);
	//DrawTriangleAA(local_location.x + 25, local_location.y + 44, local_location.x + 67, local_location.y + 10, local_location.x + 45, local_location.y + 75,GetColor(255, 255, 255), TRUE);
	////左翼
	//DrawTriangleAA(local_location.x + 3, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 63, local_location.y + 85, GetColor(255, 255, 255), TRUE);
	//DrawTriangleAA(local_location.x + 3, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 36, local_location.y + 80, GetColor(255, 255, 255), TRUE);
	//DrawTriangleAA(local_location.x + 5, local_location.y + 44, local_location.x - 34, local_location.y + 10, local_location.x - 15, local_location.y + 75, GetColor(255, 255, 255), TRUE);
	////頭
	//DrawQuadrangleAA(local_location.x + 15, local_location.y + 16, local_location.x + 30, local_location.y + 30, local_location.x + 15, local_location.y + 40, local_location.x, local_location.y + 30, GetColor(255, 255, 255), TRUE);
	////胴体
	//DrawQuadrangleAA(local_location.x + 15, local_location.y + 45, local_location.x + 30, local_location.y + 68, local_location.x + 15, local_location.y + 95, local_location.x, local_location.y + 68, GetColor(255, 255, 255), TRUE);


	//if (leftwall_flg == true) {
	//}
	//配列の各頂点を利用して三角形を描画する
	
}

void EnemyBat::Finalize()
{

}

void EnemyBat::Move()
{
	up += 1;
	//左移動
	if (bat_state == BatState::LEFT) {
		location.x -= ENEMY_SPEED;
		location.y += sin(PI * 2 / 40  * up) * 5;

	}
	//右移動
	if (bat_state == BatState::RIGHT) {
		location.x += ENEMY_SPEED;
		location.y -= sin(PI * 2 / 40 * up) * 5;
	}
	

}

void EnemyBat::BatArray()
{

}

void EnemyBat::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	if (_type == BLOCK)
	{
		//右壁に当たった時
		if (location.x < _location.x + _erea.width && location.y + erea.height - 12 > _location.y)
		{
			bat_state = BatState::LEFT;
		}
		//左壁に当たった時
		if (location.x > _location.x && location.y + erea.height - 12 > _location.y)
		{
			bat_state = BatState::RIGHT;
		}
		//床に当たった時
		if (location.y + erea.height - 24 < _location.y) {
			location.y = _location.y - erea.height;
		}
	}
}
