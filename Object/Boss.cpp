#include "Boss.h"
#include <math.h>
#include "../Utility/KeyInput.h"
#include "../Scene/GameMain.h"
#include <algorithm>
#include <iostream>
#include <fstream>

#define BOSS_SIZE 250
#define STATE_CHANGE_INTERVAL 340
#define BOSS_MAX_SPEED 4.0f

#define ANGLE_SPEED 0.01f
#define RADIUS 300.0f

#define BLACK 0x000000


Boss::Boss() :vector{ 0.0f }, boss_state(BossState::ATTACK), barrier_num(3), damage_flg(false), state_change_time(0), speed(0.0f),wing_fps(0),damage_se(0)
{
	type = BOSS;
	can_swap = TRUE;
	can_hit = TRUE;
	for (int i = 0; i < 4; i++) {
		move[i] = 0;
		stageHitFlg[1][i] = false;
	}
	for (int i = 0; i < barrier_num; i++)
	{
		barrier_rad[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		part_color[i] = 0;
	}

	// wing の初期化
	wing.fill({ 0.0f,0.0f });
	wing_mirror.fill({ 0.0f,0.0f });

	cunt = 1;
	c = 1;
	num = 0;

	boss_anim = 0.0f;
	shake_anim = 0;

	damage_anim_flg = false;
	damage_anim_time = 0;

	wing_color = 0;
}

Boss::~Boss()
{
}

void Boss::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = { SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 400};//x座標 ,y座標 
	//location = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 300 };//x座標 ,y座標 
	erea = { _erea };	   //高さ、幅	
	color = _color_data;

	object_pos = _object_pos;

	warp_pos = {
		 {(SCREEN_WIDTH / 2 + 50.0f) , 125.0f},			 //中央
		 {SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 400.0f},//右
		 {370.0f , SCREEN_HEIGHT - 400.0f}				 //左
	};

	barrier_rad[0] = 60;
	barrier_rad[1] = 55;
	barrier_rad[2] = 50;

	damage_effect_time = 180;
	damage_anim_time = 60;

	wing_color = 0x000000;

	for (int i = 0; i < 3; i++)
	{
		part_color[i] = BLACK;
	}

	LoadPosition();  // 初期化時に座標を読み込む

	damage_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/boss_damage.wav");

}

void Boss::Update(GameMain* _g)
{
	++wing_fps;
	// ステージとの当たり判定フラグを初期化
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	speed = BOSS_MAX_SPEED;
	vector = { 1.0f ,1.0f };

	// 更新時に座標を保存
	SavePosition(); 
	
	//羽の反転処理
	InvertedWingPositions();

	//ボスの羽を可変可能に
	//UpdateWingPositions();

	boss_anim = (float)sin(PI * 2.f / 60.f * wing_fps) * 5.f;
	Location player_pos = _g->GetPlayerLocation();

	if (player_pos.x > 140) {

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
			_g->DeleteObject(object_pos, this);
			_g->UpdateState(GameMainState::GameClear);
			break;
		default:
			break;
		}
	}

	// ダメージを受けている場合の処理
	if (damage_flg) {
		// 時間を減少
		damage_effect_time--;
		// フラグを設定
		damage_effect_flg = true;
		// 時間が0以下になったら
		if (damage_effect_time <= 0)
		{
			damage_effect_time = 180;
			// バリアを1つ減少
			barrier_num--;
			// ダメージフラグを解除
			damage_flg = false;

			//shake_anim = 0;
		}
	}
	// ダメージを受けている場合のアニメーション処理
	if (damage_anim_flg) {
		shake_anim = GetRand(10) - 5;

		damage_anim_time--;

		//color = GetRand(256);
		int r = GetRand(2);
		switch (r)
		{
		case 0:
			color = RED;
			wing_color = RED;
			//part_color[0] = RED;
			part_color[1] = BLUE;
			part_color[2] = GREEN;
			break;
		case 1:
			color = BLUE;
			wing_color = BLUE;
			//part_color[0] = BLUE;
			part_color[1] = GREEN;
			part_color[2] = RED;
			break;
		case 2:
			color = GREEN;
			wing_color = GREEN;
			//part_color[0] = GREEN;
			part_color[1] = RED;
			part_color[2] = BLUE;
			break;
		default:
			break;
		}
		if (damage_anim_time <= 0)
		{
			for (int i = 0; i < 3; i++) {
				part_color[i] = BLACK;
			}
			wing_color = 0x000000;
			shake_anim = 0;
			damage_anim_flg = false;
			damage_anim_time = 60;

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

#ifdef _DEBUG
	if (KeyInput::OnKey(KEY_INPUT_X))
	{
		/*cunt += 1;
		if (cunt % 4 == 0) {
			c += 1;
		}*/
		damage_flg = true;
		damage_anim_flg = true;
	}
	if (KeyInput::OnKey(KEY_INPUT_Z))
	{
		cunt -= 1;
		if (cunt < 0) {
			cunt = 0;
		}
		if (cunt % 4 == 0) {
			c -= 1;
			if (c < 0) {
				c = 0;
			}
		}
	}
#endif
}

void Boss::Draw() const
{
	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);

	////本体
	//DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 35, 0x000000, TRUE);
	//DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 34, 0xFFFFFF, FALSE, 3.0f);
	//DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 36, 36, color, FALSE, 2.0f);

	//羽描画
	//DrawWings();

	// フラグがTRUEだったらバリアを点滅させる
	if (damage_effect_flg)
	{
		if (damage_effect_time % 20 < 10)
		{
			DrawWings();
			//本体
			DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 35, part_color[1], TRUE);
			DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 34, 0xFFFFFF, FALSE, 3.0f);
			DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 36, 36, color, FALSE, 2.0f);
			if(barrier_num > 0) {
				DrawHexagonSphere();
				// バリアの描画
				DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 115, 50, color, FALSE, 3.0f);
				DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 112.5, 50, color, FALSE, 2.0f);
				DrawCircleAA(local_location.x + BOSS_SIZE / 2 + shake_anim, local_location.y + BOSS_SIZE / 2 + boss_anim, 109, 50, color, FALSE);
			}
		}
	}
	else
	{
		DrawWings();
		//本体
		DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 35, 0x000000, TRUE);
		DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 35, 34, 0xFFFFFF, FALSE, 3.0f);
		DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 36, 36, color, FALSE, 2.0f);

		if (barrier_num > 0) {
			// バリアの描画
			DrawHexagonSphere();
			//for (int i = 0; i < barrier_num; i++) {
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 115, 50, color, FALSE , 3.0f);
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 112.5, 50, color, FALSE, 2.0f);
			DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 109, 50, color, FALSE);
		}
	}

	for (int i = 0; i < barrier_num; ++i) {
		int barrier_color = RED;
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
		DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, barrier_rad[i], 50, barrier_color, FALSE);
	}
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
			damage_effect_time = 180;
			damage_anim_flg = true;
			damage_anim_time = 60;
			ResourceManager::StartSound(damage_se);
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
		can_swap = false;
		switch (attack)
		{
		case 0://火
			color = RED;
			if (++t > 60) {
				can_swap = true;
			}
			if (cnt % 30 == 0) {
				Erea e = { 20.f,20.f };
				_g->CreateObject(new BossAttackFire, GetCenterLocation(), e, RED);
			}
			if (cnt > 300) {
				cnt = 0;
				f = false;
				boss_state = BossState::MOVE;
				t = 0;
			}
			break;

		case 1://水
			color = BLUE;
			if (++t > 60) {
				can_swap = true;
			}
			if (cnt % 30 == 0) {
				Erea e = { 40.f,40.f };
				Location l;
				if (side) {
					l.x = 300.f;
					l.y = attack_num * 150.f + 150.f;
				}
				else {
					l.x = 1200.f;
					l.y = attack_num * 150.f + 150.f;
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
				t = 0;
			}
			break;

		default://木
			color = GREEN;
			if (++t > 20) {
				can_swap = true;
			}
			if (cnt % 30 == 0) {
				Erea e = { (float)(GetRand(400) + 400),40.f };
				/*Location l = _g->GetPlayerLocation();
				l.y += _g->GetPlayerErea().height + 40.f;*/

				Location l;
				
				float x;
				int i = 0;
				bool flg;
				do
				{
					x = (float)(GetRand(29)) * 40 + 200;
					flg = false;
					for (int j = 0; j < 3; j++)
					{
						if (x == attackWood[j])
						{
							flg = true;
						}
					}
				} while (flg);

				l.x = x;
				l.y = 930.f;

				attackWood[woodNum++] = l.x;


				_g->CreateObject(new BossAttackWood, l, e, GREEN);
				f = false;
			}
			if (cnt > 300) {
				cnt = 0;
				f = false;
				boss_state = BossState::MOVE;
				woodNum = 0;
				t = 0;
				for (int i = 0; i < 3; i++)
				{
					attackWood[i] = 0.f;
				}
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
	float hex_size = 15.0f; // 六角形のサイズ

	// 六角形の間隔（六角形の内接円の半径の2倍）
	float hex_height = (float)sqrt(3) * (float)hex_size / 2; // 六角形の高さ

	//六角形の層の数を管理
	for (int i = 0; i <= 9; ++i) { 
		//各層内の六角形の水平方向の位置を管理
		for (int j = -i; j <= i; ++j) {
			//各層内の六角形の垂直方向の位置を管理
			for (int k = -i; k <= i; ++k) {
				// 六角形の中心座標を計算
				//中心の x 座標に対して、水平に 1.5f * hex_size * j だけ移動
				hexa_center.x = center.x + (1.5f * hex_size * j);
				//中心の y 座標に対して、垂直方向に 2.0f * hex_height * k - hex_height * j だけ移動
				hexa_center.y = center.y + (2.0f * hex_height * k - hex_height * j);

				// 半径内にある六角形のみを描画
				float distance = (float)sqrt(pow(hexa_center.x - center.x, 2) + pow(hexa_center.y - center.y, 2));
				if (distance <= 110) {
					// 描画範囲を調整して内部を埋める
					DrawHexagon({ hexa_center.x, hexa_center.y }, hex_size * 0.9f, color); // 0.9fは調整可能
				}
			}
		}
	}
}

void Boss::DrawHexagon(Location center, float size, int color) const
{
	float angle_space = (float)(2.0f * PI / 6.0f); // 六角形の各頂点の間の角度
	Location vertices[6];

	// 六角形の頂点座標を計算
	//6つの頂点を中心から等距離に配置
	for (int i = 0; i < 6; ++i) {
		float angle = i * angle_space;
		//size は六角形の辺の長さで、cos(angle) は角度に対する水平方向、sin(angle) は角度に対する垂直方向
		vertices[i] = { center.x + size * cos(angle), center.y + size * sin(angle) };
	}

	// 六角形の描画
	for (int i = 0; i < 6; ++i) {
		//(i + 1) % 6 は、最後の頂点から最初の頂点に戻るための処理
		DrawLineAA(vertices[i].x, vertices[i].y + boss_anim, vertices[(i + 1) % 6].x, vertices[(i + 1) % 6].y + boss_anim, color);
	}
}

void Boss::DrawWings() const
{
	Location center = { local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 };

	// アニメーションに基づいて描画位置を計算
	float angle = 0.0f;

	// 羽の描画（）
	for (int i = 0; i < wing.size(); i += 4) {
		float delta_y = 0.f;
		delta_y = (float)sin(PI * 2.f / 60.f * wing_fps + i) * 5.f; // 2番目の羽は中程度に動く
		angle = (float)(sin(PI * 2.f / 60.f * wing_fps) * 5.f); // アニメーションに適した角度を計算

		//３番目の羽だけ違う挙動に
		if (i > 7 &&i < 12 ) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)wing_fps + i) * 15.f;
		}
		else if (i > 19 && i < 24) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)wing_fps + i) * 15.f;
		}
		else if (i > 31 && i < 36) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)wing_fps + i) * 15.f;
		}

		//右羽
		DrawQuadrangleAA(local_location.x + wing[i].x + angle, local_location.y + wing[i].y + angle + delta_y,
			local_location.x + wing[i + 1].x + 20 + angle, local_location.y + wing[i + 1].y + 10 + angle + delta_y,
			local_location.x + wing[i + 2].x + 10 + angle, local_location.y + wing[i + 2].y + 20 + angle + delta_y,
			local_location.x + wing[i + 3].x + angle, local_location.y + wing[i + 3].y + 30 + angle + delta_y, part_color[1], TRUE);
		//左羽
		DrawQuadrangleAA(local_location.x + wing_mirror[i].x + 250 - angle, local_location.y + wing_mirror[i].y + angle + delta_y,
			local_location.x + wing_mirror[i + 1].x - 20 + 250 - angle, local_location.y + wing_mirror[i + 1].y + 10 + angle + delta_y,
			local_location.x + wing_mirror[i + 2].x - 10 + 250 - angle, local_location.y + wing_mirror[i + 2].y + 20 + angle + delta_y,
			local_location.x + wing_mirror[i + 3].x + 250 - angle, local_location.y + wing_mirror[i + 3].y + 30 + angle + delta_y, part_color[1], TRUE);

		//右羽
		DrawQuadrangleAA(local_location.x + wing[i].x + angle, local_location.y + wing[i].y + angle + delta_y,
			local_location.x + wing[i + 1].x + 20 + angle, local_location.y + wing[i + 1].y + 10 + angle + delta_y,
			local_location.x + wing[i + 2].x + 10 + angle, local_location.y + wing[i + 2].y + 20 + angle + delta_y,
			local_location.x + wing[i + 3].x + angle, local_location.y + wing[i + 3].y + 30 + angle + delta_y, 0xFFFFFF, FALSE, 3.0f);
		//左羽
		DrawQuadrangleAA(local_location.x + wing_mirror[i].x + 250 - angle, local_location.y + wing_mirror[i].y + angle + delta_y,
			local_location.x + wing_mirror[i + 1].x - 20 + 250 - angle, local_location.y + wing_mirror[i + 1].y + 10 + angle + delta_y,
			local_location.x + wing_mirror[i + 2].x - 10 + 250 - angle, local_location.y + wing_mirror[i + 2].y + 20 + angle + delta_y,
			local_location.x + wing_mirror[i + 3].x + 250 - angle, local_location.y + wing_mirror[i + 3].y + 30 + angle + delta_y, 0xFFFFFF, FALSE, 3.0f);
	}
}

void Boss::UpdateWingPositions()
{
#ifdef _DEBUG
	if (KeyInput::OnKey(KEY_INPUT_W) || KeyInput::OnPresed(KEY_INPUT_W)) {
		wing[cunt - 1].y -= 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_A) || KeyInput::OnPresed(KEY_INPUT_A)) {
		wing[cunt - 1].x -= 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_S) || KeyInput::OnPresed(KEY_INPUT_S)) {
		wing[cunt - 1].y += 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_D) || KeyInput::OnPresed(KEY_INPUT_D)) {
		wing[cunt - 1].x += 1.0f;
	}

	if (KeyInput::OnKey(KEY_INPUT_UP)) {
		wing[cunt - 1].y -= 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_LEFT)) {
		wing[cunt - 1].x -= 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_DOWN)) {
		wing[cunt - 1].y += 1.0f;
	}
	if (KeyInput::OnKey(KEY_INPUT_RIGHT)) {
		wing[cunt - 1].x += 1.0f;
	}

	// マウスの位置を取得
	float mousePos_X = (float)KeyInput::GetMouseCursor().x;
	float mousePos_Y = (float)KeyInput::GetMouseCursor().y;

	if (KeyInput::OnKey(KEY_INPUT_T)) {
		num += 4;
	}
	if (KeyInput::OnKey(KEY_INPUT_Y)) {
		num -= 4;
	}
	if (num < 0) {
		num = 0;
	}


	if(KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT)){
		//羽全体の移動量を計算
		Location wingMove = { mousePos_X - wing[num].x, mousePos_Y - wing[num].y };
		// 羽全体の座標を更新
		for (int i = num; i < num + 4; ++i) {
			wing[i].x += wingMove.x - 500;
			wing[i].y += wingMove.y - 370;
		}
	}
#endif
}

void Boss::InvertedWingPositions()
{
	// wing_mirror を更新する
	Location center = { local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 };

	for (size_t i = 0; i < wing.size(); ++i)
	{
		// wing[i] の X 座標を反転して wing_mirror[i] の X 座標にセット
		wing_mirror[i].x = -wing[i].x; // X座標を反転

		// wing[i] の Y 座標をそのまま wing_mirror[i] の Y 座標にセット
		wing_mirror[i].y = wing[i].y;
	}
}

void Boss::SavePosition()
{
	std::ofstream outfile("Resource/Dat/BossLocation.txt");
	if (outfile.is_open()) {
		for (int i = 0; i < wing.size(); ++i)
		{
			outfile << wing[i].x << " " << wing[i].y << "\n";
		}
		outfile.close();
	}
}

void Boss::LoadPosition()
{
	std::ifstream infile("Resource/Dat/BossLocation.txt");
	if (infile.is_open()) {
		for (int i = 0; i < wing.size(); ++i)
		{
			infile >> wing[i].x >> wing[i].y;
		}
		infile.close();
	}
}

