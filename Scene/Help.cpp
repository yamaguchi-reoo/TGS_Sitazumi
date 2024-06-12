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
	//TriangleXNum = -50.f
	//TriangleYNum = 180.f

	if (wt < 15)
	{
		wt++;
	}
		
	// 下がる
	if (PadInput::TipLeftLStick(STICKL_Y) < -0.8f && wt >= 15)
	{
		//MenuY = MenuNumber * 140;
		
		if (++MenuNumber < 6) 
		{

		}

		if (MenuNumber > 4)MenuNumber = 5;

		wt = 0;
	}
	// 上がる
	else if (PadInput::TipLeftLStick(STICKL_Y) > 0.8f && wt >= 15)
	{
		//MenuY = MenuNumber * 140;
		if (--MenuNumber > 5)
		{

		}

		if (MenuNumber < 1) MenuNumber = 1;

		wt = 0;
	}

	// 操作方法の説明
	if (PadInput::OnButton(XINPUT_BUTTON_B) == 1)
	{
		flg = true;
	}

	return this;
}

void Help::Draw()const
{
	//DrawString(0, 10, "Help", 0x00ff00);

	//DrawFormatString(30, 30, 0xffffff, "%d", hStrWidth);

	DrawFormatString(700, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawFormatString(0, 80, GetColor(255, 0, 0), "CtrY : %0.1f", PadInput::TipLeftLStick(STICKL_Y));
	DrawFormatString(700, 100, GetColor(255, 0, 0), "menu : %d",MenuNumber);

	DrawBoxAA(320.f, 20.f, 1260.f, 700.f, 0xffffff, FALSE, 7.0f);

	//LineThicknessは線の太さ
	//大外側
	DrawBoxAA(10.f, 20.f, 300.f, 130.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 160.f, 300.f, 270.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 300.f, 300.f, 410.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 440.f, 300.f, 560.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 590.f, 300.f, 700.f, 0xeeeeee, FALSE, 7.0f);
	//DrawTriangleAA(20.f, 20.f, 60.f, 60.f, 20.f, 60.f, 0xffffff, TRUE, 10.0f);

	// 中心線
	DrawLineAA(0.f, 360.f, 1280.f, 360.f, 0xff0000);
	DrawLineAA(640.f, 0.f, 640.f, 720.f, 0xff0000);

	// 上
	//DrawLineAA(0.f, 180.f, 1280.f, 180.f, 0xff0000);
	// 下
	//DrawLineAA(0.f, 540.f, 1280.f, 540.f, 0xff0000);
	// 大外枠線
	//DrawBoxAA(0.f, 0.f, 1280.f, 720.f, 0xff0000, FALSE);

	SetFontSize(40);
	DrawString(70, 600, " ゲーム\nスタート", 0xffffff);

	SetFontSize(50); //デフォルトでは 6
	// 多分漢字の一文字の長さは 25
	DrawString(50, 50, "操作方法", 0xffffff);
	DrawString(27, 190, "色について", 0xffffff);
	DrawString(105, 330, "属性", 0xffffff);
	DrawString(25, 475, "タイトルへ", 0xffffff);

	switch (MenuNumber)
	{
	case 1:
		DrawLineAA(30.f, 100.f, 280.f, 100.f, 0xff0000, 5.0f);
		break;
	case 2:
		DrawLineAA(30.f, 240.f, 280.f, 240.f, 0xff0000, 5.0f);
		break;
	case 3:
		DrawLineAA(30.f, 380.f, 280.f, 380.f, 0xff0000, 5.0f);
		break;
	case 4:
		DrawLineAA(30.f, 520.f, 280.f, 520.f, 0xff0000, 5.0f);
		break;
	case 5:
		DrawLineAA(30.f, 683.f, 280.f, 683.f, 0xff0000, 5.0f);
		break;
	default:
		break;
	}

	if (flg == false)
	{
		
	}

	// カーソル
	//DrawBox(460, 130, 547, 220, 0xff0000, FALSE);
	//DrawTriangleAA(460.f, 130.f, 547.f, 175.f, 460.f, 220.f, 0xff0000, TRUE);	//+ 87
	//DrawTriangleAA(370.f + -50.f, 130.f + 180.f, 457.f - 50.f, 175.f + 180.f, 370.f - 50.f, 220.f + 180.f, 0xffffff, TRUE);
	//DrawTriangleAA(370.f + -50.f, 130.f + MenuY, 457.f + - 50.f, 175.f + MenuY, 370.f + - 50.f, 220.f + MenuY, 0xffffff, TRUE);
	//DrawTriangleAA(904.f, 130.f + MenuY, 817.f, 175.f + MenuY, 904.f, 220.f + MenuY, 0xffffff, TRUE);
}
