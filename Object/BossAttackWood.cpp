#include "BossAttackWood.h"

BossAttackWood::BossAttackWood()
{
	count = 0;
	type = 0;
}

BossAttackWood::~BossAttackWood()
{
}

void BossAttackWood::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	//erea = _erea;
	color = _color_data;

	object_pos = _object_pos;

	velocity.x = 0.f;
	velocity.y = -1.f;

	erea.height = 0.f;
	erea.width = _erea.width;
	bambooHeight = _location.y + _erea.height;

	//local_location = { 20,720 };
	knot = (int)(_erea.height / 40.f);
}

void BossAttackWood::Finalize()
{
}

void BossAttackWood::Update(GameMain* _g)
{
	MoveBamboo();
}

void BossAttackWood::Draw() const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
}

void BossAttackWood::Hit(Object* _object)
{
}

bool BossAttackWood::SearchColor(Object* ob)
{
	return false;
}

void BossAttackWood::MoveBamboo()
{
	location.x += velocity.x;	//上に向かって生えるからX方向いらないかも
	//location.y += velocity.y;

	erea.height += velocity.y;

	if (erea.height > bambooHeight) {
		velocity = { 0.f,0.f };
	}
}
