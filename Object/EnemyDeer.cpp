#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include "EnemyDeer.h"
#include<math.h>

EnemyDeer::EnemyDeer()
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;

	//deer_state = DeerState::LEFT;
	deer_state = DeerState::GRAVITY;

	//location == 当たり判定用 local_location == 画像を動かすだけの物
	//locationの中に値を入れると 上記の変数のどちらでも値を使用できる
	// { x , y }
	//location = { 500, 850 };

	for (int i = 0; i < 4; i++)
	{
		move[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stageHitFlg[i][j] = false;
		}
	}

	deer_spawn = true;
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	//一旦引数はパス 鹿がどこにでるかの座標 
	//地面と完全に座標が一致していると地面に引っかかって動かなくなる 859
	location = _location;
	//当たり判定の大きさを変更できる
	//erea = {100, 100};
	erea = _erea;
	color = _color_data;

	object_pos = _object_pos;
}

void EnemyDeer::Update(GameMain* _g)
{
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	/*
		r: 角度(ラジアン)
		x : 元のX座標
		y : 元のY座標

		X = x * cos(r) - y * sin(r)
		Y = x * sin(r) + y * cos(r)
	*/

	EnemyDeerMove();
}

void EnemyDeer::Draw()const
{
	if (deer_state == DeerState::LEFT || deer_state == DeerState::GRAVITY || deer_state == DeerState::IDLE)
	{
		//角
		//DrawQuadrangleAA(local_location.x - 5.0f, local_location.y - 20.0f, local_location.x + 2.0f, local_location.y - 25.0f, local_location.x + 15.0f, local_location.y + 2.0f, local_location.x + 7.0f, local_location.y + 5.0f, color, TRUE);
		//DrawQuadrangleAA()

		//頭
		//DrawBoxAA(local_location.x + r_x, local_location.y + r_y, local_location.x + 30.0f + r_x, local_location.y + 20.0f + r_y, 0xffffff, TRUE);
		ResourceManager::DrawRotaBox(local_location.x + 16.0f, local_location.y + 10.0f, 30.0f, 20.0f, d_rad, color, true);

		//首
		//DrawBoxAA(local_location.x + 13.0f, local_location.y + 25.0f, local_location.x + 30.0f, local_location.y + 35.0f, 0xffffff, TRUE);
		//DrawBoxAA(local_location.x + 20.0f, local_location.y + 40.0f, local_location.x + 30.0f, local_location.y + 50.0f, 0xffffff, TRUE);
		ResourceManager::DrawRotaBox(local_location.x + 22.0f, local_location.y + 30.0f, 17.0f, 10.0f, d_rad, color, true);
		ResourceManager::DrawRotaBox(local_location.x + 26.0f, local_location.y + 45.0f, 10.0f, 10.0f, d_rad, color, true);

		//胴体
		//DrawBoxAA(local_location.x + 20.0f, local_location.y + 55.0f, local_location.x + 85.0f, local_location.y + 70.0f, 0xffffff, TRUE);
		ResourceManager::DrawRotaBox(local_location.x + 53.0f, local_location.y + 63.0f, 65.0f, 15.0f, d_rad, color, true);

		//足　左から
		//DrawQuadrangleAA(local_location.x + 20.0f, local_location.y + 75.0f, local_location.x + 30.0f, local_location.y + 75.0f, local_location.x + 25.0f, local_location.y + 100.0f, local_location.x + 15.0f, local_location.y + 100.0f, 0xffffff, TRUE);

		//DrawBoxAA(local_location.x + 35.0f, local_location.y + 75.0f, local_location.x + 45.0f, local_location.y + 100.0f, 0xffffff, TRUE);

		//DrawBoxAA(local_location.x + 60.0f, local_location.y + 75.0f, local_location.x + 70.0f, local_location.y + 100.0f, 0xffffff, TRUE);

		//DrawQuadrangleAA(local_location.x + 75.0f, local_location.y + 75.0f, local_location.x + 85.0f, local_location.y + 75.0f, local_location.x + 95.0f, local_location.y + 100.0f, local_location.x + 85.0f, local_location.y + 100.0f, 0xffffff, TRUE);

		ResourceManager::DrawRotaBox(local_location.x + 25.0f, local_location.y + 88.0f, 10.0f, 25.0f, d_rad, color, true);

		ResourceManager::DrawRotaBox(local_location.x + 41.0f, local_location.y + 88.0f, 10.0f, 25.0f, d_rad, color, true);

		ResourceManager::DrawRotaBox(local_location.x + 66.0f, local_location.y + 88.0f, 10.0f, 25.0f, d_rad, color, true);

		ResourceManager::DrawRotaBox(local_location.x + 81.0f, local_location.y + 88.0f, 10.0f, 25.0f, d_rad, color, true);
	}
	else if (deer_state == DeerState::RIGHT)
	{
		//角
		//DrawQuadrangleAA(local_location.x - 5.0f, local_location.y - 20.0f, local_location.x + 2.0f, local_location.y - 25.0f, local_location.x + 15.0f, local_location.y + 2.0f, local_location.x + 7.0f, local_location.y + 5.0f, color, TRUE);

		//頭
		DrawBoxAA(local_location.x + 70.0f, local_location.y, local_location.x + 100.0f, local_location.y + 20.0f, color, TRUE);

		//首
		DrawBoxAA(local_location.x + 70.0f, local_location.y + 25.0f, local_location.x + 87.0f, local_location.y + 35.0f, color, TRUE);
		DrawBoxAA(local_location.x + 70.0f, local_location.y + 40.0f, local_location.x + 80.0f, local_location.y + 50.0f, color, TRUE);

		//胴体
		DrawBoxAA(local_location.x + 15.0f, local_location.y + 55.0f, local_location.x + 80.0f, local_location.y + 70.0f, color, TRUE);

		//足　左から
		DrawQuadrangleAA(local_location.x + 70.0f, local_location.y + 75.0f, local_location.x + 80.0f, local_location.y + 75.0f, local_location.x + 85.0f, local_location.y + 100.0f, local_location.x + 75.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 55.0f, local_location.y + 75.0f, local_location.x + 65.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 30.0f, local_location.y + 75.0f, local_location.x + 40.0f, local_location.y + 100.0f, color, TRUE);
		DrawQuadrangleAA(local_location.x + 15.0f, local_location.y + 75.0f, local_location.x + 25.0f, local_location.y + 75.0f, local_location.x + 20.0f, local_location.y + 100.0f, local_location.x + 10.0f, local_location.y + 100.0f, color, TRUE);
	}
	
	//マウス座標
	//DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);

	//当たり判定のBox
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, GetColor(255, 255, 255), FALSE);

	/*DrawFormatString(50, 60, 0xff0000, "スクリーン(location) X：%0.1f + %0.1f Y：%0.1f + %0.1f", location.x, erea.width, location.y, erea.height);
	DrawFormatString(50, 80, 0xff0000, "ワールド(local) X     ：%0.1f Y：%0.1f", local_location.x, local_location.y);
	DrawFormatString(50, 120, 0xff0000, "相手のlocation.x : %0.1f", DrawTest1);
	DrawFormatString(50, 140, 0xff0000, "相手のlocation.y : %0.1f", DrawTest2);
	DrawFormatString(50, 160, 0xff0000, "erea.height: %0.1f", DrawTest3);
	DrawFormatString(50, 180, 0xff0000, "erea.width : %0.1f", DrawTest4);
	DrawFormatString(50, 200, 0xff0000, "type	       : %d", DrawTest5);
	DrawFormatString(50, 220, 0xff0000, "color_data : %d", DrawTest6);

	DrawFormatString(50, 20, 0xff0000, "DeerState ; %d", deer_state);*/

	//DrawFormatString(50, 40, 0xff0000, "rx %0.1f ry %0.1f", r_x, r_y);

	DrawCircleAA(local_location.x, local_location.y, 2, 32, 0x00ff00, TRUE);
	DrawCircleAA(local_location.x, local_location.y + erea.height, 2, 32, 0xff00ff, TRUE);
	//DrawCircleAA(local_location.x, local_location.y + erea.height, 2, 32, 0x00ff00, TRUE);

	DrawFormatStringF(50, 20, 0x00ff00, "StageHitFlg [0][bottom] %d  [1][bottom] %d", stageHitFlg[0][bottom], stageHitFlg[1][bottom]);
	DrawFormatStringF(50, 40, 0x00ff00, "下方向に埋まっていたら %0.1f", tn);
	DrawFormatStringF(50, 60, 0x00ff00, "move[bottom : %0.1f]", move[bottom]);
}

void EnemyDeer::EnemyDeerMove()
{

	location.y += 10;

	switch (deer_state)
	{
	case DeerState::GRAVITY:

		location.y += 10;

		break;

	case DeerState::IDLE:

		location.x;
		location.y;

		break;

	case DeerState::LEFT:

		location.x -= 1;

		break;

	case DeerState::RIGHT:

		location.x += 1;

		break;

	case DeerState::DEATH:
		break;
	}
}

void EnemyDeer::Finalize()
{

}

//何かと当たった時の処理 _
//location=当たった相手の座標 _erea=当たった相手の大きさ _type=当たった相手の種類
// (0=ステージ(BLOCK) 1=プレイヤー(PLAYER) 2=敵 3=炎(FIRE) 4=木(WOOD) 5=水(WATER))
//_color_data=当たった相手も色情報
//自分の当たり判定を取っている部分は 一番下の辺
void EnemyDeer::Hit(Object* _object)
{
	DrawTest1 = _object->GetLocation().x;
	DrawTest2 = _object->GetLocation().y;
	DrawTest3 = _object->GetErea().height;
	DrawTest4 = _object->GetErea().width;
	DrawTest5 = _object->GetObjectType();
	DrawTest6 = _object->GetColerData();

	//ブロックと当たった時の処理
	if (_object->GetObjectType() == BLOCK)
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
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤー下方向の判定
		location.y += tmpe.height + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom]) {
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
			float t = (_object->GetLocation().y + _object->GetErea().height) - location.y;
			if (t != 0) {
				move[top] = t;
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			tn = _object->GetLocation().y - (location.y + erea.height);
			if (tn != 0) {
				move[bottom] = tn;
				deer_state = DeerState::RIGHT;
			}
		}


		//左右判定用に座標とエリアの調整
		location.y += 3.f;
		erea.height = tmpe.height - 10.f;
		erea.width = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_object->GetLocation(), _object->GetErea());
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.width + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
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
			float t = (_object->GetLocation().x + _object->GetErea().width) - location.x;
			if (t != 0) {
				move[left] = t;
				//deer_state = DeerState::RIGHT;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.width);
			if (t != 0) {
				move[right] = t;
				//deer_state = DeerState::LEFT;
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		if (location.y < _object->GetLocation().y + _object->GetErea().height && location.y + erea.height > _object->GetLocation().y) {//左右
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

	//死ぬ表示
	if ((_object->GetObjectType() == FIRE && this->color == GREEN) || (_object->GetObjectType() == WATER && this->color == RED) || (_object->GetObjectType() == WOOD && this->color == BLUE))
	{
		deer_state = DeerState::DEATH;
	}
}

bool EnemyDeer::CheckCollision(Location l, Erea e)
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