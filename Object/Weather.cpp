#include"Weather.h"
#include "../Scene/GameMain.h"

Weather::Weather(int _type):touch_ground(false)
{
	type = _type;
	can_swap = FALSE;
	can_hit = TRUE;
}

Weather::~Weather()
{

}

void Weather::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	object_pos = _object_pos;
}

void Weather::Update(GameMain* _g)
{
	location.y += 20;
	if (touch_ground == true)
	{
		_g->DeleteObject(object_pos);
	}
}

void Weather::Draw()const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
}

void Weather::Finalize()
{

}

void Weather::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	touch_ground = true;
}