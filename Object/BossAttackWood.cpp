#include "BossAttackWood.h"
#include "../Scene/GameMain.h"

BossAttackWood::BossAttackWood()
{
	count = 0;
	type = 0;

	
	type = BLOCK;
	can_swap = TRUE;
	can_hit = TRUE;
	
}

BossAttackWood::~BossAttackWood()
{
}

void BossAttackWood::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	color = _color_data;

	object_pos = _object_pos;

	velocity.x = 0.f;
	velocity.y = -10.f;

	erea.height = 0.f;
	erea.width = _erea.width;
	bambooHeight = _location.y + _erea.height;

	knot = (int)(_erea.height / 40.f);
}

void BossAttackWood::Finalize()
{
}

void BossAttackWood::Update(GameMain* _g)
{
	count++;

	if (count > 180) {
		MoveBamboo();
	}
	else {//プレイヤーを探す
		if (location.x - _g->GetPlayerLocation().x < 0.f) {//プレイヤーより右
			velocity.x += 0.1f;
			if (velocity.x > 5.f) {
				velocity.x = 5.f;
			}
		}
		else if (location.x - _g->GetPlayerLocation().x > 0.f) {
			velocity.x -= 0.1f;
			if (velocity.x < -5.f) {
				velocity.x = -5.f;
			}
		}

		location.x += velocity.x;
	}
}

void BossAttackWood::Draw() const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
}

void BossAttackWood::Hit(Object* _object)
{
	if (_object->GetObjectType() == BLOCK && _object->GetObjectType() != FIRE) {
		_object->SetColorData(color);
	}
}

bool BossAttackWood::SearchColor(Object* ob)
{
	return false;
}

void BossAttackWood::MoveBamboo()
{
	//location.x += velocity.x;	//上に向かって生えるからX方向いらないかも
	//location.y += velocity.y;

	erea.height += velocity.y;

	if (erea.height > bambooHeight) {
		velocity = { 0.f,0.f };
	}
}
