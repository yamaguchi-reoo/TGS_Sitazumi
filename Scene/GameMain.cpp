#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include "EditScene.h"
#include <fstream>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

static Location camera_location = { 0,0};	//カメラの座標
static Location screen_origin = { (SCREEN_WIDTH / 2),(SCREEN_HEIGHT / 2) };

GameMain::GameMain(int _stage) :frame(0),stage_data{0},now_stage(0), object_num(0),stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false),player_object(0), boss_object(0),weather(0), weather_timer(0), move_object_num(0)
{
	now_stage = _stage;
}

GameMain::~GameMain()
{

}

void GameMain::Initialize()
{

	weather = new WeatherManager();
	weather->Initialize();

	effect_spawner = new EffectSpawner();
	effect_spawner->Initialize();

	SetStage(now_stage);

	back_ground = new BackGround();
	back_ground->Initialize(stage_width);

	lock_pos = camera_location;

	test = new BossAttackFire();
	test->Initialize({ 1000,2000 }, { 40,40 }, RED, 1000);
}

void GameMain::Finalize()
{
	for (int i = 0; object[i] != nullptr; i++)
	{
		//生成済みのオブジェクトを削除
		object[i]->Finalize();
		delete object[i];
	}
	weather->Finalize();
	delete weather;

	effect_spawner->Finalize();
	delete effect_spawner;

	back_ground->Finalize();
	delete back_ground;
}

AbstractScene* GameMain::Update()
{
	//リセット
	move_object_num = 0;
	//フレーム測定
	frame++;

	//カメラの更新
	UpdateCamera();

	//各オブジェクトの更新
	if (object[player_object]->GetSearchFlg() == FALSE || (object[player_object]->GetSearchFlg() == TRUE && frame % 10 == 0))
	{
		for (int i = 0; object[i] != nullptr; i++)
		{
			//プレイヤー以外のオブジェクトの更新
			if (object[i] != object[player_object])
			{
				object[i]->SetScreenPosition(camera_location);
				//画面内のオブジェクトしかUpdateしない
				if (object[i]->GetLocation().x > camera_location.x - 100 &&
					object[i]->GetLocation().x < camera_location.x + SCREEN_WIDTH + 100 &&
					object[i]->GetLocation().y > camera_location.y - 100 &&
					object[i]->GetLocation().y < camera_location.y + SCREEN_WIDTH + 100
					)
				{
					object[i]->Update(this);
					move_object_num++;
					for (int j = i + 1; object[j] != nullptr; j++)
					{
						//各オブジェクトとの当たり判定
						if (object[i]->HitBox(object[j]) && object[j] != object[player_object])
						{
							object[i]->Hit(object[j]);
							object[j]->Hit(object[i]);
						}
					}
				}
			}
		}
		//if (object[player_object]->GetLocation().x > stage_width - 2000 && ++weather_timer > 200)
		//{
		//	if (++now_weather > 3)
		//	{
		//		now_weather = 1;
		//	}
		//	weather_timer = 0;
		//}
		//管理クラスの更新
		weather->Update(this);
	}

	//プレイヤーの更新＆色探知用
	object[player_object]->SetScreenPosition(camera_location);
	object[player_object]->Update(this);
	object[boss_object]->Update(this);
	move_object_num++;
	for (int i = 0; object[i] != nullptr; i++)
	{
		if (object[i]->GetCanSwap() == TRUE && object[i]->GetObjectType() != PLAYER) {
			object[player_object]->SearchColor(object[i]);
		}
		//各オブジェクトとの当たり判定
		if (object[i]->HitBox(object[player_object]))
		{
			object[i]->Hit(object[player_object]);
			object[player_object]->Hit(object[i]);
		}
	}

	//管理クラスの更新
	effect_spawner->Update(this);

#ifdef _DEBUG
	//ステージをいじるシーンへ遷移
	if (KeyInput::OnPresed(KEY_INPUT_E) && KeyInput::OnPresed(KEY_INPUT_D))
	{
		return new EditScene(now_stage);
	}

	test->Update(this);
	test->SetScreenPosition(camera_location);
#endif

	return this;
}

void GameMain::Draw() const
{
	back_ground->Draw(camera_location);
	int pn = 0;
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

	//エフェクトの描画
	effect_spawner->Draw();

	//天気管理クラスの描画
	weather->Draw();

#ifdef _DEBUG
	DrawFormatString(100, 100, 0xffffff, "Object数:%d", object_num);
	DrawFormatString(100, 120, 0xffffff, "Updeteが呼ばれているObject数:%d", move_object_num);

	test->Draw();
#endif
}

void GameMain::CreateObject(Object* _object, Location _location, Erea _erea, int _color_data)
{
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		//空いている場所に格納する
		if (object[i] == nullptr)
		{
			object[i] = _object;
			object[i]->Initialize(_location, _erea, _color_data, i);
			//プレイヤーの配列上の位置格納
			if (object[i]->GetObjectType() == PLAYER)
			{
				player_object = i;
			}
			if (object[i]->GetObjectType() == BOSS)
			{
				boss_object = i;
			}
			object_num++;
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
		if (object[j] != nullptr)
		{
			object[j]->SetObjectPos(j);
			if (object[j]->GetObjectType() == PLAYER)
			{
				player_object--;
			}
		}
		else
		{
			break;
		}
	}
	object_num--;
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
	const char* a = "../Resource/Dat/TestStageData.txt";
	switch (_stage)
	{
	case 0:
		a = "Resource/Dat/TestStageData.txt";
		break;
	case 1:
		a = "Resource/Dat/1stStageData.txt";
		break;
	case 2:
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
	bool player_flg = false;	//プレイヤーを生成したか
	now_stage = _stage;
	//ファイルの読込
	LoadStageData(now_stage);
	for (int i = stage_height_num-1; i > 0; i--)
	{
		for (int j = 0; j < stage_width_num; j++)
		{	
			switch (stage_data[i][j])
			{
			case NULL_BLOCK:
				break;
			case WHITE_BLOCK:
			case GRAY_BLOCK:
			case RED_BLOCK:
			case GREEN_BLOCK:
			case BLUE_BLOCK:
			case FIRE_BLOCK:
			case WOOD_BLOCK:
			case WATER_BLOCK:
			case WEATHER_NORMAL:
			case WEATHER_RAIN:
			case WEATHER_FIRE:
			case WEATHER_SEED:
				//ステージ内ブロックを生成
				CreateObject(new Stage(stage_data[i][j],stage_height), { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { BOX_WIDTH ,BOX_HEIGHT }, stage_data[i][j]);
				break;
			case PLAYER_BLOCK:
				//プレイヤーの生成
				CreateObject(new Player, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { PLAYER_HEIGHT,PLAYER_WIDTH }, GREEN);
				//エフェクトの生成
				effect_spawner->SpawnEffect({ (float)j * BOX_WIDTH + PLAYER_WIDTH / 2 ,(float)i * BOX_HEIGHT + PLAYER_HEIGHT / 2 }, { 20,20}, PlayerSpawnEffect, 30,object[player_object]->GetColerData());

				player_flg = true;
				break;
			case ENEMY_DEER_RED:
			case ENEMY_DEER_GREEN:
			case ENEMY_DEER_BLUE:
				//鹿の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, ColorList[stage_data[i][j] - 10]);
				break;
			case ENEMY_BAT_RED:
			case ENEMY_BAT_GREEN:
			case ENEMY_BAT_BLUE:
				//コウモリの生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 75,118 }, ColorList[stage_data[i][j] - 13]);
				break;
			case ENEMY_FROG_RED:
			case ENEMY_FROG_GREEN:
			case ENEMY_FROG_BLUE:
				//カエルの生成
				CreateObject(new EnemyFrog, {(float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT}, {50,50}, ColorList[stage_data[i][j] - 16]);
				break;
			case ENEMY_BOSS:
				//ボスの生成
				CreateObject(new Boss, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 315,315 }, ColorList[stage_data[i][j] - 19]);
				break;
			default:
				break;
			}
		}
	}

	//プレイヤーが生成されていないなら
	if (player_flg == false)
	{
		//プレイヤーの生成
		CreateObject(new Player, { (float)100,(float)100 }, { 150,75 }, RED);
	}
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

bool GameMain::GetSearchFlg()
{
	return object[player_object]->GetSearchFlg();
}

Location GameMain::GetPlayerLocation()
{
	return object[player_object]->GetLocation();
}

Erea GameMain::GetPlayerErea()
{
	return object[player_object]->GetErea();
}

Location GameMain::GetCameraLocation()
{
	return camera_location;
}

void GameMain::SpawnEffect(Location _location, Erea _erea, int _type, int _time, int _color)
{
	effect_spawner->SpawnEffect(_location, _erea, _type, _time, _color);
}

int GameMain::Swap(Object* _object1, Object* _object2)
{
	return effect_spawner->Swap(_object1, _object2);
}
