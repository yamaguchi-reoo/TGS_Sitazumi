#include <math.h>
#include "BoxCollider.h"

bool BoxCollider::HitBox(BoxCollider* bCollider)
{
	bool ret = false;

	//�����̍�����W
	float my_x = location.x;
	float my_y = location.y;
	//�����̒������W
	float my_cx = my_x + (erea.width / 2);
	float my_cy = my_y + (erea.height / 2);
	//�����̕��ƍ����̔���
	float my_harf_width = erea.width / 2;
	float my_harf_height = erea.height / 2;

	//����̍�����W
	float sub_x = bCollider->GetLocation().x;
	float sub_y = bCollider->GetLocation().y;
	//����̒������W
	float sub_cx = sub_x + (bCollider->GetErea().width / 2);
	float sub_cy = sub_y + (bCollider->GetErea().height / 2);
	//����̕��ƍ����̔���
	float sub_harf_width = bCollider->GetErea().width / 2;
	float sub_harf_height = bCollider->GetErea().height / 2;

	//�����Ƒ���̒��S���W�̍�
	float diff_x = my_cx - sub_cx;
	float diff_y = my_cy - sub_cy;

	//�����蔻��̉��Z
	if (fabsf(diff_x) < my_harf_width + sub_harf_width &&
		fabsf(diff_y) < my_harf_height + sub_harf_height)
	{
		ret = true;
	}
	return ret;
}

Location BoxCollider::GetLocation()const
{
	return location;
}
Location BoxCollider::GetCenterLocation()const
{
	Location ret = { 0 };
	ret.x = location.x + erea.width / 2;
	ret.y = location.y + erea.height / 2;
	return ret;
}
Location BoxCollider::GetLocalLocation()const
{
	return local_location;
}
Erea BoxCollider::GetErea()const
{
	return erea;
}
void BoxCollider::SetScreenPosition(Location _world_to_screen)
{
	local_location.x = location.x - _world_to_screen.x;
	local_location.y = location.y - _world_to_screen.y;
}