#include "Boss.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = {_location};//x座標 ,y座標 
	erea = { _erea };		//高さ、幅	
	color = _color_data;

	object_pos = _object_pos;
}

void Boss::Update(GameMain* _g)
{
}

void Boss::Draw() const
{
	DrawCircle(local_location.x, local_location.y, 110, color, TRUE);
}

void Boss::Finalize()
{
}

void Boss::Hit(Object* _object)
{
}
