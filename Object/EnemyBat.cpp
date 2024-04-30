#include"EnemyBat.h"
#include"../Utility/KeyInput.h"
#include<math.h>
#include "Player.h"
#include"../Scene/GameMain.h"

#define PI 3.141592654f

#define ENEMY_SPEED 2

EnemyBat::EnemyBat() :up(0), bat_state(BatState::LEFT), wing_angle(0.0f), vector{ 0.0f }
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;
	for (int i = 0; i < 4; i++){
		move[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = { _location };//x座標 ,y座標 //590 ,400
	erea = { _erea };		//高さ、幅	//100,150
	color = _color_data;

	object_pos = _object_pos;
}

void EnemyBat::Update(GameMain* _g)
{
	// 羽の角度を変化させる
	wing_angle = sin(PI * 2 / 120 * up) * 30; // 30度の振れ幅で周期的に変化させる
	//移動
	Move();

	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	if (_g->GetSearchFlg()) {
		location.x += vector.x * 0.1f;
		location.y += vector.y * 0.1f;
	}
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
	
	Location player_posi = Player::player_pos;
	// プレイヤーとの距離を計算
	vector = { 2.f };
	float dx = player_posi.x - location.x;
	float dy = player_posi.y - location.y;
	float length = sqrt(dx * dx + dy * dy);

	// プレイヤーとの距離が一定以下の場合、プレイヤーを追跡
	if (length < 400) {
		// 移動方向を決定
		dx /= length;
		dy /= length;

		// 移動する
		location.x += dx * (vector.x + 1);
		location.y += dy * (vector.y + 1);
	}
	else {
		//左移動
		if (bat_state == BatState::LEFT) {
			location.x -= vector.x;
			location.y += sin(PI * 2 / 40 * up) * 5;

		}
		//右移動
		if (bat_state == BatState::RIGHT) {
			location.x += vector.x;
			location.y -= sin(PI * 2 / 40 * up) * 5;
		}
	}
}

void EnemyBat::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	//ブロックと当たった時の処理
	if (_type == BLOCK)
	{
		Location tmpl = location;
		Erea tmpe = erea;
		move[0] = 0;
		move[1] = 0;
		move[2] = 0;
		move[3] = 0;

		//上下判定用に座標とエリアの調整
		location.x += 10.f;
		erea.height = 1.f;
		erea.width = tmpe.width - 15.f;

		//プレイヤー上方向の判定
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤー下方向の判定
		location.y += tmpe.height + 1;
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][bottom]) {
			stageHitFlg[0][bottom] = true;
			stageHitFlg[1][bottom] = true;
		}
		else {
			stageHitFlg[0][bottom] = false;
		}

		//戻す
		location.x = tmpl.x;
		location.y = tmpl.y;
		erea.height = tmpe.height;
		erea.width = tmpe.width;

		//上方向に埋まらないようにする
		if (stageHitFlg[0][top]) {//上方向に埋まっていたら
			float t = (_location.y + _erea.height) - location.y;
			if (t != 0) {
				vector.y = 0.f;
				move[top] = t;
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _location.y - (location.y + erea.height);
			if (t != 0) {
				move[bottom] = t;
			}
		}


		//左右判定用に座標とエリアの調整
		location.y += 3.f;
		erea.height = tmpe.height - 3.f;
		erea.width = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_location, _erea);
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.width + 1;
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][right]) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}

		//最初の値に戻す

		location.x = tmpl.x;
		location.y += -3.f;
		erea.height = tmpe.height;
		erea.width = tmpe.width;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_location.x + _erea.width) - location.x;
			if (t != 0) {
				vector.x = 0.f;
				move[left] = t;
				bat_state = BatState::RIGHT;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _location.x - (location.x + erea.width);
			if (t != 0) {
				vector.x = 0.f;
				move[right] = t;
				bat_state = BatState::LEFT;
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		if (location.y < _location.y + _erea.height && location.y + erea.height > _location.y) {//左右
			if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
				move[left] = 0.f;
				move[right] = 0.f;
			}
		}

		location.x += move[left];
		location.x += move[right];
		location.y += move[top];
		location.y += move[bottom];

		erea.height = tmpe.height;
		erea.width = tmpe.width;

	}
	if ((_type == FIRE && this->color == GREEN) || (_type == WATER && this->color == RED) || (_type == WOOD && this->color == BLUE))
	{
		bat_state = BatState::DEATH;
	}
}

bool EnemyBat::CheckCollision(Location l, Erea e)
{
	bool ret = false;

	//自分の左上座標
	float my_x = location.x;
	float my_y = location.y;
	//自分の中央座標
	float my_cx = my_x + (erea.width / 2);
	float my_cy = my_y + (erea.height / 2);
	//自分の幅と高さの半分
	float my_harf_width = erea.width / 2;
	float my_harf_height = erea.height / 2;

	//相手の左上座標
	float sub_x = l.x;
	float sub_y = l.y;
	//相手の中央座標
	float sub_cx = sub_x + (e.width / 2);
	float sub_cy = sub_y + (e.height / 2);
	//相手の幅と高さの半分
	float sub_harf_width = e.width / 2;
	float sub_harf_height = e.height / 2;

	//自分と相手の中心座標の差
	float diff_x = my_cx - sub_cx;
	float diff_y = my_cy - sub_cy;

	//当たり判定の演算
	if (fabsf(diff_x) < my_harf_width + sub_harf_width &&
		fabsf(diff_y) < my_harf_height + sub_harf_height)
	{
		ret = true;
	}
	return ret;
}
