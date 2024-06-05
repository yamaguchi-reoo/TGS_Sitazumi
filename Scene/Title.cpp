#include "Title.h"
#include "DxLib.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Title::Title() :menu_location{ 0,0 }, menu_size{ 0,0 }, player_location{ 0,0 },current_menu(0), stick_angle(0.0f)
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	player_location = { (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2), SCREEN_HEIGHT - PLAYER_HEIGHT };
	menu_location[0] = { 100,300 };
	menu_location[1] = { 500,250 };
	menu_location[2] = { 900,300 };
	menu_size[0] = { 75,150 };
	menu_size[1] = { 75,150 };
	menu_size[2] = { 75,150 };
}

void Title::Finalize()
{

}

AbstractScene* Title::Update()
{
	stick_angle = sqrtf(powf(fabsf(PadInput::TipLeftLStick(STICKL_X)), 2) + powf(fabsf(PadInput::TipLeftLStick(STICKL_Y)), 2));

	//Aボタンで選択中のシーンへ遷移
	if (PadInput::OnButton(XINPUT_BUTTON_A))
	{
		switch (current_menu)
		{
		case 0:
			//ゲームメイン
			return new GameMain(0);
			break;
		case 1:
			//ヘルプ画面

			break;
		case 2:
			//ゲーム終了

			break;
		}
	}
	return this;
}

void Title::Draw()const
{
	DrawString(0, 10, "Title", 0x00ff00);
	DrawBox(200, 50, SCREEN_WIDTH - 200, 200, 0x000000, TRUE);
	DrawBox(200, 50, SCREEN_WIDTH - 200, 200, 0xffffff, FALSE);
	SetFontSize(48);
	DrawString(200, 100, "ゲーム名", 0xffffff);
	
	for (int i = 0; i < 3; i++)
	{
		DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0x000000, TRUE);
		DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].width, menu_location[i].y + menu_size[i].height, 0xffffff, FALSE);
		DrawFormatStringF(menu_location[i].x, menu_location[i].y, 0xffffff, "%s", menu_string[i]);
	}

	DrawBox(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0x000000, TRUE);
	DrawBox(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0xffffff, FALSE);
	DrawString(player_location.x, player_location.y, "プレイヤー画像", 0xffffff);

	DrawFormatString(100, 200, 0xffffff, "%f", stick_angle);
}
