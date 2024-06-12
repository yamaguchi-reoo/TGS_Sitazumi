#include "Title.h"
#include "DxLib.h"
#include "GameMain.h"
#include "Help.h"
#include "End.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Title::Title() :frame(0), menu_location{ 0,0 }, menu_size{ 0,0 }, player_location{ 0,0 }, cursor_location{ 0,0 }, draw_stick_location{ 0,0 }, draw_stick_shift{ 0,0 }, anim_start{ 0,0 }, current_menu(0), swap_anim_flg(false), swap_anim_timer(0), stick_angle(0.0f), button_draw(false), end_game_flg(false), end_game_count(0), title_image_handle(0), a_num(0)
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

	int xnum = (SCREEN_WIDTH / cellSize_) + 1;
	int ynum = (SCREEN_HEIGHT / cellSize_) + 1;
	tiles_.reserve(xnum * ynum);

	for (int yidx = 0; yidx < ynum; ++yidx) {
		for (int xidx = 0; xidx < xnum; ++xidx) {
			tiles_.push_back({ xidx,yidx });
		}
	}
	std::shuffle(tiles_.begin(), tiles_.end(), mt_);

	logo_img = ResourceManager::SetGraph("Resource/Images/Logo1.png");
	//GraphFilter(ResourceManager::GetGraph(logo_img), DX_GRAPH_FILTER_GAUSS, 16, 1400);
	GetGraphSizeF(ResourceManager::GetGraph(logo_img), &logo_size.width, &logo_size.height);

	swap_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap.wav");

	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			bg[i][j].flg = true;
			bg[i][j].location = { (float)i * 40,(float)j * 40 };
			bg[i][j].erea = { 40,40 };
			bg[i][j].move_flg = false;
			bg[i][j].move_goal = { 0,0 };
			bg[i][j].move_speed = 6;
			bg[i][j].color = GetRandColor();
			bg[i][j].move_rad = 0.0f;
			bg[i][j].anim_size = 0;
		}
	}
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

	//背景の更新
	bgUpdate();

	//演出中もしくはエンドアニメーション中は更新しない
	if (swap_anim_flg == false && end_game_flg == false)
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
	if (PadInput::OnRelease(XINPUT_BUTTON_B) && end_game_flg == false)
	{
		if (swap_anim_flg == false)
		{
			swap_anim_flg = true;
			anim_start = menu_location[current_menu];
		}
		//再生中に押されたらスキップ
		else
		{
			swap_anim_timer = 255;
		}
	}

	//演出開始
	if (swap_anim_flg == true)
	{
		//エンド画面ならすぐ終了
		if (current_menu == 2)
		{
			end_game_flg = true;
			title_image_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
			GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, title_image_handle);
		}
		//演出１つ目
		//if (swap_anim_timer < 30)
		//{
		//	menu_location[current_menu].x += (player_location.x - anim_start.x-(menu_size[current_menu].width/4)) / 30;
		//	menu_location[current_menu].y += (player_location.y - anim_start.y) / 30;
		//}
		//else
		//{
		//	menu_location[current_menu].y -= (SCREEN_HEIGHT / 30);
		//	menu_size[current_menu].height += (SCREEN_HEIGHT /15);
		//	menu_location[current_menu].x -= (SCREEN_WIDTH / 30);
		//	menu_size[current_menu].width += (SCREEN_WIDTH / 15);
		//}
				//演出開始から一定時間経ったら遷移
		//if (++swap_anim_timer > 60)
		//{
		//	swap_anim_flg = false;
		//	swap_anim_timer = 0;
		//	menu_location[current_menu] = anim_start;
		//	menu_size[current_menu] = { 75,150 };
		//	switch (current_menu)
		//	{
		//	case 0:
		//		//ゲームメイン
		//		return new GameMain(0);
		//		break;
		//	case 1:
		//		//ヘルプ画面
		//		printfDx("ヘルプ画面");
		//		return new Help();
		//		break;
		//	case 2:
		//		//ゲーム終了
		//		printfDx("ゲーム終了");
		//		/*return new End();*/
		//		end_game_flg = true;
		//		title_image_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
		//		GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, title_image_handle);
		//		break;
		//	}
		//}
	
		//演出二つ目
		swap_anim_timer++;
		int w, h, try_num=0;
		for (int i = 0; i < 3; i++)
		{
			do {
				w = GetRand(BG_BLOCK_WIDTH_NUM - 1);
				h = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
				try_num++;
			} while (bg[w][h].flg == false && try_num < 100);
			bg[w][h].flg = false;
			if (try_num >= 100 || swap_anim_timer > 255)
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
					/*return new End();*/
					end_game_flg = true;
					title_image_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
					GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, title_image_handle);
					break;
				}
			}
		}
	}
	if (end_game_flg == true)
	{
		GameEnd();
		if (end_game_count > 90)
		{
			return nullptr;
		}
	}
	return this;
}

void Title::Draw()const
{
	//変更前のフォントサイズを保存
	int old_size = GetFontSize();
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);
	if (end_game_flg == false)
	{
		//動いていないブロックを先に描画
		for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
		{
			for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
			{
				if (bg[i][j].move_flg == false && bg[i][j].flg == true)
				{
					DrawBoxAA(bg[i][j].location.x- bg[i][j].anim_size, 
						bg[i][j].location.y- bg[i][j].anim_size, 
						bg[i][j].location.x + bg[i][j].erea.width + bg[i][j].anim_size, 
						bg[i][j].location.y + bg[i][j].erea.height+ bg[i][j].anim_size, bg[i][j].color, TRUE);
					DrawBoxAA(bg[i][j].location.x- bg[i][j].anim_size, 
						bg[i][j].location.y- bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.width+ bg[i][j].anim_size, 
						bg[i][j].location.y + bg[i][j].erea.height+ bg[i][j].anim_size, 0x444444, FALSE);
				}
			}
		}
		//動いているブロックを描画
		for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
		{
			for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
			{
				if (bg[i][j].move_flg == true && bg[i][j].flg == true)
				{
					DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
						bg[i][j].location.y - bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.width + bg[i][j].anim_size,
						bg[i][j].location.y + bg[i][j].erea.height + bg[i][j].anim_size, bg[i][j].color, TRUE);
					DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
						bg[i][j].location.y - bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.width + bg[i][j].anim_size,
						bg[i][j].location.y + bg[i][j].erea.height + bg[i][j].anim_size, 0x444444, FALSE);
				}
			}
		}
		DrawString(0, 10, "Title", 0x00ff00);
		DrawGraph((SCREEN_WIDTH/2)-(logo_size.width/2),50, ResourceManager::GetGraph(logo_img), TRUE);


		//プレイヤー画像描画
		DrawBoxAA(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0x000000, TRUE);
		DrawBoxAA(player_location.x, player_location.y, player_location.x + PLAYER_WIDTH, player_location.y + PLAYER_HEIGHT, 0xffffff, FALSE);
		DrawStringF(player_location.x, player_location.y, "プレイヤー画像", 0xffffff);


		DrawCircleAA(cursor_location.x, cursor_location.y, 10, 100, 0x00ff00, TRUE);
		DrawCircleAA(draw_stick_location.x, draw_stick_location.y, 15, 100, 0x000000, TRUE);
		DrawCircleAA(draw_stick_location.x, draw_stick_location.y, 15, 100, 0x666666, FALSE);
		DrawCircleAA(draw_stick_location.x + draw_stick_shift.x, draw_stick_location.y + draw_stick_shift.y, 12, 100, 0x666666, TRUE);
		SetFontSize(24);
		DrawStringF(draw_stick_location.x + 25, draw_stick_location.y - 10, "Left Stick", 0xffffff);
		DrawStringF(draw_stick_location.x + 50, draw_stick_location.y + 20, "&", 0xffffff);
		DrawStringF(draw_stick_location.x + 35, draw_stick_location.y + 45, "B Button", 0xffffff);
		SetFontSize(16);
		if (button_draw == false)
		{
			//ボタンイメージ描画
			DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, FALSE);
			DrawStringF(draw_stick_location.x - 3, draw_stick_location.y + 53, "B", 0xff0000);
			
			//押してる演出の描画
			DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 63, draw_stick_location.x - 25, draw_stick_location.y + 60, 0xffffff, TRUE);
			DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 58, draw_stick_location.x - 25, draw_stick_location.y + 53, 0xffffff, TRUE);
			DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 63, draw_stick_location.x + 25, draw_stick_location.y + 60, 0xffffff, TRUE);
			DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 58, draw_stick_location.x + 25, draw_stick_location.y + 53, 0xffffff, TRUE);
		}
		else
		{
			DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 55, 12, 100, 0xff0000, TRUE);
			DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, TRUE);
			DrawBoxAA(draw_stick_location.x - 12, draw_stick_location.y + 55, draw_stick_location.x + 12, draw_stick_location.y + 60, 0xff0000, TRUE);
			DrawStringF(draw_stick_location.x - 3, draw_stick_location.y + 48, "B", 0x000000);
		}

		SetFontSize(48);
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
		if (swap_anim_flg == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, swap_anim_timer);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	else
	{
		//DrawGraph(0, 0, title_image_handle, TRUE);
		SetDrawScreen(DX_SCREEN_BACK);
		auto rate = (float)end_game_count / (float)interval_;
		for (const auto& cell : tiles_) {
			DrawRectGraph(
				cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cellSize_, cellSize_,
				title_image_handle, true);
			DrawBoxAA(cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cell.xidx * cellSize_ + cellSize_,
				cell.yidx * cellSize_ + cellSize_,
				0xffffff, FALSE);
		}
	}
	//以前のサイズに戻す
	SetFontSize(old_size);
}

void Title::GameEnd()
{
	end_game_count++;
	SetDrawScreen(DX_SCREEN_BACK);
	if (end_game_count > 90) {
		return;
	}
	if (end_game_count % 5 == 0)
	{
		ResourceManager::StartSound(swap_se);
	}
	int xnum = (SCREEN_WIDTH / cellSize_) + 1;
	int ynum = (SCREEN_HEIGHT / cellSize_) + 1;
	int eraseNum = ((xnum * ynum) / interval_);
	if (tiles_.size() > eraseNum) {
		tiles_.erase(tiles_.end() - eraseNum, tiles_.end());
	}
	else {
		tiles_.clear();
	}
}

void Title::bgUpdate()
{
	//背景の交換
	if (frame % 120 == 0 || PadInput::OnRelease(XINPUT_BUTTON_A))
	{
		int w1, h1, w2, h2,try_num=0;
		do {
			w1 = GetRand(BG_BLOCK_WIDTH_NUM - 1);
			h1 = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
			try_num++;
		} while (bg[w1][h1].move_flg == true && try_num < 100);
		if (try_num > 100)
		{
			return;
		}
		do {
			w2 = GetRand(BG_BLOCK_WIDTH_NUM - 1);
			h2 = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
			try_num++;
		} while (bg[w2][h2].move_flg == true && try_num < 100);
		if (try_num > 100)
		{
			return;
		}

		bg[w1][h1].move_flg = true;
		bg[w1][h1].move_goal = bg[w2][h2].location;
		bg[w1][h1].move_rad = atan2f(bg[w1][h1].move_goal.y - bg[w1][h1].location.y, bg[w1][h1].move_goal.x - bg[w1][h1].location.x);
		bg[w2][h2].move_flg = true;
		bg[w2][h2].move_goal = bg[w1][h1].location;
		bg[w2][h2].move_rad = atan2f(bg[w2][h2].move_goal.y - bg[w2][h2].location.y, bg[w2][h2].move_goal.x - bg[w2][h2].location.x);
	}
	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			if (bg[i][j].move_flg == true)
			{
				bg[i][j].location.x += bg[i][j].move_speed * cosf(bg[i][j].move_rad);
				bg[i][j].location.y += bg[i][j].move_speed * sinf(bg[i][j].move_rad);
				if (bg[i][j].location.x > bg[i][j].move_goal.x - 5 &&
					bg[i][j].location.x < bg[i][j].move_goal.x + 5 &&
					bg[i][j].location.y > bg[i][j].move_goal.y - 5 &&
					bg[i][j].location.y < bg[i][j].move_goal.y + 5)
				{
					bg[i][j].location = bg[i][j].move_goal;
					bg[i][j].move_flg = false;
					bg[i][j].anim_size = 10;
				}
			}
			else if (bg[i][j].anim_size > 0)
			{
				bg[i][j].anim_size--;
			}
		}
	}
}

int Title::GetRandColor()
{
	int rand = GetRand(99);
	if (rand >= 0 && rand < 85)
	{
		return 0x000000;
	}
	else if (rand >= 85 && rand < 90)
	{
		return 0x440000;
	}
	else if (rand >= 90 && rand < 95)
	{
		return 0x004400;
	}
	else if (rand >= 95&& rand < 100)
	{
		return 0x000044;
	}
	return 0x222222;
}