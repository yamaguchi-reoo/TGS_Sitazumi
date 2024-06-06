#include "Boss.h"
#include <math.h>
#include "../Utility/KeyInput.h"
#include "../Scene/GameMain.h"
#include <algorithm>
#include <iostream>

#define BOSS_SIZE 250
#define STATE_CHANGE_INTERVAL 340
#define BOSS_MAX_SPEED 4.0f

#define ANGLE_SPEED 0.01f
#define RADIUS 300.0f


Boss::Boss() :vector{ 0.0f }, boss_state(BossState::ATTACK), barrier_num(3), damage_flg(false), state_change_time(0), speed(0.0f)
{
	type = BOSS;
	can_swap = TRUE;
	can_hit = TRUE;
	for (int i = 0; i < 4; i++) {
		move[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}
	for (int i = 0; i < barrier_num; i++)
	{
		barrier_rad[i] = 0;
	}

	//srand(time(0));
	//SetRandMove();
}

Boss::~Boss()
{
}

void Boss::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = { SCREEN_WIDTH - 200, SCREEN_HEIGHT - 300};//x座標 ,y座標 
	erea = { _erea };	   //高さ、幅	
	color = _color_data;

	object_pos = _object_pos;

	//ボスの羽画用配列
	vertices = {
		//右翼No1
		{ 190.0f, 105.0f }, { 230.0f, 90.0f },{ 245.0f, 105.0f },{ 205.0f, 120.0f },
		{ 240.0f, 85.0f }, { 257.5f, 77.5f },{ 272.5f, 92.5f },{ 255.0f, 100.0f },
		{ 270.0f, 72.5f }, { 310.0f, 57.5f },{ 325.0f, 72.5f },{ 285.0f, 87.5f },
		//右翼No2
		{ 210.0f, 135.0f }, { 250.0f,120.0f }, { 265.0f, 135.0f }, { 225.0f, 150.0f },
		{ 260.0f, 115.0f }, { 277.5f,107.5f }, { 292.5f, 122.5f }, { 275.0f, 130.0f },
		{ 290.0f, 102.5f }, { 330.0f, 87.5f }, { 345.0f, 102.5f }, { 305.0f, 117.5f }
	};

	warp_pos = {
		 {(SCREEN_WIDTH / 2 + 50) , 125},		   //中央
		 {SCREEN_WIDTH - 200, SCREEN_HEIGHT - 300},//右
		 {250 , SCREEN_HEIGHT - 300}				   //左
	};

	barrier_rad[0] = 60;
	barrier_rad[1] = 55;
	barrier_rad[2] = 50;

}

void Boss::Update(GameMain* _g)
{
	// ステージとの当たり判定フラグを初期化
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	speed = BOSS_MAX_SPEED;
	vector = { 1.0f ,1.0f };

	Location player_pos = _g->GetPlayerLocation();

	if (player_pos.x > 120) {

		switch (boss_state)
		{
		case BossState::MOVE:
			// ボスの移動処理を呼び出し
			Move(_g);
			break;
		case BossState::ATTACK:
			BossAtack(_g);
			break;
		case BossState::DEATH:
			_g->DeleteObject(object_pos);
			break;
		default:
			break;
		}
	}

	//プレイヤーとボスの距離を計算
	//DistanceCalc(_g);

	barrier();
	// ダメージを受けている場合の処理
	if (damage_flg) {
		// 時間を減少
		damage_effect_time--;
		// フラグを設定
		damage_effect_flg = true;
		// 時間が0以下になったら
		if (damage_effect_time <= 0)
		{
			// バリアを1つ減少
			barrier_num--;
			// ダメージフラグを解除
			damage_flg = false;
		}
	}

	//竹攻撃制作中
	/*if (GetRand(60) > 58 && !f)
	{
		Erea e = { 1200,40 };
		Location l = _g->GetPlayerLocation();
		l.y += _g->GetPlayerErea().height + 40;
		_g->CreateObject(new BossAttackWood, l, e, GREEN);
		f = true;
	}*/
	

}

void Boss::Draw() const
{
	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);

	//本体
	DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, 35, 35, 0x000000, TRUE);

	//羽描画
	for (int i = 0; i < vertices.size(); i += 4)
	{
		//DrawQuadrangleAA(local_location.x + vertices[i].x, local_location.y + vertices[i].y, local_location.x + vertices[i + 1].x, local_location.y + vertices[i + 1].y, local_location.x + vertices[i + 2].x, local_location.y + vertices[i + 2].y, local_location.x + vertices[i + 3].x, local_location.y + vertices[i + 3].y, color, TRUE);
	}

	// 羽の頂点を反転させて左側の座標を計算
	std::vector<Location> mirrored_vertices;
	//for (int i = 0; i < vertices.size(); ++i)
	//{
	//	const Location& vertex = vertices[i];
	//	float mirrored_x = center.x - (vertex.x - center.x); // X座標を反転
	//	mirrored_vertices.push_back({ mirrored_x, vertex.y });
	//}

	// 羽の描画（左側）
	for (size_t i = 0; i < mirrored_vertices.size(); i += 4)
	{
		//DrawQuadrangleAA(local_location.x + vertices[i].x, local_location.y + vertices[i].y, local_location.x + vertices[i + 1].x, local_location.y + vertices[i + 1].y, local_location.x + vertices[i + 2].x, local_location.y + vertices[i + 2].y, local_location.x + vertices[i + 3].x, local_location.y + vertices[i + 3].y, color, TRUE);
	}
	// フラグがTRUEだったらバリアを点滅させる
	if (damage_effect_flg)
	{
		if (damage_effect_time % 20 < 10)
		{
			if(barrier_num > 0) {
				DrawHexagonSphere();
				// バリアの描画
				DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, 115, 50, color, FALSE);
			}
		}
	}
	else
	{
		if (barrier_num > 0) {
			// バリアの描画
			DrawHexagonSphere();
			//for (int i = 0; i < barrier_num; i++) {
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, 115, 50, color, FALSE);
		}
	}

	for (int i = 0; i < barrier_num; ++i) {
		int barrier_color;
		switch (barrier_num - i) {
		case 1:
			barrier_color = GREEN;
			break;
		case 2:
			barrier_color = BLUE;
			break;
		case 3:
			barrier_color = RED;
			break;
			// それ以上のバリアは想定しないが、必要に応じて追加
		}
		DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, barrier_rad[i], 50, barrier_color, FALSE);
	}


	//DrawFormatString(1100, 80, color, "%d", barrier_num);
	//DrawFormatString(1100, 0, color, "%d", damage_flg);
	//DrawFormatString(1100, 0, color, "%d", damage_effect_time);
	//DrawFormatString(1100, 0, color, "%f", location.x);
	//DrawFormatString(1100, 30, color, "%f", location.y);
	//DrawFormatString(1100, 20, color, "%f", local_location.x);
	//DrawFormatString(1100, 60, color, "%d", boss_state);
	//DrawFormatString(1100, 60, color, "%d", cnt);
}

void Boss::Finalize()
{
}

void Boss::Move(GameMain* _g)
{
	// 現在の時間をシードとして乱数生成器を初期化する
	srand((unsigned)time(NULL));

	// 0から2までの乱数を生成し、ワープ位置のインデックスを決定する
	int warp_index = rand() % 3;

	// 生成されたインデックスに基づいて、ボスのワープ先を設定する
	location = warp_pos[warp_index];

	// ボスの状態を攻撃状態に変更する
	boss_state = BossState::ATTACK;
}
	

void Boss::Hit(Object* _object)
{


	//弱点色に触れた時の処理
	if (
		(_object->GetObjectType() == FIRE && this->color == GREEN) ||
		(_object->GetObjectType() == WATER && this->color == RED) ||
		(_object->GetObjectType() == WOOD && this->color == BLUE)
		)
	{
		//バリア減るごとにクールタイムを設ける
		if (!damage_flg) {
			damage_flg = true;
			damage_effect_time = 300;
		}
		// バリアがなくなった場合の処理
		if (barrier_num == 0) {
			boss_state = BossState::DEATH;
		}
	}
}

bool Boss::CheckCollision(Location l, Erea e)
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

float Boss::DistanceCalc(Location pos1, Location pos2)
{
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	return (float)sqrt(dx * dx + dy * dy);
}

void Boss::barrier()
{

}

void Boss::BossAtack(GameMain *_g)
{
	if ((local_location.x > 0 && local_location.x < 1280 && local_location.y > 0 && local_location.y < 720) && ++cnt >= 240) {
		oldF = f;
		f = true;
		if (cnt == 240) {
			attack = GetRand(2);
			if (local_location.x < 640.f) {
				side = true;
			}
			else {
				side = false;
			}
		}
	}

	if (f) {
		switch (attack)
		{
		case 0://火
			if (cnt % 30 == 0) {
				Erea e = { 20.f,20.f };
				_g->CreateObject(new BossAttackFire, GetCenterLocation(), e, RED);
			}
			if (cnt > 300) {
				cnt = 0;
				f = false;
				boss_state = BossState::MOVE;
			}
			break;

		case 1://水
			if (cnt % 30 == 0) {
				Erea e = { 20.f,20.f };
				Location l;
				if (side) {
					l.x = 80.f;
					l.y = attack_num * 200.f + 250.f;
				}
				else {
					l.x = 1200.f;
					l.y = attack_num * 200.f + 250.f;
				}
				//_g->CreateObject(new BossAttackWater, GetCenterLocation(), e, BLUE);
				_g->CreateObject(new BossAttackWater, l, e, BLUE);
				attack_num++;
			}
			if (cnt > 300) {
				cnt = 0;
				f = false;
				attack_num = 0;
				boss_state = BossState::MOVE;
			}
			break;

		default://木
			if (cnt % 30 == 0) {
				Erea e = { (float)(GetRand(400) + 200),40.f };
				/*Location l = _g->GetPlayerLocation();
				l.y += _g->GetPlayerErea().height + 40.f;*/

				Location l;
				l.x = GetRand(29) * 40 + 40;
				l.y = 930.f;


				_g->CreateObject(new BossAttackWood, l, e, GREEN);
				f = false;
			}
			if (cnt > 300) {
				cnt = 0;
				f = false;
				boss_state = BossState::MOVE;
			}
			break;
		}
	}
}

void Boss::DrawHexagonSphere() const
{
	// ボスの中心座標
	Location center = { local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 };
	// 六角形の中心
	Location hexa_center;

	// バリアの半径の配列を定義
	float hex_size = 10.0f; // 六角形のサイズ

	//// 六角形の間隔（六角形の内接円の半径の2倍）
	float hex_height = sqrt(3) * hex_size / 2; // 六角形の高さ

	for (int i = 0; i <= 15; ++i) { 
		for (int j = -i; j <= i; ++j) {
			for (int k = -i; k <= i; ++k) {
				if (abs(j + k) <= i) {
					// 六角形の中心座標を計算
					hexa_center.x = center.x + (1.5f * hex_size * j);
					hexa_center.y = center.y + (2.0f * hex_height * k - hex_height * j);

					// 半径内にある六角形のみを描画
					float distance = sqrt(pow(hexa_center.x - center.x, 2) + pow(hexa_center.y - center.y, 2));
					if (distance <= 110) {
						// 描画範囲を調整して内部を埋める
						DrawHexagon({ hexa_center.x, hexa_center.y }, hex_size * 0.9f, color); // 0.9fは調整可能
					}
				}
			}
		}
	}
}

void Boss::DrawHexagon(Location center, int size, int color) const
{
	float angle_space = 2 * PI / 6; // 六角形の各頂点の間の角度
	Location vertices[6];

	// 六角形の頂点座標を計算
	for (int i = 0; i < 6; ++i) {
		float angle = i * angle_space;
		vertices[i] = { center.x + size * cos(angle), center.y + size * sin(angle) };
	}

	// 六角形の描画
	for (int i = 0; i < 6; ++i) {
		DrawLineAA(vertices[i].x, vertices[i].y, vertices[(i + 1) % 6].x, vertices[(i + 1) % 6].y, color);
	}
}
