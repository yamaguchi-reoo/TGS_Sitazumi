#include "BossAttackFire.h"
#include "../Scene/GameMain.h"


BossAttackFire::BossAttackFire()
{
	type = FIRE;
	can_swap = FALSE;
	can_hit = FALSE;

	flg = false;
	hitFlg = false;
}

BossAttackFire::~BossAttackFire()
{

}

void BossAttackFire::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	color = _color_data;
	erea = _erea;
	object_pos = _object_pos;
}

void BossAttackFire::Finalize()
{
}

void BossAttackFire::Update(GameMain* _g)
{
	if (!flg) {
		//プレイヤーとボスの座標からベクトルを計算
		Location player = _g->GetPlayerLocation();
		Location boss = { _g->GetBossLocation()};
		Location v;
		float len = sqrtf(powf(player.x - boss.x, 2) + powf(player.y - boss.y + (GetRand(6) - 3), 2));
		v.x = (player.x - boss.x) / len;
		v.y = (player.y - boss.y) / len;
		
		velocity.x = v.x * 5;
		velocity.y = v.y * 5;

		flg = true;
	}

	location.x += velocity.x;
	location.y += velocity.y;

	if (hitFlg) {
		//ここで削除
		_g->DeleteObject(object_pos);
	}
}

void BossAttackFire::Draw() const
{
	DrawCircleAA(local_location.x, local_location.y, erea.width, color, TRUE);
}

void BossAttackFire::Hit(Object* _object)
{
	if (_object->GetObjectType() == BLOCK && _object->GetObjectType() != WATER) {
		_object->SetColorData(color);
		_object->SetCanSwap(TRUE);
		hitFlg = true;
	}
}

bool BossAttackFire::SearchColor(Object* ob)
{
	return false;
}
