#include "Help.h"
#include "DxLib.h"
#include "Title.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"

Help::Help():l_help_red{ 360.f, 120.f }, l_help_blue{ 420.f, 120.f }, l_help_fire{ 360, 330 }, l_help_water{ 420, 330 }, l_help_wood{ 480, 330 }, l_help_bamboo{ 540, 330 }, l_help_respawn{ 450, 600 }
{

}

Help::~Help()
{

}

void Help::Initialize()
{
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
	decision_se = ResourceManager::SetSound("Resource/Sounds/System/decision.wav");
	ResourceManager::SetSoundVolume(decision_se, 200);
}

void Help::Finalize()
{

}

AbstractScene* Help::Update()
{
	if (++frame > 6000)
	{
		frame = 0;
	}

	if (frame % 10 == 0)
	{
		button_draw = !button_draw;
	}

	if (wt < 15)
	{
		wt++;
	}
		
	// 下がる
	if (PadInput::TipLeftLStick(STICKL_Y) < -0.8f && wt >= 15)
	{
		ResourceManager::StartSound(cursor_se);
		if (++MenuNumber < 6) {}
		if (MenuNumber > 4)MenuNumber = 5;

		wt = 0;
	}
	// 上がる
	else if (PadInput::TipLeftLStick(STICKL_Y) > 0.8f && wt >= 15)
	{
		ResourceManager::StartSound(cursor_se);
		if (--MenuNumber > 5){}
		if (MenuNumber < 1) MenuNumber = 1;

		wt = 0;
	}

	// 操作方法の説明
	if (PadInput::OnButton(XINPUT_BUTTON_B) == 1)
	{
		ResourceManager::StartSound(decision_se);
		switch (MenuNumber)
		{
		case 5:
			return new Title();
			break;
		default:
			break;
		}
	}

	return this;
}

void Help::Draw()const
{
	DrawFormatString(1100, 60, GetColor(255, 0, 0), "%d %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawFormatString(700, 100, GetColor(255, 0, 0), "menu : %d",MenuNumber);

	
	DrawBoxAA(320.f, 20.f, 1260.f, 700.f, 0xffffff, FALSE, 7.0f);
	

	//LineThicknessは線の太さ
	//大外側
	DrawBoxAA(10.f, 20.f, 300.f, 130.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 160.f, 300.f, 270.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 300.f, 300.f, 410.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 440.f, 300.f, 560.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 590.f, 300.f, 700.f, 0xeeeeee, FALSE, 7.0f);

	// 中心線
	//DrawLineAA(0.f, 360.f, 1280.f, 360.f, 0xff0000);
	//DrawLineAA(640.f, 0.f, 640.f, 720.f, 0xff0000);

	//SetFontSize(40);
	//DrawString(70, 600, " GAME\nSTART", 0xffffff);

	SetFontSize(49); //デフォルトでは 
	// 多分漢字の一文字の長さは 25
	DrawString(40, 50, "UserGuide", 0xffffff);	//操作方法
	DrawString(80, 190, "Player", 0xffffff);		//Playerについて
	DrawString(100, 330, "Stage", 0xffffff);		//ステージにある色ブロックについて(色ブロックとかダメージゾーンとか)
	DrawString(40, 475, "TypeChart", 0xffffff);	//属性表
	DrawString(70, 620, " Title", 0xffffff);		//タイトルへ

	switch (MenuNumber)
	{
		
	case 1:	//操作方法

		//カーソル描画
		DrawLineAA(30.f, 100.f, 280.f, 100.f, 0xff0000, 5.0f);

		DrawCircleAA(500.f, 85.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(500.f, 85.f, 30.f, 32, 0x525156);
		DrawString(610, 60, "LeftStick: Move", 0xffffff);	//左スティック：移動

		DrawCircleAA(500.f, 215.f, 50.f,  32, 0x1c2b3e);
		DrawCircleAA(500.f, 215.f, 30.f, 32, 0x557b0e);
		DrawString(488, 190, "A", 0xffffff);
		DrawString(630, 190, "A Button: Jump", 0xffffff);	//Aボタン：ジャンプ

		DrawCircleAA(500.f, 355.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(500.f, 355.f, 30.f, 32, 0xc5482c);
		DrawString(488, 332, "B", 0xffffff);
		DrawString(630, 310, "B Button\n(LongPress)", 0xffffff);	//Bボタン\n(長押し)
		DrawString(925, 330, ":ColorSwap", 0xffffff);			//：色の交換

		DrawCircleAA(390.f, 500.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 500.f, 30.f, 32, 0xc5482c);
		DrawString(380, 478, "B", 0xffffff);
		DrawString(440, 478, "＋", 0xffffff);
		DrawCircleAA(540.f, 500.f, 50.f, 32, 0x1c2b3e);
		DrawBox(525, 460, 555, 545, 0x211d1e, TRUE);
		DrawBox(500, 490, 580, 515, 0x211d1e, TRUE);
		DrawString(630, 450, "B(Pressing)\nD-Pad", 0xffffff);	//B(長押し中)\n十字カーソル
		DrawString(925, 470, ":ColorSelect", 0xffffff);					//：色の選択

		DrawCircleAA(390.f, 620.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 620.f, 30.f, 32, 0xc5482c);
		DrawString(380, 595, "B", 0xffffff);
		DrawString(440, 595, "＋", 0xffffff);
		DrawCircleAA(540.f, 620.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(540.f, 620.f, 30.f, 32, 0x525156);
		DrawString(630, 578, "B(Pressing)\nLeftStick", 0xffffff);	//B(長押し中)\n左スティック
		DrawString(925, 590, ":ColorSelect", 0xffffff);					//：色の選択

		break;

	case 2: //Player 何をしたらダメージを受けるのか、何回ダメージを受けたら死ぬのかの説明

		DrawLineAA(30.f, 240.f, 280.f, 240.f, 0xff0000, 5.0f);


	
		break;

	case 3:	//色 横並びで説明 
		
		DrawLineAA(30.f, 380.f, 280.f, 380.f, 0xff0000, 5.0f);

		DrawBoxAA(340, 98, 542, 182, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(342, 100, 540, 180, 0x555555, TRUE, 2.0f);

		// 色の交換できるブロックの説明
		DrawBoxAA(360.f, 60.f + 60, 400.f, 100.f + 60, RED, TRUE);
		ResourceManager::StageBlockDraw(l_help_red, 0);

		DrawBoxAA(420.f, 60.f + 60, 460.f, 100.f + 60, BLUE, TRUE);
		ResourceManager::StageBlockDraw(l_help_blue, 1);

		DrawBoxAA(480.f, 60.f + 60, 520.f, 100.f + 60, GREEN, TRUE);
		DrawBoxAA(490.f, 80.f + 60, 495.f, 85.f + 60, 0x00ee00, TRUE);
		DrawBoxAA(505.f, 95.f + 60, 510.f, 100.f + 60, 0x00ee00, TRUE);
		DrawBoxAA(510.f, 75.f + 60, 515.f, 80.f + 60, 0x00ee00, TRUE);
		
		DrawString(620, 100, " Color Block\n(ColorSwap OK)", 0xffffff);

		DrawBoxAA(340, 298, 612, 402, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(342, 300, 610, 400, 0x555555, TRUE, 2.0f);

		// ダメージゾーンの説明
		ResourceManager::StageAnimDraw(l_help_fire, 3);
		ResourceManager::StageAnimDraw(l_help_water, 5);
		ResourceManager::StageAnimDraw(l_help_wood, 4);

		DrawBoxAA(l_help_bamboo.x + 3, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 3, l_help_bamboo.y + BOX_HEIGHT, 0x00cc00, true);
		DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + 2, 0x00ff00, true);
		DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y + BOX_HEIGHT, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ff00, true);
		DrawBoxAA(l_help_bamboo.x + 10, l_help_bamboo.y + 2, l_help_bamboo.x + 13, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ee00, true);

		DrawString(620, 310, " Damage Zone\n(ColorSwap NG)", 0xffffff);

		DrawBoxAA(415, 568, 532, 672, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(417, 570, 530, 670, 0x555555, TRUE, 2.0f);

		// 中間地点の説明
		DrawTriangleAA(l_help_respawn.x + (BOX_WIDTH / 2), l_help_respawn.y,l_help_respawn.x + BOX_WIDTH, l_help_respawn.y + (BOX_HEIGHT / 4),l_help_respawn.x + (BOX_WIDTH / 2), l_help_respawn.y + (BOX_HEIGHT / 2), 0xffffff, TRUE);

		DrawBoxAA(l_help_respawn.x + (BOX_WIDTH / 2) - 2, l_help_respawn.y, l_help_respawn.x + (BOX_WIDTH / 2) + 2, l_help_respawn.y + BOX_HEIGHT, 0xffffff, TRUE);
		DrawBoxAA(l_help_respawn.x, l_help_respawn.y + BOX_HEIGHT - 4, l_help_respawn.x + BOX_WIDTH, l_help_respawn.y + BOX_HEIGHT, 0xffffff, TRUE);

		DrawStringF(620.f, l_help_respawn.y, "Respawn Point", 0xffffff);

		break;
	case 4: //属性

		//カーソル描画
		DrawLineAA(30.f, 520.f, 280.f, 520.f, 0xff0000, 5.0f);

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
		DrawLineAA(982.f, 440.f, 980.f, 380.f, 0xffffff, 10.0f);
		DrawLineAA(918.f, 426.f, 973.f, 440.f, 0xffffff, 10.0f);

		SetFontSize(30);
		DrawString(710, 110, "  Red\n(Fire)", 0xFFFF55);			//赤\n(火属性)
		DrawString(450, 525, "  Blue\n(Water)", 0xFFFF55);			//青\n(水属性)
		DrawString(1000, 520, " Green\n(Wood)", 0xFFFF55);			//緑\n(木属性)

		DrawString(730, 580, "Damage", 0x00ff00);
		DrawString(500, 300, "Damage", 0x0000ff);
		DrawString(970, 300, "Damage", 0xff0000);

		break;
	case 5: //タイトルへ
		
		DrawLineAA(30.f, 683.f, 280.f, 683.f, 0xff0000, 5.0f);
		//Press B button
		DrawString(630, 330, "Press B button", 0xffffff);
		if (button_draw == false)
		{
			//ボタンイメージ描画
			DrawCircleAA(550, 360, 37.5, 100, 0xff0000, FALSE);
			DrawStringF(550 - 10, 290 + 40, "B", 0xff0000);

			//押してる演出の描画
			DrawLineAA(550 - 40, 290 + 75, 550 - 60, 290 + 70, 0xffffff, TRUE);
			DrawLineAA(550 - 40, 290 + 58, 550 - 60, 290 + 53, 0xffffff, TRUE);
			DrawLineAA(550 + 40, 290 + 75, 550 + 60, 290 + 70, 0xffffff, TRUE);
			DrawLineAA(550 + 40, 290 + 58, 550 + 60, 290 + 53, 0xffffff, TRUE);
		}
		else
		{
			DrawCircleAA(550, 290 + 55, 37.5, 100, 0xff0000, TRUE);
			DrawCircleAA(550, 290 + 70, 37.5, 100, 0xff0000, TRUE);
			DrawBoxAA(550 - 20, 290 + 55, 550 + 20, 290 + 70, 0xff0000, TRUE);
			DrawStringF(550 - 10, 290 + 25, "B", 0x000000);
		}
		break;
	//case 5:
	//	DrawLineAA(30.f, 683.f, 280.f, 683.f, 0xff0000, 5.0f);
	//	//Press B button
	//	DrawString(630, 330, "Press B button", 0xffffff);
	//	if (button_draw == false)
	//	{
	//		//ボタンイメージ描画
	//		DrawCircleAA(550, 360, 37.5, 100, 0xff0000, FALSE);
	//		DrawStringF(550 - 10, 290 + 40, "B", 0xff0000);

	//		//押してる演出の描画
	//		DrawLineAA(550 - 40, 290 + 75, 550 - 60, 290 + 70, 0xffffff, TRUE);
	//		DrawLineAA(550 - 40, 290 + 58, 550 - 60, 290 + 53, 0xffffff, TRUE);
	//		DrawLineAA(550 + 40, 290 + 75, 550 + 60, 290 + 70, 0xffffff, TRUE);
	//		DrawLineAA(550 + 40, 290 + 58, 550 + 60, 290 + 53, 0xffffff, TRUE);
	//	}
	//	else
	//	{
	//		DrawCircleAA(550, 290 + 55, 37.5, 100, 0xff0000, TRUE);
	//		DrawCircleAA(550, 290 + 70, 37.5, 100, 0xff0000, TRUE);
	//		DrawBoxAA(550 - 20, 290 + 55, 550 + 20, 290 + 70, 0xff0000, TRUE);
	//		DrawStringF(550 - 10, 290 + 25, "B", 0x000000);
	//	}
	//	break;
	default:
		break;
	}

	
}


