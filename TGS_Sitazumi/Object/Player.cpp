#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/ResourceManager.h"


Player::Player()
{
	location = { 40,200 };
	erea = { 120,60 };
	vector = { 0,0 };
}

Player::~Player() 
{
	
}

void Player::Update(GameMain* main)
{
	if (stageHitFlg[0][bottom] != true) {
		vector.y += 1;
		if (vector.y > 5) {
			vector.y = 5;
		}
	}
	else {
		vector.y = 0;
	}

	Move();

	location.x += vector.x;
	location.y += vector.y;
}

void Player::Draw()const
{
	DrawBox(location.x - (erea.width / 2), location.y - (erea.height / 2),
		location.x + (erea.width / 2), location.y + (erea.height / 2),
		0xff0000, TRUE);

	DrawFormatString(0, 0, 0xff0000, "%d", stageHitFlg[bottom]);
}

void Player::Move()
{
	if (PadInput::OnButton(XINPUT_BUTTON_A) && stageHitFlg[bottom]) {
		vector.y = -20;
	}
}

bool Player::CheckCollision(Stage* stage)
{
	Location tmpl = location;
	Erea tmpe = erea;

	if (stage->GetStageType() != 0) {

		erea.height = 1;

		//プレイヤーの上側
		location.y -= tmpe.height / 2;
		if (stage->HitBox(this) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤーの下側
		location.y += tmpe.height;
		if (stage->HitBox(this)) {
			stageHitFlg[0][bottom] = true;
		}
		else {
			stageHitFlg[0][bottom] = false;
		}

		erea.height = tmpe.height;
		erea.width = 1;

		//プレイヤーの左側
		location.x -= tmpe.width / 2;
		if (stage->HitBox(this)) {
			stageHitFlg[0][left] = true;
		}
		else {
			stageHitFlg[0][left] = false;
		}

		//プレイヤーの右側
		location.x += tmpe.width;
		if (stage->HitBox(this)) {
			stageHitFlg[0][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}

		location.x = tmpl.x;
		location.y = tmpl.y;

		erea.height = tmpe.height;
		erea.width = tmpe.width;

	}
	return 0;
}
