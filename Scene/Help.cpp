#include "Help.h"
#include "DxLib.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include <string.h>

Help::Help()
{

}

Help::~Help()
{

}

void Help::Initialize()
{

}

void Help::Finalize()
{

}

AbstractScene* Help::Update()
{
	/*hStrLen = strlen("作");
	hStrWidth = GetDrawStringWidth("作", hStrLen);*/

	//if (PadInput::TipLeftLStick(STICKL_Y) > 0.1f)
	//{
	//	if (++MenuNumber < 3);
	// 
	// TriangleXNum = -50.f
	// TriangleYNum = 180.f
	//}
	//else if (PadInput::TipLeftLStick(STICKL_Y) < 0.f)
	//{
	//	//if (++MenuNumber < 3);
	//	//if (MenuNumber > 2)MenuNumber = 0;
	//}

	return this;
}

void Help::Draw()const
{
	DrawString(0, 10, "Help", 0x00ff00);

	//DrawFormatString(30, 30, 0xffffff, "%d", hStrWidth);

	DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	DrawFormatString(0, 80, GetColor(255, 0, 0), "CtrY : %0.1f", PadInput::TipLeftLStick(STICKL_Y));
	DrawFormatString(0, 100, GetColor(255, 0, 0), "menu : %d",MenuNumber);

	//LineThicknessは線の太さ
	//大外側
	DrawBoxAA(20.f, 20.f, 1260.f, 700.f, 0xeeeeee, FALSE, 10.0f);
	DrawBoxAA(60.f, 60.f, 1220.f, 660.f, 0xffffff, FALSE, 10.0f);
	//DrawTriangleAA(20.f, 20.f, 60.f, 60.f, 20.f, 60.f, 0xffffff, TRUE, 10.0f);

	// 中心線
	DrawLineAA(0.f, 360.f, 1280.f, 360.f, 0xff0000);
	DrawLineAA(640.f, 0.f, 640.f, 720.f, 0xff0000);
	
	// 上
	DrawLineAA(0.f, 180.f, 1280.f, 180.f, 0xff0000);
	// 下
	//DrawLineAA(0.f, 540.f, 1280.f, 540.f, 0xff0000);
	// 大外枠線
	//DrawBoxAA(0.f, 0.f, 1280.f, 720.f, 0xff0000, FALSE);

	//DrawLine()

	 SetFontSize(90); //デフォルトでは 6
	// 多分漢字の一文字の長さは 25
	DrawString(458, 130, "操作方法", 0xffffff);
	DrawString(410, 310, "色について", 0xffffff);
	DrawString(550, 495, "属性", 0xffffff);

	// カーソル
	DrawBox(460, 130, 547, 220, 0xff0000, FALSE);
	//DrawTriangleAA(460.f, 130.f, 547.f, 175.f, 460.f, 220.f, 0xff0000, TRUE);	+ 87
	DrawTriangleAA(370.f + -50.f, 130.f + 180.f, 457.f - 50.f, 175.f + 180.f, 370.f - 50.f, 220.f + 180.f, 0xffffff, TRUE);
	DrawTriangleAA(904.f, 130.f, 817.f, 175.f, 904.f, 220.f, 0xffffff, TRUE);
}
