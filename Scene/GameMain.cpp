#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include "EditScene.h"
#include <fstream>
#include <iostream>
#include <string>

static Location camera_location = { 0,0};	//カメラの座標
static Location screen_origin = { (SCREEN_WIDTH / 2),(SCREEN_HEIGHT / 2) };

GameMain::GameMain(int _stage) :stage_data{0},now_stage(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false),player_object(0)
{
	SetStage(_stage);
	lock_pos = camera_location;
}

GameMain::~GameMain()
{
	for (int i = 0; object[i] != nullptr; i++)
	{
		//生成済みのオブジェクトを削除
		object[i]->Finalize();
		delete object[i];
	}
}

AbstractScene* GameMain::Update()
{
	//カメラの更新
	UpdateCamera();

	//各オブジェクトの更新
	for (int i = 0; object[i] != nullptr; i++)
	{
		object[i]->SetScreenPosition(camera_location);
		object[i]->Update();
		for (int j = i+1; object[j] != nullptr; j++)
		{
			//各オブジェクトとの当たり判定
			if (object[i]->HitBox(object[j]) && object[i]->GetCanHit()==TRUE && object[j]->GetCanHit() == TRUE)
			{
				object[i]->Hit(object[j]->GetLocation(), object[j]->GetErea(), object[j]->GetObjectType(), object[j]->GetColerData());
				object[j]->Hit(object[i]->GetLocation(), object[i]->GetErea(), object[i]->GetObjectType(), object[i]->GetColerData());
			}
			//各オブジェクトの色交換
			if (object[i]->GetObjectType() == PLAYER) {
				if (object[j]->GetObjectType() == BLOCK && object[j]->GetCanSwap() == TRUE) {
					object[i]->SearchColor(object[j]);
				}
			}
		}
	}
#ifdef _DEBUG
	//ステージをいじるシーンへ遷移
	if (KeyInput::OnPresed(KEY_INPUT_E) && KeyInput::OnPresed(KEY_INPUT_D))
	{
		return new EditScene(now_stage);
	}
#endif

	return this;
}

void GameMain::Draw() const
{
	int pn;
	for (int i = 0; object[i] != nullptr; i++)
	{
		if (object[i]->GetObjectType() == PLAYER) {
			pn = i;
			continue;
		}
		object[i]->Draw();
	}
	//プレイヤーを最後に描画
	object[pn]->Draw();
}

void GameMain::CreateObject(Object* _object, Location _location, Erea _erea, int _color_data)
{
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		//空いている場所に格納する
		if (object[i] == nullptr)
		{
			object[i] = _object;
			object[i]->Initialize(_location, _erea, _color_data);
			//プレイヤーの配列上の位置格納
			if (object[i]->GetObjectType() == PLAYER)
			{
				player_object = i;
			}
			break;
		}
	}
}

void GameMain::DeleteObject(int i)
{
	//オブジェクトを前に寄せる
	for (int j = i; object[j] != nullptr; j++)
	{
		object[j] = object[j + 1];
	}
}

void GameMain::UpdateCamera()
{
	//X座標が画面端以外なら
	if (object[player_object]->GetCenterLocation().x > (SCREEN_WIDTH / 2) && object[player_object]->GetCenterLocation().x < stage_width - (SCREEN_WIDTH / 2))
	{
		//X座標のロックをしない
		camera_x_lock_flg = false;
		//ロック時に一度だけ入る処理をリセットする
		x_pos_set_once = false;
	}
	//X座標が画面端なら
	else
	{
		//X座標のロックをする
		camera_x_lock_flg = true;
		//固定する位置を一度だけ設定する
		if (x_pos_set_once == false)
		{
			lock_pos.x = object[player_object]->GetCenterLocation().x;
			x_pos_set_once = true;
		}
	}
	//Y座標が画面端以外なら
	if (object[player_object]->GetCenterLocation().y < stage_height - (SCREEN_HEIGHT / 2) - 10 && object[player_object]->GetCenterLocation().y>(SCREEN_HEIGHT / 2))
	{
		//Y座標のロックをしない
		camera_y_lock_flg = false;
		//ロック時に一度だけ入る処理をリセットする
		y_pos_set_once = false;
	}
	//Y座標が画面端なら
	else
	{
		//Y座標のロックをする
		camera_y_lock_flg = true;
		//固定する位置を一度だけ設定する
		if (y_pos_set_once == false)
		{
			lock_pos.y = object[player_object]->GetCenterLocation().y;
			y_pos_set_once = true;
		}
	}

	//今のステージが３（ボス）以外で、カメラ固定フラグが立ってないなら
	if (now_stage != 3)
	{
		//カメラ更新
		if (camera_x_lock_flg == false && camera_y_lock_flg == false)
		{
			camera_location.x = object[player_object]->GetCenterLocation().x - (SCREEN_WIDTH / 2);
			camera_location.y = object[player_object]->GetCenterLocation().y - (SCREEN_HEIGHT / 2);
		}
		else if (camera_x_lock_flg == false && camera_y_lock_flg == true)
		{
			camera_location.x = object[player_object]->GetCenterLocation().x - (SCREEN_WIDTH / 2);
			camera_location.y = lock_pos.y - (SCREEN_HEIGHT / 2);
		}
		else if (camera_x_lock_flg == true && camera_y_lock_flg == false)
		{
			camera_location.x = lock_pos.x - (SCREEN_WIDTH / 2);
			camera_location.y = object[player_object]->GetCenterLocation().y - (SCREEN_HEIGHT / 2);
		}
		else
		{
			camera_location.x = lock_pos.x - (SCREEN_WIDTH / 2);
			camera_location.y = lock_pos.y - (SCREEN_HEIGHT / 2);
		}
	}
	else if (now_stage == 4)
	{
		camera_location.x = screen_origin.x - (SCREEN_WIDTH / 2);
		camera_location.y = screen_origin.y + 48;
	}
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
		a = "Resource/Dat/4thStageData.txt";
		break;
	case 4:
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
	//オブジェクト作成用変数
	Location spawn;
	Erea size;
	now_stage = _stage;
	//ファイルの読込
	LoadStageData(now_stage);
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			//絶対もっと簡略化出来る 一旦これで
			switch (stage_data[i][j])
			{
			case 0:
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				//ステージ内ブロックを生成
				CreateObject(new Stage(stage_data[i][j]), { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { BOX_WIDTH ,BOX_HEIGHT }, stage_data[i][j]);
				break;
			case 9:
				//プレイヤーの生成
				CreateObject(new Player, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 150,75 }, RED);
				break;
			case 10:
				//敵（赤鹿）の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, RED);
				break;
			case 11:
				//敵（緑鹿）の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, GREEN);
				break;
			case 12:
				//敵（青鹿）の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, BLUE);
				break;
			case 13:
				//敵（赤コウモリ）の生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, RED);
				break;
			case 14:
				//敵（緑コウモリ）の生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, GREEN);
				break;
			case 15:
				//敵（青コウモリ）の生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, BLUE);
				break;
			default:
				break;
			}
		}
	}

	////プレイヤーのリスポーン
	//Location res_location = { 100,stage_height - 500 };
	//player->Respawn(res_location);

	//カメラのリセット
	ResetCamera();

	//カメラの位置がプレイヤーの位置にならないように
	x_pos_set_once = true;
	y_pos_set_once = true;
}

void GameMain::ResetCamera()
{
	camera_location.x = screen_origin.x;
	camera_location.y = screen_origin.y;
}
