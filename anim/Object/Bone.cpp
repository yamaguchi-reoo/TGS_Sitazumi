#include "Bone.h"
#include "../Utility/inputCtrl.h"
#include "../Utility/calculation.h"

Bone::Bone()
{
	for (int i = 0; i < 32; i++)
	{
		movedAng[i] = 0.f;
		oneFrameAng[i] = 0.f;
		time[i] = 0;
	}
	angle = 0.f;
	flg = false;
	num = 0;

	for (int i = 0; i < 2; i++)
	{
		BoneLoc[i] = { -1,-1 };
		BoneLocBase[i] = { -1,-1 };
	}

	centerLoc = { -1,-1 };
}

Bone::~Bone()
{
}

void Bone::Initialize(Vector2D sl, Vector2D gl)
{
	centerLoc = sl;
	BoneLoc[0] = sl - centerLoc;
	BoneLoc[1] = gl - centerLoc;

	BoneLocBase[0] = sl - centerLoc;
	BoneLocBase[1] = gl - centerLoc;
}

void Bone::Update()
{
	if (flg) {
		angle += oneFrameAng[num];
		BoneLoc[1] = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);
		if (angle >= movedAng[num]) {
			flg = false;
			num++;
		}
	}
}

void Bone::Draw() const
{
	//DrawLine(BoneLoc[0].x, BoneLoc[0].y, BoneLoc[1].x, BoneLoc[1].y, 0xffffff, 1);
	DrawLine(BoneLoc[0].x + centerLoc.x, BoneLoc[0].y + centerLoc.y, 
		BoneLoc[1].x + centerLoc.x, BoneLoc[1].y + centerLoc.y, 0xffffff, 1);

	for (int i = 0; i < 2; i++)
	{
		if (BoneLoc[i].x != -1) {
			DrawCircle(BoneLoc[i].x + centerLoc.x, BoneLoc[i].y + centerLoc.y, 5, 0xffffff, TRUE);
		}
	}

	if (flg) {
		Vector2D tmp = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], movedAng[num]);
		DrawCircle(tmp.x + centerLoc.x, tmp.y + centerLoc.y, 5, 0xff0000, TRUE);
	}
}




