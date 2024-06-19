#include "EnemyFrog.h"
#include <math.h>
#include"../Scene/GameMain.h"
#include"../Utility/ResourceManager.h"

EnemyFrog::EnemyFrog():
	timer(0),
	frog_state(FrogState::LEFT_JUMP), 
	vector{0,0}, 
	death_timer(0), 
	face_angle(0.0f),
	effect_once(false),
	jump_se(0)
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;
}

EnemyFrog::~EnemyFrog()
{

}

void EnemyFrog::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;

	object_pos = _object_pos;

	stageHitFlg[1][bottom] = true;
	jump_se = ResourceManager::SetSound("Resource/Sounds/Enemy/frog_jump.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_water.wav");
}

void EnemyFrog::Update(GameMain* _g)
{
	__super::Update(_g);

	//if (vector.x != 0 || vector.y != 0)
	//{
	//	_g->SpawnEffect(location, erea, 1, 20, color);
	//}
	if (stageHitFlg[1][bottom] != true) { //重力
		vector.y += 1.f;
		if (vector.y > 16.f) {
			vector.y = 16.f;
		}
		effect_once = false;
	}
	else {
		//一回だけエフェクトを出す
		if (effect_once == false)
		{
			_g->SpawnEffect(location, erea, LandingEffect, 15, color);
			effect_once = true;
		}
		vector.y = 0.f;
	}
	UpdataState(_g);
	//カエルの状態に応じて更新
	switch (frog_state)
	{
	case FrogState::LEFT_JUMP:
		if (frame % 60 == 0 && stageHitFlg[1][bottom] == true)
		{
			vector.x = -5.f;
			vector.y = -20.f;
			ResourceManager::StartSound(jump_se);
		}
		//空中で僅かに加速(ブロックに引っかかる対策)
		if (vector.x == 0 && stageHitFlg[1][bottom] == false)
		{
			vector.x = -0.5f;
		}
		break;
	case FrogState::RIGHT_JUMP:
		if (frame % 60 == 0 && stageHitFlg[1][bottom] == true)
		{
			vector.x = 5.f;
			vector.y = -20.f;
			ResourceManager::StartSound(jump_se);
		}
		//空中で僅かに加速(ブロックに引っかかる対策)
		if (vector.x == 0 && stageHitFlg[1][bottom] == false)
		{
			vector.x = 0.5f;
		}
		break;
	case FrogState::DEATH:
		_g->SpawnEffect(location, erea, DeathEffect, 15, color);
		if (++death_timer > 60)
		{
			if (this != nullptr) {
				_g->DeleteObject(object_pos, this);
			}
		}
		break;
	default:
		break;
	}

	Move(_g);

	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}
}

void EnemyFrog::Draw()const
{
	//残像描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (death_timer * 4)) - 500);
	FrogDraw({ local_location.x - (vector.x * 2),local_location.y - (vector.y * 2) });
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (death_timer * 4)) - 100);
	FrogDraw({ local_location.x - (vector.x * 3),local_location.y - (vector.y * 3) });
	//カエル描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (death_timer * 4));
	FrogDraw(local_location);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}

void EnemyFrog::Finalize()
{

}

void EnemyFrog::Hit(Object* _object)
{
	__super::Hit(_object);

	//ブロックと当たった時の処理
	if (_object->GetObjectType() == BLOCK && _object->GetCanHit() == TRUE)
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
				vector.y = 0.f;
				move[top] = t;
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _object->GetLocation().y - (location.y + erea.height);
			if (t != 0) {
				move[bottom] = t;
			}
		}


		//左右判定用に座標とエリアの調整
		location.y += 3.f;
		erea.height = tmpe.height - 3.f;
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
				vector.x = 0.f;
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.width);
			if (t != 0) {
				vector.x = 0.f;
				move[right] = t;
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

	//弱点色に触れた時の処理
	if (
		(_object->GetObjectType() == FIRE && this->color == GREEN)||
		(_object->GetObjectType() == WATER && this->color == RED)||
		(_object->GetObjectType() == WOOD && this->color == BLUE)
	   )
	{
		//死亡状態へ
		if (frog_state != FrogState::DEATH)
		{
			frog_state = FrogState::DEATH;
			ResourceManager::StartSound(damage_se[_object->GetObjectType() - 3]);
			can_swap = FALSE;
		}
	}

	//ダメージゾーンを上書きする
	if ((this->color == GREEN && _object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE) ||
		(this->color == BLUE && _object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE) ||
		(this->color == RED && _object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE))
	{
		_object->SetColorData(color);
	}
}

void EnemyFrog::FrogDraw(Location location)const
{
#ifdef _DEBUG
	//当たり判定
	DrawBoxAA(location.x, location.y, location.x + erea.width, location.y + erea.height, draw_color, FALSE);
	//移動方向
	//DrawFormatStringF(local_location.x, local_location.y, 0x00ff00, "%f %f", vector.x, vector.y);
#endif

	//胴体
	ResourceManager::DrawRotaBox(location.x + (erea.width / 2), location.y + (erea.height / 2), erea.width, erea.height / 2, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
	ResourceManager::DrawRotaBox(location.x + (erea.width / 2), location.y + (erea.height / 2), erea.width, erea.height / 2, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);

	//右着地
	if (face_angle == 0 && vector.x == 0 && vector.y == 0)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//前足
		//ResourceManager::DrawRotaBox(location.x + 20, location.y + 20, 20, 30, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + erea.height - 20, 10, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0xffffff, TRUE);

	}
	//左着地
	else if (face_angle == 180 && vector.x == 0 && vector.y == 0)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + erea.height - 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + erea.height - 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + erea.height - 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width - 10, location.y + erea.height - 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//前足
		//ResourceManager::DrawRotaBox(location.x + 20, location.y + erea.height - 20, 20, 30, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + 20, 10, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0xffffff, TRUE);

	}
	//左向き
	else if (face_angle > 90 && face_angle < 270)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.width, location.y + erea.height - 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width, location.y + erea.height - 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.width + 30, location.y + erea.height - 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width + 30, location.y + erea.height - 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//前足
		//ResourceManager::DrawRotaBox(location.x, location.y + erea.height - 20, 20, 30, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + 20, 10, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0xffffff, TRUE);

	}
	//右向き
	else
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.width, location.y + 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width, location.y + 20, 30, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.width + 30, location.y + 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.width + 30, location.y + 10, 40, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0x000000, FALSE);
		//前足
		//ResourceManager::DrawRotaBox(location.x, location.y + 20, 20, 30,location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, draw_color, TRUE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + erea.height - 20, 10, 10, location.x + (erea.width / 2), location.y + (erea.height / 2), face_angle, 0xffffff, TRUE);

	}
}

void EnemyFrog::Move(GameMain* _g)
{
	//顔の向き更新
	if (vector.x == 0 && vector.y == 0)
	{
		if (frog_state == FrogState::LEFT_JUMP)
		{
			face_angle = 180;
		}
		if (frog_state == FrogState::RIGHT_JUMP)
		{
			face_angle = 0;
		}
	}
	else
	{
		if (frog_state == FrogState::LEFT_JUMP)
		{
			face_angle = atanf(vector.y / vector.x) * 60 +180;
		}
		if (frog_state == FrogState::RIGHT_JUMP)
		{
			face_angle = atanf(vector.y / vector.x) * 60;
		}
	}
	//移動
	location.x += vector.x;
	location.y += vector.y;
}

void EnemyFrog::UpdataState(GameMain* _g)
{
	if (frog_state != FrogState::DEATH && stageHitFlg[1][bottom] == true)
	{
		//カエルとプレイヤーの座標を比較して、カエルがプレイヤーに向かって行くように
		if (location.x > _g->GetPlayerLocation().x)
		{
			frog_state = FrogState::LEFT_JUMP;
		}
		else
		{
			frog_state = FrogState::RIGHT_JUMP;
		}
	}

}

bool EnemyFrog::CheckCollision(Location l, Erea e)
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
