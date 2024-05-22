#include "BossAttackWater.h"
#include "../Scene/GameMain.h"

BossAttackWater::BossAttackWater()
{
	type = BLOCK;
	can_swap = TRUE;
	can_hit = TRUE;

	flg = false;
	count = 0;
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
	count++;

	if (count < 180) {
		//プレイヤーとボスの座標からベクトルを計算
		Location player = _g->GetPlayerLocation();
		Location boss = { 1000,2000 };
		Location v;
		len = sqrtf(powf(player.x - boss.x, 2) + powf(player.y - boss.y, 2));
		v.x = (player.x - boss.x) / len;
		v.y = (player.y - boss.y) / len;

		unitVelocity.x = v.x;
		unitVelocity.y = v.y;

		v.x = (player.x - boss.x);
		v.y = (player.y - boss.y);

		velocity.x = v.x;
		velocity.y = v.y;
	}
	else if (count > 180 && count < 240) {

	}

	if (count == 240) {
		location.x += velocity.x;
		location.y += velocity.y;
		flg = true;
	}
}

void BossAttackWater::Draw() const
{
	if (!flg) {
		DrawLine(local_location.x, local_location.y, local_location.x + velocity.x, local_location.y + velocity.y, color, TRUE);
	}
	else {
		DrawLine(local_location.x, local_location.y, local_location.x + velocity.x, local_location.y + velocity.y, color, TRUE);
	}
}

void BossAttackWater::Hit(Object* _object)
{
	/*Location tmp = location;
	for (int i = 0; i < (int)len; i++)
	{
		location.x += unitVelocity.x * i;
		location.y += unitVelocity.y * i;
		if (HitBox(_object)) 
		{
			
		}
	}*/
}

bool BossAttackWater::SearchColor(Object* ob)
{
	return false;
}
