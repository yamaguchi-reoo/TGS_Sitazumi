#include "DxLib.h"
#include "TutorialText.h"

TutorialText::TutorialText() : GNum(0), GColor(GREEN), Gbutton_draw{ false, false, false, false }, GGNum(0), frame(0), Gstick_angle(0.0), stage_height(0), p_c(GREEN), add_x(0), add_y(0), f_c(RED), Gdraw_stick_shift{ 0, 0 }, in_camera{ 0, 0 }
{
}

TutorialText::~TutorialText()
{
}

void TutorialText::Update(Location camera , Location _p, int height)
{

	frame++;

	in_camera = camera;
	stage_height = height;

	if (frame % 10 == 0)
	{
		Gbutton_draw[0] = !Gbutton_draw[0];
	}
	if (frame % 60 == 0)
	{
		Gbutton_draw[1] = !Gbutton_draw[1];
		//Gbutton_draw[2] = false;
	}
	if (Gbutton_draw[2] == true)
	{
		p_c = 0xffffff;
		f_c = 0xffffff;
	}
	// チュートリアルエリア：左スティック
	Gstick_angle += 0.05f;
	if (Gstick_angle > 1)
	{
		Gstick_angle = 0;
	}

	Gdraw_stick_shift.x = cosf(Gstick_angle * M_PI * 2) * 5;
	Gdraw_stick_shift.y = sinf(Gstick_angle * M_PI * 2) * 5;

	if (_p.x >= 1540.0f && _p.y <= 2680.0f && _p.x <= 2600.f && _p.y >= 2000.f)
	{
		if (GColor == GREEN && GNum != 26 || GColor == BLUE && GNum < 160)
		{
			GNum += 2;
		}
		else
		{
			GNum;
			GGNum++;
			if (GGNum > 60)
			{
				if (GColor == GREEN)
				{
					GColor = BLUE;
				}
				else
				{
					GColor = GREEN;
				}

				GGNum = 0;
				GNum = 0;
			}

		}
	}

	// >= 5400 >=2580
	if (_p.x >= 5400.f && _p.y <= 2680.f && _p.x <= 6800.f && _p.y >= 2000.f && add_x < 220)
	{
		add_x++;
	}
	else
	{
		add_x = 0;
		Gbutton_draw[3] = true;
	}

	if (frame > 59)
	{
		frame = 0;
	}

}

void TutorialText::Draw() const
{
	SetFontSize(50);

	// 最初のチュートリアルテキストボックス
	DrawBoxAA(515 - in_camera.x, stage_height - 546 - in_camera.y, 903 - in_camera.x, stage_height - 317 - in_camera.y, 0xffffff, FALSE, 3.0f);
	DrawBoxAA(518 - in_camera.x, stage_height - 543 - in_camera.y, 900 - in_camera.x, stage_height - 320 - in_camera.y, 0x555555, TRUE, 3.0f);

	// 左スティック：移動の説明
	DrawCircleAA(580 - in_camera.x, stage_height - 480 - in_camera.y, 25, 100, 0x000000, TRUE);
	DrawCircleAA(580 - in_camera.x, stage_height - 480 - in_camera.y, 25, 100, 0x666666, FALSE, 3.0f);
	DrawCircleAA(580 - in_camera.x + Gdraw_stick_shift.x, stage_height - 480 - in_camera.y + Gdraw_stick_shift.y, 18, 100, 0x666666, TRUE);
	DrawStringF(610 - in_camera.x, stage_height - 500 - in_camera.y, "：Move", 0xffffff);

	if (Gbutton_draw[0] == false)
	{
		// A ボタンイメージ描画
		DrawCircleAA(577 - in_camera.x, stage_height - 460 - in_camera.y + 60, 25, 100, 0x557b0e, FALSE);
		DrawStringF(577 - in_camera.x - 12, stage_height - 460 - in_camera.y + 38, "A", 0x557b0e);
	}
	else
	{
		DrawCircleAA(577 - in_camera.x, stage_height - 460 - in_camera.y + 55, 25, 100, 0x557b0e, TRUE);
		DrawCircleAA(577 - in_camera.x, stage_height - 460 - in_camera.y + 60, 25, 100, 0x557b0e, TRUE);
		DrawStringF(577 - in_camera.x - 12, stage_height - 460 - in_camera.y + 33, "A", 0x000000);
	}
	DrawStringF(610 - in_camera.x, stage_height - 428 - in_camera.y, "：Jump", 0xffffff);

	// 色ブロックに乗れるかチュートリアルテキスト
	DrawBoxAA(1742 - in_camera.x, stage_height - 700 - in_camera.y, 2073 - in_camera.x, stage_height - 397 - in_camera.y, 0xffffff, FALSE, 3.0f);
	DrawBoxAA(1745 - in_camera.x, stage_height - 697 - in_camera.y, 2070 - in_camera.x, stage_height - 400 - in_camera.y, 0x555555, TRUE, 3.0f);

	// 緑ブロック描画
	if (GColor == GREEN)
	{
		// 赤　マル
		DrawCircleAA(1960 - in_camera.x, stage_height - 630 - in_camera.y, 40, 100, 0xCC3300, FALSE, 10.0f);
		for (int i = 0; i < 160; i += 40)
		{
			DrawBoxAA(1800 - in_camera.x + i, stage_height - 540 - in_camera.y, 1840 - in_camera.x + i, stage_height - 500 - in_camera.y, GColor, TRUE);
			DrawBoxAA(1810 - in_camera.x + i, stage_height - 540 - in_camera.y + 20, 1815 - in_camera.x + i, stage_height - 540 - in_camera.y + 25, 0x999999, true);
			DrawBoxAA(1830 - in_camera.x + i, stage_height - 540 - in_camera.y + 15, 1835 - in_camera.x + i, stage_height - 540 - in_camera.y + 20, 0x999999, true);
			DrawBoxAA(1825 - in_camera.x + i, stage_height - 540 - in_camera.y + 35, 1830 - in_camera.x + i, stage_height - 540 - in_camera.y + 40, 0x999999, true);
		}
	}
	else
	{
		// 青 バツ
		DrawLineAA(1910 - in_camera.x, stage_height - 680 - in_camera.y, 2012 - in_camera.x, stage_height - 580 - in_camera.y, 0x3300CC, 20.0f);
		DrawLineAA(2012 - in_camera.x, stage_height - 680 - in_camera.y, 1910 - in_camera.x, stage_height - 580 - in_camera.y, 0x3300CC, 20.0f);

		// 青ブロック描画
		for (int i = 0; i < 160; i += 40)
		{
			DrawBoxAA(1800 - in_camera.x + i, stage_height - 540 - in_camera.y, 1840 - in_camera.x + i, stage_height - 500 - in_camera.y, GColor, TRUE);
		}
	}
	// チュートリアルテキスト プレイヤー描画
	GDrawPlayer(0, 0, 0, GNum, GREEN);

	// 色交換のチュートリアルテキスト
	DrawBoxAA(4100 - in_camera.x, stage_height - 650 - in_camera.y, 4503 - in_camera.x, stage_height - 400 - in_camera.y, 0xffffff, FALSE, 3.0f);
	DrawBoxAA(4103 - in_camera.x, stage_height - 647 - in_camera.y, 4500 - in_camera.x, stage_height - 403 - in_camera.y, 0x555555, TRUE, 3.0f);

	if (Gbutton_draw[1] == false)
	{
		// B ボタンイメージ描画
		DrawCircleAA(577 - in_camera.x + 3600, stage_height - 460 - in_camera.y + 60 - 200, 25, 100, 0xc5482c, FALSE);
		DrawStringF(577 - in_camera.x - 12 + 3600, stage_height - 460 - in_camera.y + 38 - 200, "B", 0xc5482c);
	}
	else
	{
		DrawCircleAA(577 - in_camera.x + 3600, stage_height - 460 - in_camera.y + 55 - 200, 25, 100, 0xc5482c, TRUE);
		DrawCircleAA(577 - in_camera.x + 3600, stage_height - 460 - in_camera.y + 60 - 200, 25, 100, 0xc5482c, TRUE);
		DrawStringF(577 - in_camera.x - 12 + 3600, stage_height - 460 - in_camera.y + 33 - 200, "B", 0x000000);
	}

	DrawStringF(4210 - in_camera.x, stage_height - 623 - in_camera.y, "＆", 0xffffff);

	// 左スティック描画
	DrawCircleAA(580 - in_camera.x + 3710, stage_height - 480 - in_camera.y - 121, 25, 100, 0x000000, TRUE);
	DrawCircleAA(580 - in_camera.x + 3710, stage_height - 480 - in_camera.y - 121, 25, 100, 0x666666, FALSE, 3.0f);
	DrawCircleAA(580 - in_camera.x + Gdraw_stick_shift.x + 3710, stage_height - 480 - in_camera.y + Gdraw_stick_shift.y - 121, 18, 100, 0x666666, TRUE);

	// チュートリアルテキスト プレイヤー描画
	GDrawPlayer(2350, -130,0, 0, GREEN);
	// チュートリアルテキスト 蛙描画
	GDrawFrag();

	// 赤ブロック
	DrawBoxAA(1800 - in_camera.x + 2600, stage_height - 540 - in_camera.y + 65, 1840 - in_camera.x + 2600, stage_height - 500 - in_camera.y + 65, RED, TRUE);
	//ResourceManager::StageBlockDraw(in_camera, 0);

	if (p_c == 0xffffff)
	{
		// player 緑
		DrawBoxAA(4170 - in_camera.x, stage_height - 490 - in_camera.y, 4190 - in_camera.x, stage_height - 470 - in_camera.y, GREEN, TRUE);
		DrawBoxAA(4170 - in_camera.x, stage_height - 490 - in_camera.y, 4190 - in_camera.x, stage_height - 470 - in_camera.y, 0x000000, FALSE);

		// frog 赤
		DrawBoxAA(4410 - in_camera.x, stage_height - 540 - in_camera.y, 4430 - in_camera.x, stage_height - 520 - in_camera.y, RED, TRUE);
		DrawBoxAA(4410 - in_camera.x, stage_height - 540 - in_camera.y, 4430 - in_camera.x, stage_height - 520 - in_camera.y, 0x000000, FALSE);
	}

	// ダメージゾーンのチュートリアルテキスト
	DrawBoxAA(5950 - in_camera.x, stage_height - 500 - in_camera.y, 6243 - in_camera.x, stage_height - 260 - in_camera.y, 0xffffff, FALSE, 3.0f);
	DrawBoxAA(5953 - in_camera.x, stage_height - 497 - in_camera.y, 6240 - in_camera.x, stage_height - 263 - in_camera.y, 0x555555, TRUE, 3.0f);

	if (Gbutton_draw[3] == false)
	{
		GDrawPlayer(4150, -300, add_x, 0, RED);
		// 青 バツ
		DrawLineAA(5960 - in_camera.x, stage_height - 480 - in_camera.y, 6030 - in_camera.x, stage_height - 410 - in_camera.y, 0x3300CC, 20.0f);
		DrawLineAA(5960 - in_camera.x, stage_height - 410 - in_camera.y, 6030 - in_camera.x, stage_height - 480 - in_camera.y, 0x3300CC, 20.0f);
	}
	else
	{
		GDrawPlayer(4150, -300, add_x, 0, BLUE);
		// 赤　マル
		DrawCircleAA(6000 - in_camera.x, stage_height - 450 - in_camera.y, 30, 100, 0xCC3300, FALSE, 10.0f);
	}
}

void TutorialText::GDrawPlayer(int xNum, int yNum, int add_x, int add_y, int _c)const
{
	//頭
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 646 + yNum - in_camera.y - add_y, 23, 15, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 646 + yNum - in_camera.y - add_y, 23, 15, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, false);

	//目
	ResourceManager::DrawRotaBox(1824 - xNum - in_camera.x - add_x, stage_height - 646 + yNum - in_camera.y - add_y, 6, 7, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, true);

	//首
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 660 + yNum - in_camera.y - add_y, 10, 5, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 660 + yNum - in_camera.y - add_y, 10, 5, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, false);

	//胴体																					
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 686 + yNum - in_camera.y - add_y, 21, 37, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 686 + yNum - in_camera.y - add_y, 21, 37, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, false);

	//バッグ
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera.x - add_x, stage_height - 686 + yNum - in_camera.y - add_y, 5, 23, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera.x - add_x, stage_height - 686 + yNum - in_camera.y - add_y, 5, 23, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, false);
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera.x - add_x, stage_height - 686 + yNum - in_camera.y - add_y, 3, 15, 1830 - in_camera.x, stage_height - 646 - in_camera.y, 0, 0x000000, true);

	//腕
	ResourceManager::DrawRotaBox(1815 - xNum - in_camera.x - add_x, stage_height - 616 + yNum - in_camera.y - add_y, 28, 7, 1830 - in_camera.x, stage_height - 616 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1815 - xNum - in_camera.x - add_x, stage_height - 616 + yNum - in_camera.y - add_y, 28, 7, 1830 - in_camera.x, stage_height - 616 - in_camera.y, 0, 0x000000, false);

	//足												 														
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 651 + yNum - in_camera.y - add_y, 7, 27, 1830 - in_camera.x, stage_height - 616 - in_camera.y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera.x - add_x, stage_height - 651 + yNum - in_camera.y - add_y, 7, 27, 1830 - in_camera.x, stage_height - 616 - in_camera.y, 0, 0x000000, false);

	//帽子　中央
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1820 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 1840 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1820 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 1840 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 0x000000, false);
	//帽子　右側
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1840 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 1850 + xNum - in_camera.x + add_x, stage_height - 655 - yNum - in_camera.y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1840 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 1850 + xNum - in_camera.x + add_x, stage_height - 655 - yNum - in_camera.y + add_y, 0x000000, false);
	//帽子　左側
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1810 + xNum - in_camera.x + add_x, stage_height - 655 - yNum - in_camera.y + add_y, 1820 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera.x + add_x, stage_height - 667 - yNum - in_camera.y + add_y, 1810 + xNum - in_camera.x + add_x, stage_height - 655 - yNum - in_camera.y + add_y, 1820 + xNum - in_camera.x + add_x, stage_height - 651 - yNum - in_camera.y + add_y, 0x000000, false);
}

void TutorialText::GDrawFrag()const
{
	//胴体
	ResourceManager::DrawRotaBox(4410 - in_camera.x, stage_height - 530 - in_camera.y, 50, 25,4410 - in_camera.x, stage_height - 530 - in_camera.y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4410 - in_camera.x, stage_height - 530 - in_camera.y, 50, 25,4410 - in_camera.x, stage_height - 530 - in_camera.y, 0, 0x000000, FALSE);

	//付け根側後ろ足
	ResourceManager::DrawRotaBox(4423 - in_camera.x, stage_height - 520 - in_camera.y, 30, 10, 4423 - in_camera.x, stage_height - 520 - in_camera.y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4423 - in_camera.x, stage_height - 520 - in_camera.y, 30, 10, 4423 - in_camera.x, stage_height - 520 - in_camera.y, 0, 0x000000, FALSE);
													  																 
	//後ろ足先端																									 
	ResourceManager::DrawRotaBox(4424 - in_camera.x, stage_height - 510 - in_camera.y, 40, 10, 4424 - in_camera.x, stage_height - 510 - in_camera.y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4424 - in_camera.x, stage_height - 510 - in_camera.y, 40, 10, 4424 - in_camera.x, stage_height - 510 - in_camera.y, 0, 0x000000, FALSE);
	
	//目
	ResourceManager::DrawRotaBox(4382 - in_camera.x, stage_height - 538 - in_camera.y, 10, 10, 4382 - in_camera.x, stage_height - 538 - in_camera.y, 0, 0xffffff, TRUE);
	ResourceManager::DrawRotaBox(4382 - in_camera.x, stage_height - 538 - in_camera.y, 10, 10, 4382 - in_camera.x, stage_height - 538 - in_camera.y, 0, 0x000000, FALSE);
}