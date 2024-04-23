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

GameMain::GameMain(int _stage) :stage_data{0},now_stage(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false),player_object(0)
{
	swap_anim[0].move_flg = false;
	swap_anim[1].move_flg = false;
	SetStage(_stage);
	lock_pos = camera_location;
	swap_anim_timer = 0;
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
		object[i]->Update(this);
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
				if (/*object[j]->GetObjectType() == BLOCK && */object[j]->GetCanSwap() == TRUE) {
					object[i]->SearchColor(object[j]);
				}
			}
		}
	}
	//色交換エフェクトの更新
	for (int i = 0; i < 2; i++)
	{
		if (swap_anim[i].move_flg == true)
		{
			swap_anim[i].location.x += 20 * cosf(swap_anim[i].move_rad);
			swap_anim[i].location.y += 20 * sinf(swap_anim[i].move_rad);
		}
		if (--swap_anim[i].timer < SWAP_EFFECT_STOP_TIME)
		{
			swap_anim[i].move_flg = false;
		}
		if (swap_anim[i].timer < SWAP_EFFECT_STOP_TIME && swap_anim[i].timer > 0)
		{
			swap_anim_timer++;
		}
		else
		{
			swap_anim_timer = 0;
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
	//色交換エフェクトの描画
	for (int i = 0; i < 2; i++)
	{
		if (swap_anim[i].move_flg == true)
		{
			DrawBox(swap_anim[i].start.x - camera_location.x, swap_anim[i].start.y - camera_location.y, swap_anim[i].start.x + swap_anim[i].erea.width - camera_location.x, swap_anim[i].start.y + swap_anim[i].erea.height- camera_location.y, 0x000000, true);
			DrawBox(swap_anim[i].start.x - camera_location.x, swap_anim[i].start.y - camera_location.y, swap_anim[i].start.x + swap_anim[i].erea.width - camera_location.x, swap_anim[i].start.y + swap_anim[i].erea.height- camera_location.y, 0xffffff, false);
			DrawBox(swap_anim[i].location.x - camera_location.x, swap_anim[i].location.y - camera_location.y, swap_anim[i].location.x + 40 - camera_location.x, swap_anim[i].location.y + 40 - camera_location.y, swap_anim[i].color, true);
		}
		if (swap_anim_timer > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - (swap_anim_timer*20));
			DrawBox(swap_anim[i].start.x - camera_location.x - (swap_anim_timer*5), swap_anim[i].start.y - camera_location.y - (swap_anim_timer * 5), swap_anim[i].start.x + swap_anim[i].erea.width - camera_location.x + (swap_anim_timer * 5), swap_anim[i].start.y + swap_anim[i].erea.height - camera_location.y + (swap_anim_timer * 5), swap_anim[i].color, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); 
		}
	}
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
	bool player_flg = false;	//プレイヤーを生成したか
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
				player_flg = true;
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

int GameMain::Swap(Object* _object1, Object* _object2)
{
	swap_anim[0].location = _object1->GetLocation();
	swap_anim[1].location = _object2->GetLocation();
	swap_anim[0].start = _object1->GetLocation();
	swap_anim[1].start = _object2->GetLocation();
	swap_anim[0].erea = _object1->GetErea();
	swap_anim[1].erea = _object2->GetErea();
	swap_anim[0].move_flg = true;
	swap_anim[1].move_flg = true;
	swap_anim[0].move_rad = atan2f(_object2->GetLocation().y - _object1->GetLocation().y, _object2->GetLocation().x - _object1->GetLocation().x);
	swap_anim[1].move_rad = atan2f(_object1->GetLocation().y - _object2->GetLocation().y, _object1->GetLocation().x - _object2->GetLocation().x);
	swap_anim[0].color = _object1->GetColerData();
	swap_anim[1].color = _object2->GetColerData();

	//移動にかかる時間測定
	Location move;
	float g;
	move.x = fabsf(_object1->GetCenterLocation().x - _object2->GetCenterLocation().x);
	move.y = fabsf(_object1->GetCenterLocation().y - _object2->GetCenterLocation().y);
	g = powf(move.x, 2) + powf(move.y, 2);

	swap_anim[0].timer = (int)(sqrtf(g) / SWAP_EFFECT_SPEED * 0.9f) + SWAP_EFFECT_STOP_TIME;
	swap_anim[1].timer = (int)(sqrtf(g) / SWAP_EFFECT_SPEED * 0.9f) + SWAP_EFFECT_STOP_TIME;

	return swap_anim[0].timer;
}
