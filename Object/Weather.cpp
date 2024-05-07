#include"Weather.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"

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
	switch (type)
	{
	case FIRE:
		location.y += 7;
		if (touch_ground == true)
		{
			_g->DeleteObject(object_pos);
			//木に当たったなら木は消え、その場所に火を作る
			if (delete_object->GetObjectType() == WOOD)
			{
				_g->CreateObject(new Stage(6), delete_object->GetLocation(), delete_object->GetErea(), RED);
				_g->DeleteObject(delete_object->GetObjectPos());
			}
		}
		break;
	case WATER:
		location.y += 20;
		if (touch_ground == true)
		{
			_g->DeleteObject(object_pos);
			//火に当たったなら火は消え、その場所に水を作る
			if (delete_object->GetObjectType() == FIRE)
			{
				_g->CreateObject(new Stage(8), delete_object->GetLocation(), delete_object->GetErea(), BLUE);
				_g->DeleteObject(delete_object->GetObjectPos());
			}
		}
		break;
	case WOOD:
		location.y += 20;
		if (touch_ground == true)
		{
			_g->DeleteObject(object_pos);
			//水に当たったなら水は消え、その場所に草を作る
			if (delete_object->GetObjectType() == WATER)
			{
				_g->CreateObject(new Stage(7), delete_object->GetLocation(), delete_object->GetErea(), GREEN);
				_g->DeleteObject(delete_object->GetObjectPos());
			}
		}
		break;
	default:
		break;
	}
}

void Weather::Draw()const
{
	DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, TRUE);
}

void Weather::Finalize()
{

}

void Weather::Hit(Object* _object)
{
	touch_ground = true;
	delete_object = _object;
}