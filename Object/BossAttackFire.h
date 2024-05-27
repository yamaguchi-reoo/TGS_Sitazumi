#pragma once
#include "Object.h"
class BossAttackFire : public Object
{
private:
	Location velocity;
	bool flg;
	bool hitFlg;
public:
	BossAttackFire();
	~BossAttackFire();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(GameMain* _g)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

};