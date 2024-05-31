#include "Boss.h"
#include <math.h>
#include "../Utility/KeyInput.h"
#include "../Scene/GameMain.h"
#include <algorithm>

#define BOSS_SIZE 250
#define STATE_CHANGE_INTERVAL 120
#define BOSS_MAX_SPEED 4.0f
#define DIRECTION_CHANGE_SPEED 0.005f // 方向変更の速度（補間係数）

#define ANGLE_SPEED 0.01f
#define RADIUS 500.0f

float Boss::angle = 0.0f;
float Boss::direction = 1.0f;

Boss::Boss() :vector{ 0.0f }, boss_state(BossState::MOVE), barrier_num(3), damage_flg(false), state_change_time(STATE_CHANGE_INTERVAL), /*direction{ 1.0f, 0.0f },*/ target_direction{ 1.0f, 0.0f }, speed(0.0f)
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
	srand(time(0));
	//SetRandMove();
}

Boss::~Boss()
{
}

void Boss::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = { _location };//x座標 ,y座標 
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

	switch (boss_state)
	{
	case BossState::MOVE:
		// ボスの移動処理を呼び出し
		Move(_g);
		break;
	case BossState::DEATH:
		_g->DeleteObject(object_pos);
		break;
	default:
		break;
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
			// バリアが残っている場合
			if (barrier_num > 0)
			{
				// バリアを1つ減少
				barrier_num--;
				// ダメージフラグを解除
				damage_flg = false;
			}
		}
	}
	// バリアがなくなった場合の処理
	if (barrier_num == 0) {
	}

	// 状態変更のタイミングをチェック
	state_change_time--;
	// 状態変更のタイミングが0以下になったら
	if (state_change_time <= 0) {
		// ランダムな方向に移動する設定を行う
		SetRandMove();
		// 状態変更のインターバルをリセット
		state_change_time = STATE_CHANGE_INTERVAL;
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
	
	
	if ((local_location.x > 0 && local_location.x < 1280 && local_location.y > 0 && local_location.y < 720) && ++cnt >= 240 ) {
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
			}
			break;

		default://木
			if (cnt % 30 == 0) {
				Erea e = { (float)(GetRand(400) + 200),40.f};
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
			}
			break;
		}

	}


}

void Boss::Draw() const
{
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);

	// ボスの中心座標
	float boss_center_x = local_location.x + BOSS_SIZE / 2;
	float boss_center_y = local_location.y + BOSS_SIZE / 2;

	// バリアの半径の配列を定義
	float barrier_rad[] = { 120, 115, 110 };
	//本体
	DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, 35, 35, color, TRUE);

	//羽描画
	for (int i = 0; i < vertices.size(); i += 4)
	{
		//DrawQuadrangleAA(local_location.x + vertices[i].x, local_location.y + vertices[i].y, local_location.x + vertices[i + 1].x, local_location.y + vertices[i + 1].y, local_location.x + vertices[i + 2].x, local_location.y + vertices[i + 2].y, local_location.x + vertices[i + 3].x, local_location.y + vertices[i + 3].y, color, TRUE);
	}

	// 羽の頂点を反転させて左側の座標を計算
	std::vector<Location> mirrored_vertices;
	for (int i = 0; i < vertices.size(); ++i)
	{
		const Location& vertex = vertices[i];
		float mirrored_x = boss_center_x - (vertex.x - boss_center_x); // X座標を反転
		mirrored_vertices.push_back({ mirrored_x, vertex.y });
	}

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
			for (int i = 0; i < barrier_num; i++) {
				// バリアの描画
				DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, barrier_rad[i], 50, color, FALSE);
			}
		}
	}
	else
	{
		// バリアの描画
		for (int i = 0; i < barrier_num; i++) {
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2, barrier_rad[i], 50, color, FALSE);
		}
	}

	//DrawFormatString(1100, 0, color, "%d", barrier_num);
	//DrawFormatString(1100, 0, color, "%d", damage_flg);
	//DrawFormatString(1100, 0, color, "%d", damage_effect_time);
	//DrawFormatString(1100, 0, color, "%f", location.x);
	//DrawFormatString(1100, 20, color, "%f", local_location.y);

}

void Boss::Finalize()
{
}

void Boss::Move(GameMain* _g)
{
	Location player_pos = _g->GetPlayerLocation();
	float distance_to_player = DistanceCalc(location, player_pos);

	// ボスの回転角度
	/*static float angle = 0.0f;
	static float direction = 1.0f;*/

	//// プレイヤーとの距離が一定範囲内の場合にのみ移動する
	//if (distance_to_player <= 1280)
	//{
	//	  // プレイヤーとの距離が600未満の場合、プレイヤーから離れる
	//	if (distance_to_player < 450)
	//	{
	//		// プレイヤーから離れる方向を計算
	//		target_direction.x = location.x - player_pos.x;
	//		target_direction.y = location.y - player_pos.y;
	//	}
	//	// プレイヤーとの距離が600より大きい場合、プレイヤーに近づく
	//	else
	//	{
	//		// プレイヤーに近づく方向を計算
	//		target_direction.x = player_pos.x - location.x;
	//		target_direction.y = player_pos.y - location.y;
	//	}

	//	// 移動方向を正規化
	//	float lengths = sqrt(target_direction.x * target_direction.x + target_direction.y * target_direction.y);
	//	if (lengths != 0)
	//	{
	//		target_direction.x /= lengths;
	//		target_direction.y /= lengths;
	//	}

	//	// 移動方向を滑らかにする
	//	direction.x += (target_direction.x - direction.x) * DIRECTION_CHANGE_SPEED;
	//	direction.y += (target_direction.y - direction.y) * DIRECTION_CHANGE_SPEED;

	//	// 移動方向を正規化して移動速度を掛けてボスの位置を更新する
	//	float length = (float)sqrt(direction.x * direction.x + direction.y * direction.y);
	//	if (length != 0) {
	//		direction.x /= length;
	//		direction.y /= length;
	//	}

	//	location.x += direction.x * speed;
	//	location.y += direction.y * speed;
	//}

	// プレイヤーとの距離が一定範囲内の場合にのみ移動する
	if (distance_to_player <= 1280) {

		angle += ANGLE_SPEED * direction;

		float target_x = (player_pos.x - 80) + RADIUS * cos(angle);
		float target_y = (player_pos.y - 100) + RADIUS * sin(angle);

		/*float target_x = (SCREEN_WIDTH / 2) + RADIUS * cos(angle);
		float target_y = (SCREEN_HEIGHT / 2) + RADIUS * sin(angle);*/


		// 地面に到達したら回転方向を逆にする
		if (target_y > player_pos.y) {
			direction *= -1.0f; // 回転方向を逆にする
			angle += ANGLE_SPEED * direction; // すぐに逆回転を反映
			target_y = player_pos.y + (player_pos.y - target_y); // 地面の位置で反射する
		}

		location.x = target_x;
		location.y = target_y;
	}
}

void Boss::Hit(Object* _object)
{
	if (
		(_object->GetObjectType() == BLOCK && _object->GetCanHit() == TRUE) ||
		(_object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE && this->color == RED) ||
		(_object->GetObjectType() == WOOD && _object->GetCanSwap() == TRUE && this->color == GREEN) ||
		(_object->GetObjectType() == WATER && _object->GetCanSwap() == TRUE && this->color == BLUE)
		)
	{
		//if (barrier_num > 0) {
		//Location tmpl = location;
		//Erea tmpe = erea;
		//move[0] = 0;
		//move[1] = 0;
		//move[2] = 0;
		//move[3] = 0;

		////上下判定用に座標とエリアの調整
		//location.x += 10.f;
		//erea.height = 1.f;
		//erea.width = tmpe.width - 15.f;

		////プレイヤー上方向の判定
		//if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top]) {
		//	stageHitFlg[0][top] = true;
		//	stageHitFlg[1][top] = true;
		//}
		//else {
		//	stageHitFlg[0][top] = false;
		//}

		////プレイヤー下方向の判定
		//location.y += tmpe.height + 2;
		//if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom]) {
		//	stageHitFlg[0][bottom] = true;
		//	stageHitFlg[1][bottom] = true;
		//}
		//else {
		//	stageHitFlg[0][bottom] = false;
		//}

		////戻す
		//location.x = tmpl.x;
		//location.y = tmpl.y;
		//erea.height = tmpe.height;
		//erea.width = tmpe.width;

		////上方向に埋まらないようにする
		//if (stageHitFlg[0][top]) {//上方向に埋まっていたら
		//	float t = (_object->GetLocation().y + _object->GetErea().height) - location.y;
		//	if (t != 0) {
		//		vector.y = 0.f;
		//		speed = 0.0f;
		//		move[top] = t;
		//	}
		//}

		////下方向に埋まらないようにする
		//if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
		//	float t = _object->GetLocation().y - (location.y + erea.height);
		//	if (t != 0) {
		//		move[bottom] = t;
		//	}
		//}


		////左右判定用に座標とエリアの調整
		//location.y += 3.f;
		//erea.height = tmpe.height - 3.f;
		//erea.width = 1;

		////プレイヤー左方向の判定
		//if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
		//	stageHitFlg[0][left] = true;
		//	stageHitFlg[1][left] = true;
		//	int a = CheckCollision(_object->GetLocation(), _object->GetErea());
		//}
		//else {
		//	stageHitFlg[0][left] = false;
		//}


		////プレイヤー右方向の判定
		//location.x = tmpl.x + tmpe.width + 1;
		//if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
		//	stageHitFlg[0][right] = true;
		//	stageHitFlg[1][right] = true;
		//}
		//else {
		//	stageHitFlg[0][right] = false;
		//}

		////最初の値に戻す

		//location.x = tmpl.x;
		//location.y += -3.f;
		//erea.height = tmpe.height;
		//erea.width = tmpe.width;



		////左方向に埋まらないようにする
		//if (stageHitFlg[0][left]) {//左方向に埋まっていたら
		//	float t = (_object->GetLocation().x + _object->GetErea().width) - location.x;
		//	if (t != 0) {
		//		vector.x = 0.f;
		//		speed = 0.0f;
		//		move[left] = t;
		//		SetRandMove();
		//		//boss_state = BossState::RIGHT;
		//	}
		//}

		////右方向に埋まらないようにする
		//if (stageHitFlg[0][right]) {//右方向に埋まっていたら
		//	float t = _object->GetLocation().x - (location.x + erea.width);
		//	if (t != 0) {
		//		vector.x = 0.f;
		//		speed = 0.0f;
		//		move[right] = t;
		//		//boss_state = BossState::LEFT;
		//	}
		//}


		////上下左右の移動量から移動後も埋まってるか調べる
		//if (location.y < _object->GetLocation().y + _object->GetErea().height && location.y + erea.height > _object->GetLocation().y) {//左右
		//	if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
		//		move[left] = 0.f;
		//		move[right] = 0.f;
		//	}
		//}

		//location.x += move[left];
		//location.x += move[right];
		//location.y += move[top];
		//location.y += move[bottom];

		//erea.height = tmpe.height;
		//erea.width = tmpe.width;
		//}
	}


	//弱点色に触れた時の処理
	if (
		(_object->GetObjectType() == FIRE && this->color == GREEN) ||
		(_object->GetObjectType() == WATER && this->color == RED) ||
		(_object->GetObjectType() == WOOD && this->color == BLUE)
		)
	{
		//バリア減るごとにクールタイムを設ける
		if (!damage_flg && barrier_num > 0) {
			damage_flg = true;
			damage_effect_time = 300;
		}
	}

	/*if (barrier_num < 0 && boss_state != BossState::DEATH) {
		boss_state = BossState::DEATH;
	}*/
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

void Boss::SetRandMove()
{
	float angle = (float)static_cast<float>(rand()) / RAND_MAX * 2 * PI;
	/*direction.x = (float)cos(angle);
	direction.y = (float)sin(angle);*/
	//speed = 0.0f; // 新しい方向に切り替える際に速度をリセット
}
 