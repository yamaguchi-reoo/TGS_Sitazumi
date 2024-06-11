#include "Title.h"
#include "DxLib.h"
#include "GameMain.h"
#include "Help.h"
#include "End.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Title::Title() :frame(0), menu_location{ 0,0 }, menu_size{ 0,0 }, player_location{ 0,0 }, cursor_location{ 0,0 }, draw_stick_location{ 0,0 }, draw_stick_shift{ 0,0 }, anim_start{ 0,0 },current_menu(0), swap_anim_flg(false), swap_anim_timer(0), stick_angle(0.0f), button_draw(false)
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	player_location = { (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2), SCREEN_HEIGHT - PLAYER_HEIGHT-75 };
	menu_location[0] = { 250,300 };
	menu_location[1] = { 565,250 };
	menu_location[2] = { 900,300 };
	draw_stick_location = { 300,600 };
	menu_size[0] = { 75,150 };
	menu_size[1] = { 75,150 };
	menu_size[2] = { 75,150 };
}

void Title::Finalize()
{

}

AbstractScene* Title::Update()
{
	if (++frame > 6000)
	{
		frame = 0;
	}

	if (frame % 10 == 0)
	{
		button_draw = !button_draw;
	}
	stick_angle += 0.05f;
	if (stick_angle > 1)
	{
		stick_angle = 0;
	}

	//演出中は更新しない
	if (swap_anim_flg == false)
	{
		//選択メニュー更新
		if (cursor_location.x < player_location.x && cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 0;
		}
		else if (cursor_location.x > player_location.x && cursor_location.x < player_location.x + PLAYER_WIDTH && cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 1;
		}
		else if (cursor_location.x > player_location.x + PLAYER_WIDTH && cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 2;
		}
		//カーソル更新
		cursor_location.x = player_location.x + (PLAYER_WIDTH / 2) + PadInput::TipLeftLStick(STICKL_X) * 100;
		cursor_location.y = player_location.y + (PLAYER_HEIGHT / 2) - PadInput::TipLeftLStick(STICKL_Y) * 100;
	}

	draw_stick_shift.x = cosf(stick_angle * M_PI * 2) * 5;
	draw_stick_shift.y = sinf(stick_angle * M_PI * 2) * 5;


	

	//Bボタンでアニメーションを再生
	if (PadInput::OnRelease(XINPUT_BUTTON_B))
	{
		if (swap_anim_flg == false)
		{
			swap_anim_flg = true;
			anim_start = menu_location[current_menu];
		}
		//再生中に押されたらスキップ
		else
		{
			swap_anim_timer = 60;
		}
	}

	//演出開始
	if (swap_anim_flg == true)
	{
		if (swap_anim_timer < 30)
		{
			menu_location[current_menu].x += (player_location.x - anim_start.x-(menu_size[current_menu].width/4)) / 30;
			menu_location[current_menu].y += (player_location.y - anim_start.y) / 30;
		}
		else
		{
			menu_location[current_menu].y -= (SCREEN_HEIGHT / 30);
			menu_size[current_menu].height += (SCREEN_HEIGHT /15);
			menu_location[current_menu].x -= (SCREEN_WIDTH / 30);
			menu_size[current_menu].width += (SCREEN_WIDTH / 15);
		}
		//演出開始から一定時間経ったら遷移
		if (++swap_anim_timer > 60)
		{
			swap_anim_flg = false;
			swap_anim_timer = 0;
			menu_location[current_menu] = anim_start;
			menu_size[current_menu] = { 75,150 };
			switch (current_menu)
			{
			case 0:
				//ゲームメイン
				return new GameMain(0);
				break;
			case 1:
				//ヘルプ画面
				printfDx("ヘルプ画面");
				return new Help();
				break;
			case 2:
				//ゲーム終了
				printfDx("ゲーム終了");
				return new End();
				break;
			}
		}
	}
	return this;
}

void Title::Draw()const
{
	//変更前のフォントサイズを保存
	int old_size = GetFontSize();
	DrawString(0, 10, "Title", 0x00ff00);
	DrawBox(200, 50, SCREEN_WIDTH - 200, 200, 0x000000, TRUE);
	DrawBox(200, 50, SCREEN_WIDTH - 200, 200, 0xffffff, FALSE);


	//プレイヤー画像描画
	DrawBoxAA(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0x000000, TRUE);
	DrawBoxAA(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0xffffff, FALSE);
	DrawStringF(player_location.x, player_location.y, "プレイヤー画像", 0xffffff);


	DrawCircleAA(cursor_location.x, cursor_location.y, 10, 100, 0x00ff00, TRUE);
	DrawCircleAA(draw_stick_location.x, draw_stick_location.y, 15, 100, 0x666666, FALSE);
	DrawCircleAA(draw_stick_location.x + draw_stick_shift.x, draw_stick_location.y + draw_stick_shift.y, 12, 100, 0x666666, TRUE);
	SetFontSize(24);
	DrawStringF(draw_stick_location.x + 25, draw_stick_location.y-10, "Left Stick", 0xffffff);
	DrawStringF(draw_stick_location.x + 50, draw_stick_location.y+20, "&", 0xffffff);
	DrawStringF(draw_stick_location.x + 35, draw_stick_location.y+45, "B Button", 0xffffff);
	SetFontSize(16);
	if (button_draw == false)
	{
		DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, FALSE);
		DrawStringF(draw_stick_location.x-3, draw_stick_location.y + 53, "B", 0xff0000);
		//DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 63, draw_stick_location.x - 30, draw_stick_location.y + 55, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x - 22, draw_stick_location.y + 55, draw_stick_location.x - 30, draw_stick_location.y + 55, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x - 22, draw_stick_location.y + 55, draw_stick_location.x - 30, draw_stick_location.y + 47, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x - 15, draw_stick_location.y + 50, draw_stick_location.x - 30, draw_stick_location.y + 47, 0xffffff, TRUE);

		//DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 63, draw_stick_location.x + 30, draw_stick_location.y + 55, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x + 22, draw_stick_location.y + 55, draw_stick_location.x + 30, draw_stick_location.y + 55, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x + 22, draw_stick_location.y + 55, draw_stick_location.x + 30, draw_stick_location.y + 47, 0xffffff, TRUE);
		//DrawLineAA(draw_stick_location.x + 15, draw_stick_location.y + 50, draw_stick_location.x + 30, draw_stick_location.y + 47, 0xffffff, TRUE);

		DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 63, draw_stick_location.x - 25, draw_stick_location.y + 60, 0xffffff, TRUE);
		DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 58, draw_stick_location.x - 25, draw_stick_location.y + 53, 0xffffff, TRUE);
		DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 63, draw_stick_location.x + 25, draw_stick_location.y + 60, 0xffffff, TRUE);
		DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 58, draw_stick_location.x + 25, draw_stick_location.y + 53, 0xffffff, TRUE);
	}
	else
	{
		DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 55, 12, 100, 0xff0000, TRUE);
		DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, TRUE);
		DrawBoxAA(draw_stick_location.x - 12, draw_stick_location.y + 55,draw_stick_location.x + 12, draw_stick_location.y + 60, 0xff0000, TRUE);
		DrawStringF(draw_stick_location.x - 3, draw_stick_location.y + 48, "B", 0x000000);
	}

	SetFontSize(48);
	DrawString(200, 100, "ゲーム名", 0xffffff);
	//メニュー項目の描画
	for (int i = 0; i < 3; i++)
	{
		if (swap_anim_timer <= 30)
		{
			DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0x000000, TRUE);
			DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0xffffff, FALSE);
		}
		else if (current_menu == i)
		{
			DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0x000000, TRUE);
			DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0xffffff, FALSE);
		}
		if (swap_anim_timer <= 30)
		{
			DrawFormatStringF(menu_location[i].x + (menu_size[i].width / 2) - (GetDrawStringWidth(menu_string[i], strlen(menu_string[i])) / 2), menu_location[i].y + (menu_size[i].height / 2) - 24, 0xffffff, "%s", menu_string[i]);
			//選択中のメニューに交換カーソルを出す
			if (current_menu == i)
			{
				DrawCircleAA(menu_location[i].x + (menu_size[i].width / 2), menu_location[i].y + (menu_size[i].height / 2), 70, 40, 0xffff00, FALSE, 5);
			}
		}
	}
	//以前のサイズに戻す
	SetFontSize(old_size);
}
