#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/ResourceManager.h"
#include <math.h>


Player::Player()
{
	color = WHITE;

	location = { 40,200 };
	erea = { 120,60 };
	vector = { 0,0 };

	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}

	aimLoc = { 40,200 };
	aimVec = { 0,0 };
	lineLoc = { 0,0 };
}

Player::~Player() 
{
	
}

void Player::Update(GameMain* main)
{
	if (stageHitFlg[1][bottom] != true) {
		vector.y += 1.f;
		if (vector.y > 16.f) {
			vector.y = 16.f;
		}
	}
	else {
		vector.y = 0.f;
	}

	MoveActor();
	MoveAim();

	location.x += vector.x;
	location.y += vector.y;

	for (int i = 0; i < 4; i++){
		stageHitFlg[1][i] = false;
	}


}

void Player::Draw()const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height,color, FALSE);

	DrawCircle(aimLoc.x, aimLoc.y, 10, color, TRUE);

	DrawLine(local_location.x + (erea.width / 2), local_location.y + (erea.height / 2), lineLoc.x, lineLoc.y, color);

	for (int i = 0; i < 4; i++){
		DrawFormatString(20 + i * 20, 0, 0xff0000, "%d", stageHitFlg[0][i]);
		DrawFormatString(20 + i * 20, 40, 0xff0000, "%d", stageHitFlg[1][i]);
	}
	
	DrawFormatString(20 , 80, 0xff0000, "%f", location.x);
	DrawFormatString(20 , 100, 0xff0000, "%f", location.y);	
	
}

void Player::MoveActor()
{
	//�W�����v
	if (PadInput::OnButton(XINPUT_BUTTON_A) && stageHitFlg[1][bottom]) {
		vector.y = -20.f;
	}

	//���E�ړ�
	if (PadInput::TipLeftLStick(STICKL_X) > 0.2) {
		vector.x = 0.5f;
	}
	else if (PadInput::TipLeftLStick(STICKL_X) < -0.2) {
		vector.x = -0.5f;
	}
	else {
		vector.x = 0.f;
	}
}

void Player::MoveAim()
{
	//�Ə��̍��W
	aimLoc.x = (local_location.x + (erea.width / 2)) + PadInput::TipLeftLStick(STICKL_X) * 100.f;
	aimLoc.y = (local_location.y + (erea.height / 2)) + PadInput::TipLeftLStick(STICKL_Y) * -100.f;

	//�Ə��̃x�N�g��
	Location tmpv;
	float len;
	tmpv.x = aimLoc.x - (local_location.x + (erea.width / 2));
	tmpv.y = aimLoc.y - (local_location.y + (erea.height / 2));
	len = sqrtf(powf(tmpv.x, 2) + powf(tmpv.y, 2));

	//�Ə��̒P�ʃx�N�g��
	aimVec.x = tmpv.x / len;
	aimVec.y = tmpv.y / len;

	lineLoc.x = local_location.x + (erea.width / 2) + aimVec.x * 1000.f;
	lineLoc.y = local_location.y + (erea.height / 2) + aimVec.y * 1000.f;
}

bool Player::CheckCollision(Stage* stage)
{
	Location tmpl = location;
	Erea tmpe = erea;

	if (stage->GetStageType() != 0) {

		erea.height = 1.f;
		erea.width = tmpe.width - 10.f;

		//�v���C���[�̏㑤
		//location.y -= tmpe.height / 2;
		if (stage->HitBox(this) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//�v���C���[�̉���
		location.y += tmpe.height;
		if (stage->HitBox(this) && !stageHitFlg[1][bottom]) {
			stageHitFlg[0][bottom] = true;
			stageHitFlg[1][bottom] = true;
		}
		else {
			stageHitFlg[0][bottom] = false;
		}

		//���P�[�V�����A������߂�
		location.y = tmpl.y;
		erea.height = tmpe.height;
		//���܂�h�~(�㉺)
		if (stageHitFlg[0][bottom]) {//���̖��܂�h�~
			float t = stage->GetLocation().y - (location.y + erea.height);
			if (t != 0) {
				location.y += t;
			}
		}


		

		//���E�G���A�̐ݒ�
		erea.height = tmpe.height - 10;
		erea.width = 1;

		//�v���C���[�̍���
		//location.x -= tmpe.width / 2;
		if (stage->HitBox(this) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;

		}
		else {
			stageHitFlg[0][left] = false;
		}

		//�v���C���[�̉E��
		location.x += tmpe.width;
		if (stage->HitBox(this) && !stageHitFlg[1][right]) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}


		
		//���ɖ߂�
		location.x = tmpl.x;
		erea.height = tmpe.height;
		erea.width = tmpe.width;

	}
	return 0;
}
