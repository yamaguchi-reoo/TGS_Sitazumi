#include "BossAttackFire.h"
#include "../Scene/GameMain.h"


BossAttackFire::BossAttackFire()
{
	type = BLOCK;
	can_swap = TRUE;
	can_hit = TRUE;

	flg = false;
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
		Location boss = { 1000,2000 };
		Location v;
		float len = sqrtf(powf(player.x - boss.x, 2) + powf(player.y - boss.y, 2));
		v.x = (player.x - boss.x) / len;
		v.y = (player.y - boss.y) / len;
		
		velocity.x = v.x * 3;
		velocity.y = v.y * 3 + (GetRand(6) - 3);

		flg = true;
	}

	location.x += velocity.x;
	location.y += velocity.y;
}

void BossAttackFire::Draw() const
{
	DrawCircle(local_location.x, local_location.y, erea.width, color, TRUE);
}

void BossAttackFire::Hit(Object* _object)
{
}

bool BossAttackFire::SearchColor(Object* ob)
{
	return false;
}
