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

GameMain::GameMain(int _stage) :frame(0), stage_data{ 0 }, now_stage(0), object_num(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false), player_object(0), boss_object(0), weather(0), weather_timer(0), move_object_num(0),player_flg(false), player_respawn_flg(false)
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

	SetStage(now_stage, false);

	back_ground = new BackGround();
	back_ground->Initialize({ (float)stage_width,(float)stage_height });

	lock_pos = camera_location;

	test = new BossAttackWater();
	test->Initialize({ 10000,1600 }, { 40,40 }, RED, 1000);
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
				if (CheckInScreen(object[i]))
				{
					object[i]->Update(this);
					move_object_num++;
					for (int j = i + 1; object[j] != nullptr; j++)
					{
						//各オブジェクトとの当たり判定
						if (CheckInScreen(object[j]) == true && object[i]->HitBox(object[j]) && object[j] != object[player_object])
						{
							object[i]->Hit(object[j]);
							object[j]->Hit(object[i]);
						}
					}
				}
			}
		}
		//管理クラスの更新
		weather->Update(this);
	}

	//プレイヤーの更新
	PlayerUpdate();

	//管理クラスの更新
	effect_spawner->Update(this);

	//ボスステージに遷移
	if (object[player_object]->GetLocation().x > stage_width - 200 && now_stage != 2)
	{
		SetStage(2, false);
	}
#ifdef _DEBUG
	//ステージをいじるシーンへ遷移
	if (KeyInput::OnPresed(KEY_INPUT_E) && KeyInput::OnPresed(KEY_INPUT_D))
	{
		return new EditScene(now_stage);
	}

	if (KeyInput::OnKey(KEY_INPUT_1))
	{
		SetStage(0, false);
	}
	if (KeyInput::OnKey(KEY_INPUT_2))
	{
		SetStage(2, false);
	}
	//test->Update(this);
	//test->SetScreenPosition(camera_location);
#endif

	return this;
}

void GameMain::Draw() const
{
	back_ground->Draw(camera_location);
	for (int i = 0; object[i] != nullptr; i++)
	{
		if (player_object == i) {
			continue;
		}
		if (CheckInScreen(object[i]) == true)
		{
			object[i]->Draw();
		}
	}
	for (int i = 0; i < attack_num; i++)
	{
		object[boss_attack[i]]->Draw();
	}
	//プレイヤーを最後に描画
	object[player_object]->Draw();

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
			if (_erea.height == 1200.f)
			{
				boss_attack[attack_num++] = i;
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
				player_object = j;
			}
			if (object[j]->GetObjectType() == BOSS)
			{
				boss_object = j;
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
		if (now_stage == 2)
		{
			camera_location = { 0,BOX_HEIGHT * 5 };
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

void GameMain::SetStage(int _stage, bool _delete_player)
{
	object_num = 0;
	//すべてのオブジェクトを削除
	DeleteAllObject(_delete_player);

	now_stage = _stage;
	//ファイルの読込
	LoadStageData(now_stage);
	for (int i = stage_height_num - 1; i >= 0; i--)
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
			case PLAYER_RESPAWN_BLOCK:
			case WEATHER_NORMAL:
			case WEATHER_RAIN:
			case WEATHER_FIRE:
			case WEATHER_SEED:
				//ステージ内ブロックを生成
				CreateObject(new Stage(stage_data[i][j],stage_height), { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { BOX_WIDTH ,BOX_HEIGHT }, stage_data[i][j]);
				break;
			case PLAYER_BLOCK:
				//プレイヤーがリセットされないまま別のステージへ遷移する場合はリスポーン位置を変更する
				if (_delete_player == false)
				{
					//プレイヤーリスポーン地点の設定
					player_respawn = { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT };
				}
				//プレイヤーが居ないなら
				if (player_flg == false)
				{
					//プレイヤーの生成
					CreateObject(new Player, player_respawn, { PLAYER_HEIGHT,PLAYER_WIDTH }, GREEN);
					player_flg = true;
				}
				//プレイヤーが居るなら
				else
				{
					player_respawn_flg = true;
				}
				//エフェクトの生成
				effect_spawner->SpawnEffect({ player_respawn.x + PLAYER_WIDTH / 2 ,player_respawn.y + PLAYER_HEIGHT / 2 }, { 20,20 }, PlayerSpawnEffect, 30, object[player_object]->GetColerData());
				break;
			case ENEMY_DEER_RED:
			case ENEMY_DEER_GREEN:
			case ENEMY_DEER_BLUE:
				//鹿の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, ColorList[stage_data[i][j] - 11]);
				break;
			case ENEMY_BAT_RED:
			case ENEMY_BAT_GREEN:
			case ENEMY_BAT_BLUE:
				//コウモリの生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 75,118 }, ColorList[stage_data[i][j] - 14]);
				break;
			case ENEMY_FROG_RED:
			case ENEMY_FROG_GREEN:
			case ENEMY_FROG_BLUE:
				//カエルの生成
				CreateObject(new EnemyFrog, {(float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT}, {50,50}, ColorList[stage_data[i][j] - 17]);
				break;
			case ENEMY_BOSS:
				//ボスの生成
				CreateObject(new Boss, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 250,250 }, ColorList[stage_data[i][j] - 20]);
				break;
			default:
				break;
			}
		}
	}

	//プレイヤーが生成されていないなら
	if (player_flg == false)
	{
		//プレイヤーリスポーン地点の設定
		player_respawn = { (float)100,(float)100 };
		//プレイヤーの生成
		CreateObject(new Player, player_respawn, { PLAYER_HEIGHT,PLAYER_WIDTH }, RED);
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

bool GameMain::CheckInScreen(Object* _object)const
{
	//画面内に居るか判断
	if (_object->GetLocation().x > camera_location.x - _object->GetErea().width - 150 &&
		_object->GetLocation().x < camera_location.x + SCREEN_WIDTH + _object->GetErea().width + 150 &&
		_object->GetLocation().y > camera_location.y - _object->GetErea().height - 150 &&
		_object->GetLocation().y < camera_location.y + SCREEN_HEIGHT + _object->GetErea().height + 150
		)
	{
		return true;
	}
	return false;
}

Location GameMain::GetBossLocation()
{
	return object[boss_object]->GetLocation();
}

void GameMain::DeleteAllObject(bool _player_delete)
{
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		//プレイヤー以外削除
		if (i != player_object)
		{
			object[i] = nullptr;
		}
	}
	if (_player_delete)
	{
		object[player_object] = nullptr;
		player_flg = false;
	}
}

void GameMain::PlayerUpdate()
{
	//プレイヤーが居ないなら(DeleteObjectされていた、もしくはobject[player_object]がプレイヤーではないなら)
	if (object[player_object] == nullptr || object[player_object]->GetObjectType() != PLAYER)
	{
		//プレイヤーの生成
		CreateObject(new Player, player_respawn, { PLAYER_HEIGHT,PLAYER_WIDTH }, GREEN);
	}

	//プレイヤーの更新＆色探知用
	object[player_object]->SetScreenPosition(camera_location);
	if (object[player_object] != nullptr)
	{
		object[player_object]->Update(this);
		move_object_num++;
	}
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

	//プレイヤーが落下したときに死亡判定とする
	if (GetPlayerLocation().y > stage_height+100)
	{
		//ステージとプレイヤーをリセット
		SetStage(now_stage, true);
	}
}

void GameMain::BossUpdate()
{
	if (object[boss_object] != nullptr)
	{
		object[boss_object]->Update(this);
		move_object_num++;
	}
	for (int i = 0; i < attack_num; i++)
	{
		if (object[boss_attack[i]] != nullptr)
		{
			object[boss_attack[i]]->Update(this);
			move_object_num++;
		}
	}
}
