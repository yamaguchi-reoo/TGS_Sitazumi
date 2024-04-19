#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include <math.h>


Player::Player()
{
	type = PLAYER;
	color = WHITE;
	can_swap = TRUE;	//プレイヤーのcan_swapは真でも偽でも大丈夫

	location = { 200,200 };
	erea = { 120,60 };
	vector = { 0,0 };

	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}

	aimLoc = { 40,200 };
	aimVec = { 0,0 };
	lineLoc = { 0,0 };

	searchedLen = 1000.f;
	searchedObj = nullptr;
	searchFlg = false;
}

Player::~Player()
{

}

void Player::Update()
{
	if (stageHitFlg[1][bottom] != true) { //重力
		vector.y += 1.f;
		if (vector.y > 16.f) {
			vector.y = 16.f;
		}
	}
	else {
		vector.y = 0.f;
	}

	//Bボタンで色の交換ができるモードと切り替え
	if (PadInput::OnButton(XINPUT_BUTTON_B) && !searchFlg) {
		searchFlg = true;
	}
	else if (PadInput::OnButton(XINPUT_BUTTON_B) && searchFlg) {
		searchFlg = false;
	}
	//Yボタンで色の交換
	if (PadInput::OnButton(XINPUT_BUTTON_Y) && searchFlg) {
		ChangePlayerColor();
	}


	MoveActor();
	MoveAim();

	location.x += vector.x;
	location.y += vector.y;

	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	searchedLen = 1000.f;
	searchedObj = nullptr;

	fps++;
}

void Player::Draw()const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);
	//DrawBox(location.x, location.y, location.x + erea.width, location.y + erea.height,color, FALSE);

	DrawCircle(aimLoc.x, aimLoc.y, 10, color, TRUE);

	if (searchedObj != nullptr && searchFlg) {
		DrawCircle(searchedObj->GetLocalLocation().x + searchedObj->GetErea().width / 2, 
			searchedObj->GetLocalLocation().y + searchedObj->GetErea().height / 2, 20, 0x000000, FALSE);
		DrawFormatString(640, 80, 0xffff00, "%f", searchedObj->GetLocalLocation().x);
		DrawFormatString(640, 100, 0xff0000, "%f", searchedObj->GetLocalLocation().y);
	}
	if (searchFlg) {
		DrawLine(local_location.x + (erea.width / 2), local_location.y + (erea.height / 2), lineLoc.x, lineLoc.y, color);
	}

	DrawFormatString(400, 20, 0xffff00, "l.x%f", location.x);
	DrawFormatString(400, 40, 0xff0000, "l.y%f", location.y);
	DrawFormatString(400, 60, 0xffff00, "e.w%f", erea.width);
	DrawFormatString(400, 80, 0xff0000, "e.h%f", erea.height);

	DrawFormatString(700, 20, 0xffff00, "v.x%f", vector.x);
	DrawFormatString(700, 40, 0xff0000, "v.y%f", vector.y);

	DrawFormatString(400, 00, 0xff0000, "%d", stageHitFlg[1][bottom]);

	//DrawBox(location.x, location.y, location.x + 1, location.y + erea.height, 0xffffff, TRUE);

}

void Player::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	fps1++;
	//ブロックと当たった時の処理
	if (_type == BLOCK)
	{
		Location tmpl = location;
		Erea tmpe = erea;

		location.x += 10.f;
		erea.height = 1.f;
		erea.width = tmpe.width - 10.f;

		//プレイヤー上方向の判定
		//location.y -= tmpe.height / 2;
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//上方向に埋まらないようにする(デバックまだ)
		if (stageHitFlg[0][top]) {//上方向に埋まっていたら
			float t = (_location.y + _erea.height) - location.y;
			if (t != 0) {
				location.y += t;
				vector.y = 0.f;
			}
		}

		location.x -= 10.f;
		tmpl.y = location.y;




		erea.height = 1.f;
		erea.width = tmpe.width - 10.f;
		location.x = tmpl.x + 10.f;

		//プレイヤー下方向の判定
		location.y += tmpe.height + 1;
		if (CheckCollision(_location,_erea) && !stageHitFlg[1][bottom]) {
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


		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _location.y - (location.y + erea.height);
			if (t != 0) {
				location.y += t;
			}
		}


		//高さ、幅の変更
		location.y += 3.f;
		erea.height = tmpe.height - 3.f;
		erea.width = 1;

		//プレイヤー左方向の判定
		//location.x -= tmpe.width / 2;
		if (CheckCollision(_location, _erea) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_location, _erea);

		}
		else {
			stageHitFlg[0][left] = false;
		}
		

		//プレイヤー右方向の判定
		location.x += tmpe.width;
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
				location.x += t;
				vector.x = 0.f;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _location.x - (location.x + erea.width);
			if (t != 0) {
				location.x += t;
				vector.x = 0.f;
			}
		}

		


		erea.height = tmpe.height;
		erea.width = tmpe.width;


	}
}

void Player::MoveActor()
{
	//ジャンプ
	if (PadInput::OnButton(XINPUT_BUTTON_A) && stageHitFlg[1][bottom]) {
		vector.y = -20.f;
	}

	//左右移動
	float stick;
	if (!searchFlg) {
		if (PadInput::TipLeftLStick(STICKL_X) > 0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			if (vector.x > 5.f) {
				vector.x = 5.f;
			}
		}
		else if (PadInput::TipLeftLStick(STICKL_X) < -0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			if (vector.x < -5.f) {
				vector.x = -5.f;
			}
		}
		else {
			if (vector.x > 0.1f) {
				vector.x += -0.1f;
			}
			else if (vector.x < 0.1f) {
				vector.x += 0.1f;
			}

			if (vector.x > -0.2f && vector.x < 0.2f) {
				vector.x = 0.f;
			}
		}
	}
	else {
		vector.x = 0.f;
		vector.y = 0.f;
	}


}

void Player::MoveAim()
{
	//照準の座標
	aimLoc.x = (local_location.x + (erea.width / 2)) + PadInput::TipLeftLStick(STICKL_X) * 100.f;
	aimLoc.y = (local_location.y + (erea.height / 2)) + PadInput::TipLeftLStick(STICKL_Y) * -100.f;

	//単位ベクトル獲得
	Location tmpv;
	float len;
	tmpv.x = aimLoc.x - (local_location.x + (erea.width / 2));
	tmpv.y = aimLoc.y - (local_location.y + (erea.height / 2));
	len = sqrtf(powf(tmpv.x, 2) + powf(tmpv.y, 2));

	//単位ベクトル獲得
	aimVec.x = tmpv.x / len;
	aimVec.y = tmpv.y / len;

	lineLoc.x = local_location.x + (erea.width / 2) + aimVec.x * 1000.f;
	lineLoc.y = local_location.y + (erea.height / 2) + aimVec.y * 1000.f;
}

bool Player::SearchColor(Object* ob)
{
	if (ob->GetColerData() != 0) {
		Location tmpLoc;
		float tmpLen;
		for (int i = 0; i < 100; i++)
		{
			tmpLoc.x = local_location.x + (aimVec.x * 10 * i);
			tmpLoc.y = local_location.y + (aimVec.y * 10 * i);
			if (tmpLoc.x < 0 || tmpLoc.x > 1280 || tmpLoc.y < 0 || tmpLoc.y > 720) break;

			//tmpLen = sqrtf(powf(tmpLoc.x - ob->GetLocation().x, 2) + powf(tmpLoc.y - ob->GetLocation().y, 2));
			tmpLen = sqrtf(powf(ob->GetLocalLocation().x - tmpLoc.x, 2) + powf(ob->GetLocalLocation().y - tmpLoc.y, 2));

			if (tmpLen < searchedLen) {
				searchedLen = tmpLen;
				searchedObj = ob;
			}
		}
	}


	return false;
}

bool Player::ChangePlayerColor()
{
	ChangeColor(searchedObj);
	return false;
}

bool Player::CheckCollision(Location l, Erea e)
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
