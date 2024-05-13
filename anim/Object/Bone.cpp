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

	side = true;

	selected = false;

	cursor = 0;
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
		if (side) {//右回り
			angle += oneFrameAng[num];
			BoneLoc[1] = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);
			if (angle >= movedAng[num]) {
				flg = false;
				num++;
			}
		}
		else {//左回り
			angle -= oneFrameAng[num];
			BoneLoc[1] = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);
			float tmpAng = angle + M_PI * 2;
			if (tmpAng <= movedAng[num]) {
				flg = false;
				num++;
			}
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

	if (selected) {
		//ここでUI描画
	}
}

void Bone::SelectUpdate()
{
	Vector2D mousePos = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
	if (GetLineCollision(BoneLoc[0] + centerLoc, BoneLoc[1] + centerLoc, mousePos, 5) && !selected) {
		selected = true;
		cursor = 0;
	}

	if (selected) {
		Vector2D tmp = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], movedAng[num]) + centerLoc;
		if (GetVectorLength(tmp - mousePos) < 5) {//移動先の地点をクリックしたら
			if (InputCtrl::GetKeyState(KEY_INPUT_UP) == PRESS) {
				cursor--;
			}
			if (InputCtrl::GetKeyState(KEY_INPUT_DOWN) == PRESS) {
				cursor++;
			}

			if (flg) {
				switch (cursor)
				{
				case 0:
					num;
					cursor++;
					break;

				case 1:
					side = true;
					cursor++;
					break;

				case 2:
					movedAng[num] += M_PI;
					if (side) {//右回り
						oneFrameAng[num] = movedAng[num] / time[num];
					}
					else {//左回り
						oneFrameAng[num] = (M_PI * 2 - movedAng[num]) / time[num];
					}
					cursor++;

					break;

				case 3:
					time[num] = 180;
					if (side) {//右回り
						oneFrameAng[num] = movedAng[num] / time[num];
					}
					else {//左回り
						oneFrameAng[num] = (M_PI * 2 - movedAng[num]) / time[num];
					}
					cursor++;

					break;

				

				case 4:
					break;

				default:
					break;
				}
			}
		}
	}
}

void Bone::DrawUI() const
{
	DrawFormatString(900, 100, 0xffffff, "num : %d", num);
	DrawFormatString(900, 150, 0xffffff, "side : %d", side);
	DrawFormatString(900, 200, 0xffffff, "movedAng : %f", R_D(movedAng[num]));
	DrawFormatString(900, 250, 0xffffff, "time : %d", time[num]);
	//DrawFormatString(900, 300, 0xffffff, "num : %d", num);
}

void Bone::SetMoved(float ang, int t, int n, bool f)
{
	movedAng[n] = ang;
	time[n] = t;
	if (f) {//右回り
		oneFrameAng[n] = ang / t;
	}
	else {//左回り
		oneFrameAng[n] = (M_PI * 2 - ang) / t;
	}
	if (ang == 0) {
		oneFrameAng[n] = 0;
	}
	side = f;
	num = n;
	flg = true;
}
