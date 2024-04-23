#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include "../Scene/GameMain.h"
#include <math.h>


Player::Player()
{
	type = PLAYER;
	can_swap = TRUE;	//プレイヤーのcan_swapは真でも偽でも大丈夫
	can_hit = TRUE;

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
	swapTimer = -1;
	oldSearchFlg = false;
}

Player::~Player()
{

}

void Player::Initialize(Location _location, Erea _erea, int _color_data)
{
	location = _location;
	erea = _erea;
	color = _color_data;
}

void Player::Update(GameMain* _g)
{
	fps = 0;

	if (stageHitFlg[1][bottom] != true && !searchFlg) { //重力
		vector.y += 1.f;
		if (vector.y > 16.f) {
			vector.y = 16.f;
		}
	}
	else {
		vector.y = 0.f;
	}

	oldSearchFlg = searchFlg;
	//Bボタンで色の交換ができるモードと切り替え
	if (PadInput::OnButton(XINPUT_BUTTON_B) && !searchFlg) {
		searchFlg = true;
	}
	else if (PadInput::OnButton(XINPUT_BUTTON_B) && searchFlg && searchedObj != nullptr && swapTimer < 0) {
		//交換エフェクトにかかる時間を受け取る
		swapTimer = _g->Swap(this, searchedObj);
	}
	else if (PadInput::OnButton(XINPUT_BUTTON_Y) && searchFlg) {
		searchFlg = false;
	}
	//Yボタンで色の交換

	//交換後エフェクト用の硬直
	if (swapTimer >= 0)
	{
		if (swapTimer == SWAP_EFFECT_STOP_TIME)
		{
			ChangePlayerColor();
		}
		//硬直が終わったら色を交換
		if (--swapTimer < 0)
		{
			searchFlg = false;
			swapTimer = -1;
		}
	}

	MoveActor();
	if (swapTimer == -1)
	{
		MoveAim();
	}

	location.x += vector.x;
	location.y += vector.y;



	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	searchedLen = 1000.f;
	searchedObj = nullptr;


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
		DrawCircle(lineLoc.x, lineLoc.y, 10, color, TRUE);
	}

	DrawFormatString(400, 20, 0xffff00, "l.x%f", location.x);
	DrawFormatString(400, 40, 0xff0000, "l.y%f", location.y);
	DrawFormatString(400, 60, 0xffff00, "e.w%f", erea.width);
	DrawFormatString(400, 80, 0xff0000, "e.h%f", erea.height);

	DrawFormatString(700, 20, 0xffff00, "v.x%f", saveVec.x);
	DrawFormatString(700, 40, 0xff0000, "v.y%f", saveVec.y);
	DrawFormatString(700, 60, 0xffff00, "v.x%f", vector.x);
	DrawFormatString(700, 80, 0xff0000, "v.y%f", vector.y);

	DrawFormatString(900, 100, 0xffff00, "v.x%d", stageHitFlg[1][0]);
	DrawFormatString(900, 120, 0xff0000, "v.y%d", stageHitFlg[1][1]);
	DrawFormatString(900, 140, 0xff0000, "v.y%d", stageHitFlg[1][2]);
	DrawFormatString(900, 160, 0xff0000, "v.y%d", stageHitFlg[1][3]);
	if (searchedObj != nullptr) {
		/*Location l = { searchedObj->GetLocalLocation().x + searchedObj->GetErea().width / 2,
			searchedObj->GetLocalLocation().y + searchedObj->GetErea().height / 2 };
		Location ll = { local_location.x + erea.width / 2,local_location.y + erea.height / 2 };*/
		//DrawFormatString(400, 00, 0xff0000, "%f", ThreePointAngle(lineLoc, l, ll));
		//DrawFormatString(600, 00, 0xff0000, "%f %f", lineLoc.x - ll.x, lineLoc.y - ll.y);
		//DrawFormatString(600, 00, 0xff0000, "%f %f", ll.x, ll.y);
		//DrawCircle(ll.x, ll.y, 15, color, FALSE);
		//DrawLine(ll.x, ll.y, l.x, l.y, color);
	}

}

void Player::Finalize()
{

}

void Player::Hit(Location _location, Erea _erea, int _type, int _color_data)
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
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _location.x - (location.x + erea.width);
			if (t != 0) {
				vector.x = 0.f;
				move[right] = t;
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
}

void Player::MoveActor()
{
	//ジャンプ
	if ((PadInput::OnButton(XINPUT_BUTTON_A)/*|| PadInput::OnPressed(XINPUT_BUTTON_A)*/) && stageHitFlg[1][bottom]) {
		vector.y = -20.f;
	}

	//左右移動
	float stick;
	if (!searchFlg) {
		//サーチ状態の時のベクトルを代入
		if (oldSearchFlg) {
			vector = saveVec;
		}

		if (PadInput::TipLeftLStick(STICKL_X) > 0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			//vector.x = stick * 5.f; //こっちの移動方法を使うかも
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
		if (!oldSearchFlg) {
			saveVec = vector;
		}
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

	if (PadInput::TipLeftLStick(STICKL_X) < 0.1f && PadInput::TipLeftLStick(STICKL_X) > -0.1f &&
		PadInput::TipLeftLStick(STICKL_Y) < 0.1f && PadInput::TipLeftLStick(STICKL_Y) > -0.1f) {
		lineLoc.x = local_location.x + (erea.width / 2);
		lineLoc.y = local_location.y + (erea.height / 2);
	}
	else {
		lineLoc.x = local_location.x + (erea.width / 2) + aimVec.x * 1280.f;
		lineLoc.y = local_location.y + (erea.height / 2) + aimVec.y * 1280.f;
	}
}

bool Player::SearchColor(Object* ob)
{
	if (ob->GetColerData() != 0) {
		Location tmpLoc = { 20,20 };
		Location tmpObLoc = { 20,20 };
		float tmpLen;
		int i = 1;
		while (tmpLoc.x > 0 && tmpLoc.x < 1280 && tmpLoc.y > 0 && tmpLoc.y < 720) {
			tmpLoc.x = local_location.x + (aimVec.x * 10 * i) + (erea.width / 2);
			tmpLoc.y = local_location.y + (aimVec.y * 10 * i) + (erea.height / 2);

			if (!(tmpLoc.x > 0 && tmpLoc.x < 1280 && tmpLoc.y > 0 && tmpLoc.y < 720)) {
				break;
			}

			tmpObLoc.x = ob->GetLocalLocation().x + (ob->GetErea().width / 2);
			tmpObLoc.y = ob->GetLocalLocation().y + (ob->GetErea().height / 2);

			if (!(tmpObLoc.x > 0 && tmpObLoc.x < 1280 && tmpObLoc.y > 0 && tmpObLoc.y < 720)) {
				break;
			}

			tmpLen = sqrtf(powf(tmpObLoc.x - tmpLoc.x, 2) + powf(tmpObLoc.y - tmpLoc.y, 2));

			if (tmpLen < searchedLen) {
				searchedLen = tmpLen;
				searchedObj = ob;
				if (searchedLen > 200.f) {
					searchedObj = nullptr;
				}
			}
			i++;
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

float Player::ThreePointAngle(Location l1, Location l2, Location referenceP)const
{
	if (l1.x == l2.x && l1.y == l2.y)return 0.f;

	Location v1, v2;
	v1.x = l1.x - referenceP.x;
	v1.y = l1.y - referenceP.y;
	v2.x = l2.x - referenceP.x;
	v2.y = l2.y - referenceP.y;
	//DrawLine(referenceP.x, referenceP.y, referenceP.x + v1.x, referenceP.y + v1.y, 0x00ff00);
	DrawLine(referenceP.x, referenceP.y, referenceP.x + v2.x, referenceP.y + v2.y, 0x00ffff);

	float len1, len2;
	//len1 = sqrtf(powf(v1.x, 2) + powf(v1.y, 2));
	//len2 = sqrtf(powf(v2.x, 2) + powf(v2.y, 2));
	len1 = pow((v1.x * v1.x) + (v1.y + v1.y), 0.5);
	len2 = pow((v2.x * v2.x) + (v2.y + v2.y), 0.5);
	/*len1 = sqrtf((v1.x * v1.x) + (v1.y * v1.y));
	len2 = sqrtf((v2.x * v2.x) + (v2.y * v2.y));*/

	/*Location uv1, uv2 = { 0,0 };
	uv1.x = v1.x / len1;
	uv1.y = v1.y / len1;
	uv2.x = v2.x / len2;
	uv2.y = v2.y / len2;*/

	/*float dot;
	dot = (uv1.x * uv2.x) + (uv1.y + uv2.y);*/

	float dot;
	dot = (v1.x * v2.x) + (v1.y + v2.y);

	/*float len1, len2;
	len1 = sqrtf(powf(v1.x, 2) + powf(v1.y, 2));
	len2 = sqrtf(powf(v2.x, 2) + powf(v2.y, 2));*/

	float cos;
	cos = dot / (len1 * len2);

	float sita;
	sita = acosf(cos) * (180.f / 3.14f);

	return sita;
}

