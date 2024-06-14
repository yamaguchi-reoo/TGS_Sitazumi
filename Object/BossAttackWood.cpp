#include "BossAttackWood.h"
#include "../Scene/GameMain.h"
#include "../Utility/ResourceManager.h"

BossAttackWood::BossAttackWood()
{
	count = 0;
	w_type = 0;

	
	type = BLOCK;
	can_swap = TRUE;
	can_hit = TRUE;
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
	__super::Update(_g);

	//if (count > 180) {
	//	MoveBamboo();
	//	if (velocity.y == 0.f) {
	//		_g->DeleteObject(object_pos);
	//	}
	//}
	//else {//プレイヤーを探す
	//	if (location.x - _g->GetPlayerLocation().x < 0.f) {//プレイヤーより右
	//		velocity.x += 0.1f;
	//		if (velocity.x > 5.f) {
	//			velocity.x = 5.f;
	//		}
	//	}
	//	else if (location.x - _g->GetPlayerLocation().x > 0.f) {
	//		velocity.x -= 0.1f;
	//		if (velocity.x < -5.f) {
	//			velocity.x = -5.f;
	//		}
	//	}

	//	location.x += velocity.x;
	//}


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
	for (int i = 0; i < (erea.height / BOX_HEIGHT); i++)
	{
		ResourceManager::StageAnimDraw({ local_location.x, local_location.y + (i * BOX_HEIGHT) }, WOOD);
	}

	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
}

void BossAttackWood::Hit(Object* _object)
{
	if ((_object->GetObjectType() == BLOCK || _object->GetObjectType() == WATER) && _object->GetColorData() != WHITE) {
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
