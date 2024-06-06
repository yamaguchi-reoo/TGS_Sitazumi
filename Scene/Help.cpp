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

	return this;
}

void Help::Draw()const
{
	DrawString(0, 10, "Help", 0x00ff00);

	//DrawFormatString(30, 30, 0xffffff, "%d", hStrWidth);

	DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);

	//LineThicknessは線の太さ
	//大外側
	DrawBoxAA(20.f, 20.f, 1260.f, 700.f, 0xeeeeee, FALSE, 10.0f);
	DrawBoxAA(60.f, 60.f, 1220.f, 660.f, 0xffffff, FALSE, 5.0f);
	//DrawTriangleAA(20.f, 20.f, 60.f, 60.f, 20.f, 60.f, 0xffffff, TRUE, 10.0f);

	// 中心線
	DrawLineAA(0.f, 360.f, 1280.f, 360.f, 0xff0000);
	DrawLineAA(640.f, 0.f, 640.f, 720.f, 0xff0000);
	
	DrawLineAA(0.f, 180.f, 1280.f, 180.f, 0xff0000);
	DrawLineAA(0.f, 540.f, 1280.f, 540.f, 0xff0000);
	DrawBoxAA(0.f, 0.f, 1280.f, 720.f, 0xff0000, FALSE);

	 SetFontSize(90); //デフォルトでは 6
	// 多分漢字の一文字の長さは 25
	DrawString(458, 130, "操作方法", 0xffffff);
	DrawString(410, 310, "色について", 0xffffff);
	DrawString(550, 495, "属性", 0xffffff);

	// カーソル
	DrawBox(450, 130, 550, 210, 0xff0000, FALSE);
}
