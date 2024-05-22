#include "Boss.h"
#include<math.h>
#include"../Utility/KeyInput.h"
#include"../Scene/GameMain.h"

#define BOSS_SIZE 315

Boss::Boss() :vector{ 0.0f }, hit(false), boss_state(BossState::IDLE), barrier_num(3), damage_flg(false)
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
}

Boss::~Boss()
{
}

void Boss::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = {_location};//x座標 ,y座標 
	erea = { _erea };	   //高さ、幅	
	color = _color_data;

	object_pos = _object_pos;
	
}

void Boss::Update(GameMain* _g)
{
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	vector = { 1.f };
	//プレイヤーとボスの距離を計算
	DistanceCalc(_g);

	barrier();
	if (damage_flg) {
		damage_effect_time--;
		damage_effect_flg = !damage_effect_flg;
		if (damage_effect_time <= 0) {
			damage_flg = false;
		}
	}
}

void Boss::Draw() const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);

	//本体
	DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, 50, 50, color, TRUE);

	if (damage_effect_flg) {
		if (damage_effect_time % 20 < 10) {
			// バリアの描画
			int barrier_rad[] = { 175, 170, 165 };
			for (int i = 0; i < barrier_num; i++) {
				DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, barrier_rad[i], 50, color, FALSE);
			}

			//ボスの羽画用配列
			const std::vector<Location>vertices = {
				//右翼
				{local_location.x + 225, local_location.y + 140}, {local_location.x + 250, local_location.y + 120 },{local_location.x + 255, local_location.y + 140 },{local_location.x + 230, local_location.y + 160 },
				{local_location.x + 275, local_location.y + 140}, {local_location.x + 300, local_location.y + 120 },{local_location.x + 305, local_location.y + 140 },{local_location.x + 280, local_location.y + 160 },
			};
			//羽描画
			for (int i = 0; i < vertices.size(); i += 4)
			{
				//DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, color, TRUE);
			}
		}
	}
	else
	{
		// バリアの描画
		int barrier_rad[] = { 175, 170, 165 };
		for (int i = 0; i < barrier_num; i++) {
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, barrier_rad[i], 50, color, FALSE);
		}

		//ボスの羽画用配列
		const std::vector<Location>vertices = {
			//右翼
			{local_location.x + 225, local_location.y + 140}, {local_location.x + 250, local_location.y + 120 },{local_location.x + 255, local_location.y + 140 },{local_location.x + 230, local_location.y + 160 },
			{local_location.x + 275, local_location.y + 140}, {local_location.x + 300, local_location.y + 120 },{local_location.x + 305, local_location.y + 140 },{local_location.x + 280, local_location.y + 160 },
		};
		//羽描画
		for (int i = 0; i < vertices.size(); i += 4)
		{
			//DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, color, TRUE);
		}
	}
	
	//DrawFormatString(1100, 0, color, "%d", barrier_num);
	DrawFormatString(1100, 0, color, "%d", damage_effect_time);
}

void Boss::Finalize()
{
}

void Boss::Move(float dx, float dy)
{
	switch (boss_state)
	{
	case BossState::IDLE:
		//location.y += (float)sin(PI * 2.f / 40.f) * 5.f;
		// 移動する
		location.x += dx * (vector.x + 1.5f);
		location.y += dy * (vector.y + 1.5f);
		break;
	case BossState::LEFT:
		//location.x -= vector.x;
		break;
	case BossState::RIGHT:
		//location.x += vector.x;
		break;
	case BossState::DEATH:
		break;
	default:
		break;
	}
}

void Boss::Hit(Object* _object)
{
	if (_object->GetObjectType() == BLOCK) {
	//	Location tmpl = location;
	//	Erea tmpe = erea;
	//	move[0] = 0;
	//	move[1] = 0;
	//	move[2] = 0;
	//	move[3] = 0;

	//	//上下判定用に座標とエリアの調整
	//	location.x += 10.f;
	//	erea.height = 1.f;
	//	erea.width = tmpe.width - 15.f;

	//	//プレイヤー上方向の判定
	//	if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top]) {
	//		stageHitFlg[0][top] = true;
	//		stageHitFlg[1][top] = true;
	//	}
	//	else {
	//		stageHitFlg[0][top] = false;
	//	}

	//	//プレイヤー下方向の判定
	//	location.y += tmpe.height + 2;
	//	if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom]) {
	//		stageHitFlg[0][bottom] = true;
	//		stageHitFlg[1][bottom] = true;
	//	}
	//	else {
	//		stageHitFlg[0][bottom] = false;
	//	}

	//	//戻す
	//	location.x = tmpl.x;
	//	location.y = tmpl.y;
	//	erea.height = tmpe.height;
	//	erea.width = tmpe.width;

	//	//上方向に埋まらないようにする
	//	if (stageHitFlg[0][top]) {//上方向に埋まっていたら
	//		float t = (_object->GetLocation().y + _object->GetErea().height) - location.y;
	//		if (t != 0) {
	//			vector.y = 0.f;
	//			move[top] = t;
	//		}
	//	}

	//	//下方向に埋まらないようにする
	//	if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
	//		float t = _object->GetLocation().y - (location.y + erea.height);
	//		if (t != 0) {
	//			move[bottom] = t;
	//		}
	//	}


	//	//左右判定用に座標とエリアの調整
	//	location.y += 3.f;
	//	erea.height = tmpe.height - 3.f;
	//	erea.width = 1;

	//	//プレイヤー左方向の判定
	//	if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
	//		stageHitFlg[0][left] = true;
	//		stageHitFlg[1][left] = true;
	//		int a = CheckCollision(_object->GetLocation(), _object->GetErea());
	//	}
	//	else {
	//		stageHitFlg[0][left] = false;
	//	}


	//	//プレイヤー右方向の判定
	//	location.x = tmpl.x + tmpe.width + 1;
	//	if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
	//		stageHitFlg[0][right] = true;
	//		stageHitFlg[1][right] = true;
	//	}
	//	else {
	//		stageHitFlg[0][right] = false;
	//	}

	//	//最初の値に戻す

	//	location.x = tmpl.x;
	//	location.y += -3.f;
	//	erea.height = tmpe.height;
	//	erea.width = tmpe.width;



	//	//左方向に埋まらないようにする
	//	if (stageHitFlg[0][left]) {//左方向に埋まっていたら
	//		float t = (_object->GetLocation().x + _object->GetErea().width) - location.x;
	//		if (t != 0) {
	//			vector.x = 0.f;
	//			move[left] = t;

	//			//boss_state = BossState::RIGHT;
	//		}
	//	}

	//	//右方向に埋まらないようにする
	//	if (stageHitFlg[0][right]) {//右方向に埋まっていたら
	//		float t = _object->GetLocation().x - (location.x + erea.width);
	//		if (t != 0) {
	//			vector.x = 0.f;
	//			move[right] = t;
	//			//boss_state = BossState::LEFT;
	//		}
	//	}


	//	//上下左右の移動量から移動後も埋まってるか調べる
	//	if (location.y < _object->GetLocation().y + _object->GetErea().height && location.y + erea.height > _object->GetLocation().y) {//左右
	//		if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
	//			move[left] = 0.f;
	//			move[right] = 0.f;
	//		}
	//	}

	//	location.x += move[left];
	//	location.x += move[right];
	//	location.y += move[top];
	//	location.y += move[bottom];

	//	erea.height = tmpe.height;
	//	erea.width = tmpe.width;

	}


	//弱点色に触れた時の処理
	if (
		(_object->GetObjectType() == FIRE && this->color == GREEN) ||
		(_object->GetObjectType() == WATER && this->color == RED) ||
		(_object->GetObjectType() == WOOD && this->color == BLUE)
		)
	{
		//バリア減るごとにクールタイムを設ける
		if (!damage_flg) {
			if (barrier_num > 0)
			{
				barrier_num--;
				damage_flg = true;
				damage_effect_time = 180;
			}
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

void Boss::DistanceCalc(GameMain* _g)
{
	// プレイヤーとの距離を計算
	Location player_pos = _g->GetPlayerLocation();
	Erea player_erea = _g->GetPlayerErea();

	// プレイヤーの中心座標を計算
	float player_center_x = player_pos.x + player_erea.width / 2;
	float player_center_y = player_pos.y + player_erea.height / 2;

	// 自分の中心座標を計算
	float enemy_center_x = location.x + erea.width / 2;
	float enemy_center_y = location.y + erea.height / 2;

	// プレイヤーの中心座標との距離を計算
	float dx = player_center_x - enemy_center_x;
	float dy = player_center_y - enemy_center_y;
	float length = (float)sqrt(dx * dx + dy * dy);


	dx /= (float)length;
	dy /= (float)length;

	//移動
	Move(dx, dy);
}

void Boss::barrier()
{
	switch (barrier_num)
	{
	case 3:
		color = RED;
		break;
	case 2:
		color = BLUE;
		break;
	case 1:
		color = GREEN;
		break;
	default:
		break;
	}
}
