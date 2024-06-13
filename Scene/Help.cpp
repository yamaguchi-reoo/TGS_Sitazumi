#include "Help.h"
#include "DxLib.h"
#include "Title.h"
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
	if (wt < 15)
	{
		wt++;
	}
		
	// 下がる
	if (PadInput::TipLeftLStick(STICKL_Y) < -0.8f && wt >= 15)
	{
		if (++MenuNumber < 6) {}
		if (MenuNumber > 4)MenuNumber = 5;

		wt = 0;
	}
	// 上がる
	else if (PadInput::TipLeftLStick(STICKL_Y) > 0.8f && wt >= 15)
	{
		if (--MenuNumber > 5){}
		if (MenuNumber < 1) MenuNumber = 1;

		wt = 0;
	}

	// 操作方法の説明
	if (PadInput::OnButton(XINPUT_BUTTON_B) == 1)
	{
		switch (MenuNumber)
		{
		case 4:
			return new Title();
			break;
		case 5:
			return new GameMain(0);
			break;
		default:
			break;
		}
	}

	return this;
}

void Help::Draw()const
{
	//DrawString(0, 10, "Help", 0x00ff00);

	//DrawFormatString(30, 30, 0xffffff, "%d", hStrWidth);

	DrawFormatString(1100, 60, GetColor(255, 0, 0), "%d %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawFormatString(700, 100, GetColor(255, 0, 0), "menu : %d",MenuNumber);

	if (MenuNumber != 4 && MenuNumber != 5)
	{
		DrawBoxAA(320.f, 20.f, 1260.f, 700.f, 0xffffff, FALSE, 7.0f);
	}

	//LineThicknessは線の太さ
	//大外側
	DrawBoxAA(10.f, 20.f, 300.f, 130.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 160.f, 300.f, 270.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 300.f, 300.f, 410.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 440.f, 300.f, 560.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 590.f, 300.f, 700.f, 0xeeeeee, FALSE, 7.0f);

	// 中心線
	DrawLineAA(0.f, 360.f, 1280.f, 360.f, 0xff0000);
	DrawLineAA(640.f, 0.f, 640.f, 720.f, 0xff0000);

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
		
	case 1:	//操作方法

		//カーソル描画
		DrawLineAA(30.f, 100.f, 280.f, 100.f, 0xff0000, 5.0f);

		DrawCircleAA(500.f, 85.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(500.f, 85.f, 30.f, 32, 0x525156);
		DrawString(610, 60, "左スティック：移動", 0xffffff);

		DrawCircleAA(500.f, 215.f, 50.f,  32, 0x1c2b3e);
		DrawCircleAA(500.f, 215.f, 30.f, 32, 0x557b0e);
		DrawString(488, 190, "A", 0xffffff);
		DrawString(630, 190, "Aボタン：ジャンプ", 0xffffff);

		DrawCircleAA(500.f, 355.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(500.f, 355.f, 30.f, 32, 0xc5482c);
		DrawString(488, 332, "B", 0xffffff);
		DrawString(630, 310, "Bボタン\n(長押し)", 0xffffff);
		DrawString(840, 330, "：色の交換", 0xffffff);

		DrawCircleAA(390.f, 500.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 500.f, 30.f, 32, 0xc5482c);
		DrawString(380, 478, "B", 0xffffff);
		DrawString(440, 478, "＋", 0xffffff);
		DrawCircleAA(540.f, 500.f, 50.f, 32, 0x1c2b3e);
		DrawBox(525, 460, 555, 545, 0x211d1e, TRUE);
		DrawBox(500, 490, 580, 515, 0x211d1e, TRUE);
		DrawString(630, 450, "B(長押し中)\n十字カーソル", 0xffffff);
		DrawString(925, 470, "：色の選択", 0xffffff);

		DrawCircleAA(390.f, 620.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 620.f, 30.f, 32, 0xc5482c);
		DrawString(380, 595, "B", 0xffffff);
		DrawString(440, 595, "＋", 0xffffff);
		DrawCircleAA(540.f, 620.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(540.f, 620.f, 30.f, 32, 0x525156);
		DrawString(630, 578, "B(長押し中)\n左スティック", 0xffffff);
		DrawString(925, 590, "：色の選択", 0xffffff);

		break;
	case 2:	//色
		DrawLineAA(30.f, 240.f, 280.f, 240.f, 0xff0000, 5.0f);
		break;
	case 3: //属性

		//カーソル描画
		DrawLineAA(30.f, 380.f, 280.f, 380.f, 0xff0000, 5.0f);

		DrawCircleAA(770.f, 140.f, 100.f, 32, 0xcc0000, TRUE);
		DrawLineAA(690.f, 244.f, 580.f, 445.f, 0xffffff, 10.0f);
		DrawLineAA(648.f, 250.f, 690.f, 246.f, 0xffffff, 10.0f);
		DrawLineAA(687.f, 241.f, 695.f, 285.f, 0xffffff, 10.0f);

		DrawCircleAA(510.f, 550.f, 100.f, 32, 0x3c78d8, TRUE);

		DrawLineAA(618.f, 554.f, 930.f, 554.f, 0xffffff, 10.0f);
		DrawLineAA(620.f, 555.f, 652.f, 521.f, 0xffffff, 10.0f);
		DrawLineAA(620.f, 555.f, 655.f, 576.f, 0xffffff, 10.0f);

		DrawCircleAA(1060.f, 550.f, 100.f, 32, 0x6aa84f, TRUE);
		
		DrawLineAA(840.f, 240.f, 980.f, 445.f, 0xffffff, 10.0f);
		DrawLineAA(832.f, 291.f, 840.f, 240.f, 0xffffff, 10.0f);
		DrawLineAA(840.f, 240.f, 891.f, 250.f, 0xffffff, 10.0f);

		SetFontSize(30);
		DrawString(650, 370, "矢印の方向に強い", 0xffffff);
		DrawString(710, 110, "   赤\n(火属性)", 0xFFFF55);
		DrawString(450, 525, "   青\n(水属性)", 0xFFFF55);
		DrawString(1000, 520, "   緑\n(木属性)", 0xFFFF55);

		break;
	case 4: //タイトルへ
		DrawLineAA(30.f, 520.f, 280.f, 520.f, 0xff0000, 5.0f);
		break;
	case 5:
		DrawLineAA(30.f, 683.f, 280.f, 683.f, 0xff0000, 5.0f);
		break;
	default:
		break;
	}
}
