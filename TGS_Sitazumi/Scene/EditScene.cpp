#include "EditScene.h"
#include <fstream>
#include <iostream>
#include <string>

#include "GameMain.h"

Location camera_location = { 0,0 };

EditScene::EditScene(int _stage): current_type(0), tool_pickup_flg(false), current_leftbutton_flg(false), current_rightbutton_flg(false), current_upbutton_flg(false), current_downbutton_flg(false), button_interval(0), now_select_erea(0), current_type_select(-1), now_current_type(0)
{
	now_stage = _stage;
	tool_location.x = 100;
	tool_location.y = 0;
	tool_size.width = (OBJECT_TYPE_NUM * 50) + 210;
	tool_size.height = 100;
	width_button_location = { tool_location.x + tool_size.width - 90, tool_location.y + 25 };
	height_button_location = { tool_location.x + tool_size.width - 190, tool_location.y + 35 };
	LoadStageData(now_stage);
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j] = new Stage((float)(j * BOX_WIDTH), (float)(i * BOX_HEIGHT), BOX_WIDTH, BOX_HEIGHT, stage_data[i][j]);
			stage[i][j]->SetDebugFlg();
			select_data[i][j] = false;
		}
	}
}

EditScene::~EditScene()
{
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			delete stage[i][j];
		}
	}
}

AbstractScene* EditScene::Update()
{
	//カーソルの位置更新
	cursor = KeyInput::GetMouseCursor();
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j]->Update();
			stage[i][j]->SetScreenPosition(camera_location);
		}
	}
	switch (ChechSelectErea())
	{
	case STAGE_EDIT:
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{

				if (cursor.x > stage[i][j]->GetLocalLocation().x && cursor.x<stage[i][j]->GetLocalLocation().x + BOX_WIDTH && cursor.y>stage[i][j]->GetLocalLocation().y && cursor.y < stage[i][j]->GetLocalLocation().y + BOX_HEIGHT && tool_pickup_flg == false)
				{
					//リセットしてから選択されたselect_dataをtrueにする
					ResetSelectData();
					select_data[i][j] = true;
					//変えようとしているステージのデータが変更後のデータと一緒でないなら
					if (stage_data[i][j] != current_type)
					{
						//ひとつ前の状態を保持
						if (KeyInput::OnMouse(MOUSE_INPUT_LEFT))
						{
							SaveOldData();
						}
						//更新
						if (KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT))
						{
							stage_data[i][j] = current_type;
						}
					}
					//変えようとしているステージのデータが変更後のデータと一緒でないなら
					if (stage_data[i][j] != 0)
					{
						//ひとつ前の状態を保持
						if (KeyInput::OnMouse(MOUSE_INPUT_RIGHT))
						{
							SaveOldData();
						}
						//更新
						if (KeyInput::OnPressedMouse(MOUSE_INPUT_RIGHT))
						{
							stage_data[i][j] = 0;
						}
					}
				}
			}
		}
		break;
	case TOOL_BOX:
		for (int i = 0; i < OBJECT_TYPE_NUM; i++)
		{
			if (cursor.x > tool_location.x + (i * 50) && cursor.x < tool_location.x + (i * 50) + 50 && cursor.y>tool_location.y && cursor.y < tool_location.y + 50)
			{
				if (KeyInput::OnMouse(MOUSE_INPUT_LEFT))
				{
					if (can_select_type[i] == true)
					{
						current_type_select = i;
					}
					else
					{
						current_type = i;
					}
				}
			}
		}
		//幅を減らす
		if (cursor.x > width_button_location.x && cursor.x < width_button_location.x + 15 && cursor.y>width_button_location.y && cursor.y < width_button_location.y + 25)
		{
			current_leftbutton_flg = true;
			if (KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT) && --button_interval < 0)
			{
				button_interval = 10;
				UpdateStage(stage_width_num - 1, stage_height_num);
			}
			if (KeyInput::OnReleaseMouse(MOUSE_INPUT_LEFT))
			{
				button_interval = 0;
			}
		}
		else
		{
			current_leftbutton_flg = false;
		}

		//幅を増やす
		if (cursor.x > width_button_location.x + 65 && cursor.x < width_button_location.x + 80 && cursor.y>width_button_location.y && cursor.y < width_button_location.y + 25)
		{
			current_rightbutton_flg = true;
			if (KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT) && --button_interval < 0)
			{
				button_interval = 10;
				UpdateStage(stage_width_num + 1, stage_height_num);
			}
			if (KeyInput::OnReleaseMouse(MOUSE_INPUT_LEFT))
			{
				button_interval = 0;
			}
		}
		else
		{
			current_rightbutton_flg = false;
		}

		//高さを増やす
		if (cursor.x > height_button_location.x && cursor.x < height_button_location.x + 65 && cursor.y>height_button_location.y - 15 && cursor.y < height_button_location.y)
		{
			current_upbutton_flg = true;
			if (KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT) && --button_interval < 0)
			{
				button_interval = 10;
				UpdateStage(stage_width_num, stage_height_num + 1);
			}
			if (KeyInput::OnReleaseMouse(MOUSE_INPUT_LEFT))
			{
				button_interval = 0;
			}
		}
		else
		{
			current_upbutton_flg = false;
		}

		//高さを減らす
		if (cursor.x > height_button_location.x && cursor.x < height_button_location.x + 65 && cursor.y>height_button_location.y + 25 && cursor.y < height_button_location.y + 40)
		{
			current_downbutton_flg = true;
			if (KeyInput::OnPressedMouse(MOUSE_INPUT_LEFT) && --button_interval < 0)
			{
				button_interval = 10;
				UpdateStage(stage_width_num, stage_height_num - 1);
			}
			if (KeyInput::OnReleaseMouse(MOUSE_INPUT_LEFT))
			{
				button_interval = 0;
			}
		}
		else
		{
			current_downbutton_flg = false;
		}
		//つかんで動かす
		if (KeyInput::OnPressedMouse(MOUSE_INPUT_RIGHT))
		{
			tool_pickup_flg = true;
		}
		break;
	case SELECT_TYPE:

		break;
	default:
		break;
	}
	//つかんで動かす
	if (tool_pickup_flg == true)
	{
		MoveInsideScreen();
	}
	if (KeyInput::OnReleaseMouse(MOUSE_INPUT_RIGHT))
	{
		tool_pickup_flg = false;
	}

	//操作取り消し
	if (KeyInput::OnPresed(KEY_INPUT_LCONTROL) && KeyInput::OnKey(KEY_INPUT_Z))
	{
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				stage_data[i][j] = old_stage_data[i][j];
			}
		}
	}

	//ステージの更新
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j]->SetStageType(stage_data[i][j]);
		}
	}

	//カメラ座標を動かす
	if (KeyInput::OnPresed(KEY_INPUT_W))
	{
		camera_location.y -= 10;
	}
	if (KeyInput::OnPresed(KEY_INPUT_A))
	{
		camera_location.x -= 10;
	}
	if (KeyInput::OnPresed(KEY_INPUT_S))
	{
		camera_location.y += 10;
	}
	if (KeyInput::OnPresed(KEY_INPUT_D))
	{
		camera_location.x += 10;
	}

	//ゲームメインに戻る
	if (KeyInput::OnKey(KEY_INPUT_B))
	{
		UpdateStageData(now_stage);
		return new GameMain(now_stage);
	}

	//全部無に
	if (KeyInput::OnKey(KEY_INPUT_0))
	{
		SaveOldData();
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				stage_data[i][j] = 0;
			}
		}
	}
	return this;
}

void EditScene::Draw()const
{
	int old_size = GetFontSize();
	SetFontSize(24);
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j]->Draw();
			if (select_data[i][j] == true)
			{
				DrawBoxAA(stage[i][j]->GetLocalLocation().x, stage[i][j]->GetLocalLocation().y, stage[i][j]->GetLocalLocation().x + BOX_WIDTH, stage[i][j]->GetLocalLocation().y + BOX_HEIGHT, 0xff0000, false);
			}
		}
	}
	SetFontSize(16);
	DrawBoxAA(tool_location.x, tool_location.y, tool_location.x + tool_size.width, tool_location.y + tool_size.height, 0x000000, true);
	DrawBoxAA(tool_location.x, tool_location.y, tool_location.x + tool_size.width, tool_location.y + tool_size.height, 0xffffff, false);
	DrawStringF(tool_location.x, tool_location.y + 60, "左クリックで選択＆配置", 0xffffff);
	DrawStringF(tool_location.x, tool_location.y + 80, "ctrl+zで一つ戻る", 0xffffff);
	DrawStringF(tool_location.x + tool_size.width - 270, tool_location.y + 80, "Bキーで保存＆ゲームメインへ戻る", 0xffffff);

	//現在選択中のオブジェクトを分かりやすく	
	for (int i = 0; i < OBJECT_TYPE_NUM; i++)
	{
		if (current_type == i)
		{
			DrawBoxAA(tool_location.x + (i * 50), tool_location.y, tool_location.x + (i * 50) + 50, tool_location.y + 50, 0xffffff, true);
			DrawBoxAA(tool_location.x + (i * 50), tool_location.y, tool_location.x + (i * 50) + 50, tool_location.y + 50, 0x000000, false);
			DrawFormatStringF(tool_location.x + (i * 50), tool_location.y + 15, 0x000000, "%s", obj_string[i]);
		}
		else
		{
			DrawBoxAA(tool_location.x + (i * 50), tool_location.y, tool_location.x + (i * 50) + 50, tool_location.y + 50, 0x000000, true);
			DrawBoxAA(tool_location.x + (i * 50), tool_location.y, tool_location.x + (i * 50) + 50, tool_location.y + 50, 0xffffff, false);
			DrawFormatStringF(tool_location.x + (i * 50), tool_location.y + 15, 0xffffff, "%s", obj_string[i]);
		}
	}

	//ステージ幅変更用表示
	DrawStringF(width_button_location.x - 5, width_button_location.y - 20, "ステージ幅", 0xffffff);

	//ステージの横のブロック数が表示されるエリア
	DrawBoxAA(width_button_location.x + 15, width_button_location.y, width_button_location.x + 65, width_button_location.y + 25, 0x000000, true);
	DrawBoxAA(width_button_location.x + 15, width_button_location.y, width_button_location.x + 65, width_button_location.y + 25, 0xffffff, false);
	DrawFormatStringF(width_button_location.x + 25, width_button_location.y + 5, 0xffffff, "%d", stage_width_num);

	//ステージサイズ変更ボタン
	if (current_leftbutton_flg == false)
	{
		DrawBoxAA(width_button_location.x, width_button_location.y, width_button_location.x + 15, width_button_location.y + 25, 0x000000, true);
		DrawBoxAA(width_button_location.x, width_button_location.y, width_button_location.x + 15, width_button_location.y + 25, 0xffffff, false);
		DrawStringF(width_button_location.x + 5, width_button_location.y + 5, "<", 0xffffff);
	}
	else
	{
		DrawBoxAA(width_button_location.x, width_button_location.y, width_button_location.x + 15, width_button_location.y + 25, 0xffffff, true);
		DrawBoxAA(width_button_location.x, width_button_location.y, width_button_location.x + 15, width_button_location.y + 25, 0x000000, false);
		DrawStringF(width_button_location.x + 5, width_button_location.y + 5, "<", 0x000000);
	}
	if (current_rightbutton_flg == false)
	{
		DrawBoxAA(width_button_location.x + 65, width_button_location.y, width_button_location.x + 80, width_button_location.y + 25, 0x000000, true);
		DrawBoxAA(width_button_location.x + 65, width_button_location.y, width_button_location.x + 80, width_button_location.y + 25, 0xffffff, false);
		DrawStringF(width_button_location.x + 70, width_button_location.y + 5, ">", 0xffffff);
	}
	else
	{
		DrawBoxAA(width_button_location.x + 65, width_button_location.y, width_button_location.x + 80, width_button_location.y + 25, 0xffffff, true);
		DrawBoxAA(width_button_location.x + 65, width_button_location.y, width_button_location.x + 80, width_button_location.y + 25, 0x000000, false);
		DrawStringF(width_button_location.x + 70, width_button_location.y + 5, ">", 0x000000);
	}

	//ステージ高さ変更用表示
	DrawStringF(height_button_location.x - 15, height_button_location.y - 30, "ステージ高さ", 0xffffff);

	//ステージの縦のブロック数が表示されるエリア
	DrawBoxAA(height_button_location.x, height_button_location.y, height_button_location.x + 65, height_button_location.y + 25, 0x000000, true);
	DrawBoxAA(height_button_location.x, height_button_location.y, height_button_location.x + 65, height_button_location.y + 25, 0xffffff, false);
	DrawFormatStringF(height_button_location.x + 25, height_button_location.y + 5, 0xffffff, "%d", stage_height_num);

	//ステージサイズ変更ボタン
	if (current_upbutton_flg == false)
	{
		DrawBoxAA(height_button_location.x, height_button_location.y - 15, height_button_location.x + 65, height_button_location.y, 0x000000, true);
		DrawBoxAA(height_button_location.x, height_button_location.y - 15, height_button_location.x + 65, height_button_location.y, 0xffffff, false);
		DrawRotaStringF(height_button_location.x + 40, height_button_location.y - 10, 1, 1, 0, 0, 1.6f, 0xffffff, 0, 0, "<");
	}
	else
	{
		DrawBoxAA(height_button_location.x, height_button_location.y - 15, height_button_location.x + 65, height_button_location.y, 0xffffff, true);
		DrawBoxAA(height_button_location.x, height_button_location.y - 15, height_button_location.x + 65, height_button_location.y, 0x000000, false);
		DrawRotaStringF(height_button_location.x + 40, height_button_location.y - 10, 1, 1, 0, 0, 1.6f, 0x000000, 0, 0, "<");
	}
	//ステージサイズ変更ボタン
	if (current_downbutton_flg == false)
	{
		DrawBoxAA(height_button_location.x, height_button_location.y + 25, height_button_location.x + 65, height_button_location.y + 40, 0x000000, true);
		DrawBoxAA(height_button_location.x, height_button_location.y + 25, height_button_location.x + 65, height_button_location.y + 40, 0xffffff, false);
		DrawRotaStringF(height_button_location.x + 25, height_button_location.y + 35, 1, 1, 0, 0, 4.7f, 0xffffff, 0, 0, "<");
	}
	else
	{
		DrawBoxAA(height_button_location.x, height_button_location.y + 25, height_button_location.x + 65, height_button_location.y + 40, 0xffffff, true);
		DrawBoxAA(height_button_location.x, height_button_location.y + 25, height_button_location.x + 65, height_button_location.y + 40, 0x000000, false);
		DrawRotaStringF(height_button_location.x + 25, height_button_location.y + 35, 1, 1, 0, 0, 4.7f, 0x000000, 0, 0, "<");
	}
	
	if (current_type_select != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	SetFontSize(old_size);
}

void EditScene::LoadStageData(int _stage)
{
	const char* a = "resource/dat/1stStageData.txt";
	switch (_stage)
	{
	case 0:
		a = "resource/dat/1stStageData.txt";
		break;
	case 1:
		a = "resource/dat/2ndStageData.txt";
		break;
	case 2:
		a = "resource/dat/3rdStageData.txt";
		break;
	case 3:
		a = "resource/dat/BossStageData.txt";
		break;
	}

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> stage_width_num;
		file >> stage_height_num;
		//ランキングデータ配分列データを読み込む
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				file >> stage_data[i][j];
				old_stage_data[i][j] = stage_data[i][j];
			}
		}
	}
}

void EditScene::UpdateStageData(int _stage)
{
	const char* a = "resource/dat/1stStageData.txt";
	switch (_stage)
	{
	case 0:
		a = "resource/dat/1stStageData.txt";
		break;
	case 1:
		a = "resource/dat/2ndStageData.txt";
		break;
	case 2:
		a = "resource/dat/3rdStageData.txt";
		break;
	case 3:
		a = "resource/dat/BossStageData.txt";
		break;
	}
	std::ofstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file << stage_width_num << "\n";
		file << stage_height_num << "\n";
		//ランキングデータ配分列データを読み込む
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				file << stage_data[i][j] << "\n";
			}
		}
	}
}

void EditScene::UpdateStage(int _width, int _height)
{
	int old_stage_height_num = stage_height_num;
	stage_width_num = _width;
	stage_height_num = _height;
	int stage_height_shift = stage_height_num - old_stage_height_num;
	//拡張の上限と縮小の下限
	if (stage_width_num > MAX_STAGE_WIDTH)
	{
		stage_width_num = MAX_STAGE_WIDTH;
	}
	if (stage_width_num <= 0)
	{
		stage_width_num = 1;
	}
	if (stage_height_num > MAX_STAGE_HEIGHT)
	{
		stage_height_num = MAX_STAGE_HEIGHT;
	}
	if (stage_height_num <= 0)
	{
		stage_height_num = 1;
	}
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			if (stage_data[i][j] < 0)
			{
				stage_data[i][j] = 0;
				stage[i][j] = new Stage((float)(j * BOX_WIDTH), (float)(i * BOX_HEIGHT), BOX_WIDTH, BOX_HEIGHT, stage_data[i][j]);
				stage[i][j]->SetDebugFlg();
				select_data[i][j] = false;
			}
		}
	}
	StageShift(stage_height_shift);
}

void EditScene::SaveOldData()
{
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			old_stage_data[i][j] = stage_data[i][j];
		}
	}
}

void EditScene::StageShift(int _num)
{
	if (_num > 0)
	{
		for (int i = stage_height_num - 1; i >= 0; i--)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				if (i - _num >= 0)
				{
					stage_data[i][j] = stage_data[i - _num][j];
				}
				else
				{
					stage_data[i][j] = 0;
				}
			}
		}
	}
	else if (_num < 0)
	{
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				if (i - _num < stage_height_num + 1)
				{
					stage_data[i][j] = stage_data[i - _num][j];

				}
				else
				{
					stage_data[i][j] = 0;
				}
			}
		}
	}
}

int EditScene::ChechSelectErea()
{
	//ブロックのタイプ選択画面が開かれて居るか確認
	if (current_type_select != -1)
	{
		return SELECT_TYPE;
	}
	//カーソルがツールボックス内かどうか判断
	if (cursor.x > tool_location.x && cursor.x < tool_location.x + tool_size.width && cursor.y>tool_location.y && cursor.y < tool_location.y + tool_size.height)
	{
		return TOOL_BOX;
	}
	else
	{
		//ツールボックスをつかんでいなければツールボックス外の処理
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				if (cursor.x > stage[i][j]->GetLocalLocation().x && cursor.x<stage[i][j]->GetLocalLocation().x + BOX_WIDTH && cursor.y>stage[i][j]->GetLocalLocation().y && cursor.y < stage[i][j]->GetLocalLocation().y + BOX_HEIGHT && tool_pickup_flg == false)
				{
					return STAGE_EDIT;
				}
			}
		}
	}
	return -1;
}

void EditScene::MoveInsideScreen()
{
	//スクリーン内から出ないようにツールボックスのX座標をマウスに沿って移動
	tool_location.x = cursor.x - (tool_size.width / 2);
	if (tool_location.x < 0)
	{
		tool_location.x = 0;
	}
	else if (tool_location.x + tool_size.width > SCREEN_WIDTH)
	{
		tool_location.x = SCREEN_WIDTH - tool_size.width;
	}
	width_button_location.x = tool_location.x + (tool_size.width - WIDTH_BUTTON_POS_X);
	height_button_location.x = tool_location.x + (tool_size.width - HEIGHT_BUTTON_POS_X);

	//スクリーン内から出ないようにツールボックスのY座標をマウスに沿って移動
	tool_location.y = cursor.y - (tool_size.height / 2);
	if (tool_location.y < 0)
	{
		tool_location.y = 0;
	}
	else if (tool_location.y + tool_size.height > SCREEN_HEIGHT)
	{
		tool_location.y = SCREEN_HEIGHT - tool_size.height;
	}
	width_button_location.y = tool_location.y + WIDTH_BUTTON_POS_Y;
	height_button_location.y = tool_location.y + HEIGHT_BUTTON_POS_Y;
}

void EditScene::ResetSelectData()
{
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			select_data[i][j] = false;
		}
	}
}