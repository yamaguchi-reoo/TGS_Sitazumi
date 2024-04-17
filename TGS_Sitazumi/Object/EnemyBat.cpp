#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
	type = ENEMY;
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
	// 配列の各頂点を利用して三角形を描画する
	for (int i = 0; i < NUM_VERTICES - 2; i += 3) {
		if (i < 22) {
			DrawTriangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,GetColor(255, 255,255), TRUE);
		}
		//ひし形の描画
		else
		{
			DrawQuadrangle(vertices[i].x, vertices[i].y,vertices[i + 1].x, vertices[i + 1].y,vertices[i + 2].x, vertices[i + 2].y,vertices[i + 3].x, vertices[i + 3].y,GetColor(255, 255, 255), TRUE);
			i++;
		}
	}
	DrawFormatString(300, 300, 0xfffff, "%f    %f", vertices[0].x, vertices[0].y);
}

void EnemyBat::Move()
{
	// vertices の各要素の座標を local_location にコピーする
	for (int i = 0; i < NUM_VERTICES; ++i) {
		local_location.x = vertices[i].x;
		local_location.y = vertices[i].y;
	}
	
}

void EnemyBat::BatArray()
{
}

void EnemyBat::Hit(Location _location, Erea _erea, int _type)
{

}
