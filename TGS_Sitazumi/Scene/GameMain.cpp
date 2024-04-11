#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include <fstream>
#include <iostream>
#include <string>

static Location camera_location = { 0,0};	//カメラの座標
static Location screen_origin = { (SCREEN_WIDTH / 2),0 };

GameMain::GameMain(int _stage):stage{0},now_stage(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0)
{
	for (int i = 0; i < MAX_STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < MAX_STAGE_WIDTH; j++)
		{
			stage_data[i][j] = 0;
		}
	}
	now_stage = _stage;
	SetStage(now_stage);
}

GameMain::~GameMain()
{
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			delete stage[i][j];
		}
	}
}

AbstractScene* GameMain::Update()
{
	//カメラの更新
	UpdateCamera();

	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j]->SetScreenPosition(camera_location);
			stage[i][j]->Update();
		}
	}
	return this;
}

void GameMain::Draw() const
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			DrawFormatString(10 + j * 20, 10 + i * 20, 0x00ff00, "%d",stage_data[i][j]);
		}
	}
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			stage[i][j]->Draw();
		}
	}
}

void GameMain::UpdateCamera()
{
	////X座標が画面端以外なら
	//if (player->GetCenterLocation().x > (SCREEN_WIDTH / 2) && player->GetCenterLocation().x < stage_width - (SCREEN_WIDTH / 2))
	//{
	//	//X座標のロックをしない
	//	camera_x_lock_flg = false;
	//	//ロック時に一度だけ入る処理をリセットする
	//	x_pos_set_once = false;
	//}
	////X座標が画面端なら
	//else
	//{
	//	//X座標のロックをする
	//	camera_x_lock_flg = true;
	//	//固定する位置を一度だけ設定する
	//	if (x_pos_set_once == false)
	//	{
	//		lock_pos.x = player->GetCenterLocation().x;
	//		x_pos_set_once = true;
	//	}
	//}
	////Y座標が画面端以外なら
	//if (player->GetCenterLocation().y < stage_height - (SCREEN_HEIGHT / 2) - 10 && player->GetCenterLocation().y>(SCREEN_HEIGHT / 2))
	//{
	//	//Y座標のロックをしない
	//	camera_y_lock_flg = false;
	//	//ロック時に一度だけ入る処理をリセットする
	//	y_pos_set_once = false;
	//}
	////Y座標が画面端なら
	//else
	//{
	//	//Y座標のロックをする
	//	camera_y_lock_flg = true;
	//	//固定する位置を一度だけ設定する
	//	if (y_pos_set_once == false)
	//	{
	//		lock_pos.y = player->GetCenterLocation().y;
	//		y_pos_set_once = true;
	//	}
	//}

	////今のステージが３（ボス）以外で、カメラ固定フラグが立ってないなら
	//if (now_stage != 3 && (lock_flg == 0 || lock_flg == 6))
	//{
		//カメラ更新
		//if (camera_x_lock_flg == false && camera_y_lock_flg == false)
		//{
		//	CameraLocation(player->GetCenterLocation().x, player->GetCenterLocation().y);
		//}
		//else if (camera_x_lock_flg == false && camera_y_lock_flg == true)
		//{
		//	CameraLocation(player->GetCenterLocation().x, lock_pos.y);
		//}
		//else if (camera_x_lock_flg == true && camera_y_lock_flg == false)
		//{
		//	CameraLocation(lock_pos.x, player->GetCenterLocation().y);
		//}
		//else
		//{
		//	CameraLocation(lock_pos.x, lock_pos.y);
		//}
	//}
	//else if (now_stage == 3)
	//{
	//	CameraLocation(screen_origin.x, screen_origin.y + (SCREEN_HEIGHT / 2) + 48);
	//}
	//else if (lock_flg != 0)
	//{
	//	CameraLocation(battle_start_pos[now_battle].x, battle_start_pos[now_battle].y - (SCREEN_HEIGHT / 2) + 96);
	//}
}

void GameMain::LoadStageData(int _stage)
{
	const char* a = "../Resource/Dat/1stStageData.txt";
	switch (_stage)
	{
	case 0:
		a = "Resource/Dat/1stStageData.txt";
		break;
	case 1:
		a = "Resource/Dat/2ndStageData.txt";
		break;
	case 2:
		a = "Resource/Dat/3rdStageData.txt";
		break;
	case 3:
		a = "Resource/Dat/BossStageData.txt";
		break;
	}

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> stage_width_num;
		file >> stage_height_num;

		stage_width = stage_width_num * BOX_WIDTH;
		stage_height = stage_height_num * BOX_HEIGHT;
		//配列データを読み込む
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				file >> stage_data[i][j];
			}
		}
	}
}

void GameMain::SetStage(int _stage)
{
	//ファイルの読込
	LoadStageData(now_stage);
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			//ステージ内ブロックを生成
			stage[i][j] = new Stage((float)(j * BOX_WIDTH), (float)(i * BOX_HEIGHT), BOX_WIDTH, BOX_HEIGHT, stage_data[i][j]);
		}
	}

	//ステージのマップチップ用
	//for (int i = 0; i < stage_height_num; i++)
	//{
	//	for (int j = 0; j < stage_width_num; j++)
	//	{
	//		stage[i][j]->SetDrawType(GetStageType(i - 1, j), GetStageType(i, j - 1), GetStageType(i, j + 1), GetStageType(i + 1, j));
	//	}
	//}

	////プレイヤーのリスポーン
	//Location res_location = { 100,stage_height - 500 };
	//player->Respawn(res_location);

	////カメラのリセット
	//ResetCamera();

	////カメラの位置がプレイヤーの位置にならないように
	//x_pos_set_once = true;
	//y_pos_set_once = true;
}

int GameMain::GetStageType(int _i, int _j)
{
	int ret = -1;
	if (stage[_i][_j] == nullptr || _i >= stage_height_num || _j >= stage_width_num)
	{
		//無を返す
		ret = -1;
	}
	else
	{
		ret = stage[_i][_j]->GetStageType();
	}
	return ret;
}
