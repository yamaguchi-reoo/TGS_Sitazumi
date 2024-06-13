#include "BossAttackWood.h"
#include "../Scene/GameMain.h"

BossAttackWood::BossAttackWood()
{
	count = 0;
	w_type = 0;

	
	type = WOOD;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;
}

BossAttackWood::~BossAttackWood()
{
}

void BossAttackWood::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location.x = _location.x;
	location.y = _location.y;
	color = _color_data;

	object_pos = _object_pos;

	velocity.x = 0.f;
	velocity.y = -10.f;

	//erea.height = 0.f;
	erea.height = _erea.height;
	erea.width = _erea.width;
	//bambooHeight = _location.y + _erea.height;
	bambooHeight = _erea.height;

	knot = (int)(_erea.height / 40.f);

	startLoc = _location;
}

void BossAttackWood::Finalize()
{
}

void BossAttackWood::Update(GameMain* _g)
{
	MoveBamboo();
	if (velocity.y == 0.f) {
		if (count++ > 180) {
			if (this != nullptr) {
				_g->DeleteObject(object_pos, this);
			}
		}
	}
}

void BossAttackWood::Draw() const
{
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);


	DrawBoxAA(local_location.x + 3, local_location.y, local_location.x + erea.width - 3, local_location.y + erea.height, 0x00cc00, true);
	DrawBoxAA(local_location.x + 2, local_location.y, local_location.x + erea.width - 2, local_location.y + 2, 0x00ff00, true);
	DrawBoxAA(local_location.x + 2, local_location.y + erea.height, local_location.x + erea.width - 2, local_location.y + erea.height - 2, 0x00ff00, true);
	DrawBoxAA(local_location.x + 10, local_location.y + 2, local_location.x + 13, local_location.y + erea.height - 2, 0x00ee00, true);
}

void BossAttackWood::Hit(Object* _object)
{
	if (_object->GetObjectType() == BLOCK && _object->GetObjectType() != FIRE && _object->GetColorData() != WHITE) {
		_object->SetCanSwap(TRUE);
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

	/*erea.height -= velocity.y;

	if (abs(erea.height) > bambooHeight) {
		velocity = { 0.f,0.f };
	}*/


	//location.x += velocity.x;	//上に向かって生えるからX方向いらないかも
	location.y += velocity.y;
	if (location.y < startLoc.y - erea.height) {
		velocity.y = 0.f;
	}
}
