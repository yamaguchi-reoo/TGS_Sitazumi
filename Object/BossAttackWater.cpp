#include "BossAttackWater.h"
#include "../Scene/GameMain.h"

BossAttackWater::BossAttackWater()
{
	type = BLOCK;
	can_swap = TRUE;
	can_hit = TRUE;

	flg = false;
	count = 0;
	moveFlg = false;
	hitFlg = false;
}

BossAttackWater::~BossAttackWater()
{
}

void BossAttackWater::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	color = _color_data;
	erea = _erea;
	object_pos = _object_pos;
}

void BossAttackWater::Finalize()
{
}

void BossAttackWater::Update(GameMain* _g)
{
	if (flg) {
		if (moveFlg) {
			if (velocity.y < -4.f) {
				moveFlg = false;
			}
			velocity.y -= 0.1f;
		}
		else {
			if (velocity.y > 4.f) {
				moveFlg = true;
			}
			velocity.y += 0.1f;
		}

		location.x += velocity.x;
		location.y += velocity.y;
	}
	else {
		Location player = _g->GetPlayerLocation();
		if (player.x - location.x > 0) {
			velocity.x = 3.f;
		}
		else {
			velocity.x = -3.f;
		}
		velocity.y = (float)(GetRand(6) - 3);
		flg = true;
	}

	if (hitFlg) {
		//ここで削除
	}
}

void BossAttackWater::Draw() const
{
	if (flg) {
		DrawCircleAA(local_location.x, local_location.y, erea.width, color, TRUE);
	}
}

void BossAttackWater::Hit(Object* _object)
{
	if (_object->GetObjectType() == BLOCK && _object->GetObjectType() != WOOD) {
		_object->SetColorData(color);
		hitFlg = true;
	}
}

bool BossAttackWater::SearchColor(Object* ob)
{
	return false;
}
