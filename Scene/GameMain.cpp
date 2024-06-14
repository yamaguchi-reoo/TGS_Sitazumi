#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include "EditScene.h"
#include <fstream>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Title.h"
#include "End.h"
#include "Help.h"


static Location camera_location = { 0,0};	//カメラの座標
static Location screen_origin = { (SCREEN_WIDTH / 2),(SCREEN_HEIGHT / 2) };

GameMain::GameMain(int _stage) :frame(0), stage_data{ 0 }, now_stage(0), object_num(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false), player_object(0), boss_object(0), weather(0), weather_timer(0), move_object_num(0), player_flg(false), player_respawn_flg(false), fadein_flg(true), create_once(false), game_over_flg(false), game_clear_flg(false), game_pause_flg(false), pause_after_flg(false), cursor(0)
{
	now_stage = _stage;
}

GameMain::GameMain(int _stage, Location _respawn_locatoin, Player p) :frame(0), stage_data{ 0 }, now_stage(0), object_num(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false), player_object(0), boss_object(0), weather(0), weather_timer(0), move_object_num(0), player_flg(false), player_respawn_flg(false), fadein_flg(true), create_once(false), game_over_flg(false), game_clear_flg(false), game_pause_flg(false), cursor(0)
{
	//now_stage = _stage;
	//player_respawn = _respawn_locatoin;

	//Object* play = dynamic_cast<Object*>(&p);
	//CreateObject(play, player_respawn, { PLAYER_HEIGHT,PLAYER_WIDTH }, play->GetColorData());
	//player_flg = true;
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

	back_ground = new BackGround();

	SetStage(now_stage, false);

	back_ground->Initialize({ (float)stage_width,(float)stage_height });

	lock_pos = camera_location;

	test = new BossAttackWater();
	test->Initialize({ 10000,1600 }, { 40,40 }, RED, 1000);

	bgm_normal = ResourceManager::SetSound("Resource/Sounds/BGM/GameMainNormal.wav");
	bgm_noise = ResourceManager::SetSound("Resource/Sounds/BGM/GameMainNoise.wav");
	bgm_abnormal = ResourceManager::SetSound("Resource/Sounds/BGM/GameMainAbnormal.wav");

	ResourceManager::StartSound(bgm_normal, TRUE);
}

void GameMain::Finalize()
{
	//BGMを止める
	ResourceManager::StopSound(bgm_normal);
	ResourceManager::StopSound(bgm_noise);
	ResourceManager::StopSound(bgm_abnormal);

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

	test->Finalize();
	delete test;
}

AbstractScene* GameMain::Update()
{
	//フレーム測定
	frame++;
	//カメラの更新
	UpdateCamera();

	if (frame % 10 == 0)
	{
		Gbutton_draw = !Gbutton_draw;
	}
	// チュートリアルエリア：左スティック
	Gstick_angle += 0.05f;
	if (Gstick_angle > 1)
	{
		Gstick_angle = 0;
	}

	Gdraw_stick_shift.x = cosf(Gstick_angle * M_PI * 2) * 5;
	Gdraw_stick_shift.y = sinf(Gstick_angle * M_PI * 2) * 5;

	//演出の終了
	if ((frame > FADEIN_TIME || PadInput::OnButton(XINPUT_BUTTON_B)) && fadein_flg == true)
	{
		fadein_flg = false;
	}
	//演出中は更新を止める
	if (frame == 1 || fadein_flg == false)
	{
			if (game_clear_flg)
		{
			if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
			{
				cursor = 0;
			}
			else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
			{
				cursor = 1;
			}

			if (PadInput::OnButton(XINPUT_BUTTON_B)) {
				if (cursor == 0)
				{
					return new Title();
				}
				else
				{
					return new End();
				}
			}


			if (circleAng++ >= 360.f) {
				circleAng = 0.f;
			}
		}
		//ゲームオーバー
		else if (game_over_flg)
		{
			if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
			{
				cursor = 0;
			}
			else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
			{
				cursor = 1;
			}

			if (PadInput::OnButton(XINPUT_BUTTON_B)) {
				if (cursor == 0)
				{
					SetStage(now_stage, true);
					game_over_flg = false;
				}
				else
				{
					return new Title();
				}
			}


			if (circleAng++ >= 360.f) {
				circleAng = 0.f;
			}
		}
		else if (game_pause_flg)
		{
			if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
			{
				cursor = 0;
			}
			else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
			{
				cursor = 1;
			}

			if (PadInput::OnButton(XINPUT_BUTTON_B) || PadInput::OnButton(XINPUT_BUTTON_START)) {
				if (cursor == 0)
				{
					game_pause_flg = false;
					pause_after_flg = true;
				}
				else
				{
					return new Title();
				}
			}


			if (circleAng++ >= 360.f) {
				circleAng = 0.f;
			}
		}
		else
		{
		if (PadInput::OnButton(XINPUT_BUTTON_START) && !game_pause_flg)
			{
				game_pause_flg = true;
			}

		if (PadInput::OnRelease(XINPUT_BUTTON_B) && pause_after_flg)
			{
				pause_after_flg = false;
			}
			//リセット
			move_object_num = 0;
			//各オブジェクトの更新
			if (object[player_object]->GetSearchFlg() == FALSE || (object[player_object]->GetSearchFlg() == TRUE && frame % 10 == 0))
			{
				for (int i = 0; i < OBJECT_NUM; i++)
				{
					//プレイヤーとボス以外の画面内オブジェクトの更新
					if (i != player_object && CheckInScreen(object[i]))
					{
						object[i]->SetScreenPosition(camera_location);
						object[i]->Update(this);
						move_object_num++;
						for (int j = i + 1; object[j] != nullptr; j++)
						{
							//各オブジェクトとの当たり判定
							if (object[i] != nullptr && CheckInScreen(object[j]) == true && object[i]->HitBox(object[j]) && j != player_object)
							{
								object[i]->Hit(object[j]);
								object[j]->Hit(object[i]);
							}
						}
						//プレイヤーに選択されているオブジェクトなら、描画色を変える
						if (object[i] != nullptr && object[i] == now_current_object && now_current_object != object[boss_object])
						{
							object[i]->SetDrawColor(WHITE);
						}
						else if (object[i] != nullptr)
						{
							object[i]->SetDrawColor(object[i]->GetColorData());
						}
					}
				}
				//管理クラスの更新
				weather->Update(this);
			}

			//プレイヤーの更新
			PlayerUpdate();

			//ボスの更新
			BossUpdate();

			//管理クラスの更新
			effect_spawner->Update(this);

			////背景の更新
			//back_ground->Update();

			//プレイヤーがボスエリアに入ったら退路を閉じる
			if (now_stage == 2 && object[player_object]->GetLocalLocation().x > 160 && object[player_object]->GetLocalLocation().x < 200 && create_once == false)
			{
				CreateObject(new Stage(2), { 160,520 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(2), { 160,560 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(2), { 160,600 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(2), { 160,640 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(1), { 120,520 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(1), { 120,560 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(1), { 120,600 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				CreateObject(new Stage(1), { 120,640 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
				create_once = true;
			}

			//ボスステージに遷移
			if (now_stage != 2 && object[player_object]->GetLocation().x > stage_width - 100 && object[player_object]->GetLocation().y > stage_height - 300)
			{
				SetStage(2, false);
			}
		}


#ifdef _DEBUG
		//ステージをいじるシーンへ遷移
		if (KeyInput::OnPresed(KEY_INPUT_E) && KeyInput::OnPresed(KEY_INPUT_D))
		{
			return new EditScene(now_stage);
		}

		if (KeyInput::OnKey(KEY_INPUT_1))
		{
			ResourceManager::StopSound(0);
		}
		if (KeyInput::OnKey(KEY_INPUT_2))
		{
			SetStage(2, false);
		}


#endif

	}
		return this;
}

void GameMain::Draw() const
{
	
	SetFontSize(12);
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
	//for (int i = 0; i < attack_num; i++)
	//{
	//	object[boss_attack[i]]->Draw();
	//}
	
	//エフェクトの描画
	effect_spawner->Draw();

	//天気管理クラスの描画
	weather->Draw();

	//フェードイン演出
	/*if (fadein_flg == true)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (frame*(255/FADEIN_TIME) +3));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}*/

	if (game_clear_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
		DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);
		DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
		DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

		int fontsize = 192 / 2;
		SetFontSize(fontsize * 2);
		DrawString(200, 100, "G", 0xff0000);
		DrawString(200 + fontsize * 1, 100, "A", 0xffffff);
		DrawString(200 + fontsize * 2, 100, "M", 0xffffff);
		DrawString(200 + fontsize * 3, 100, "E", 0xffffff);
		DrawString(200 + fontsize * 4, 100, "C", 0x00ff00);
		DrawString(200 + fontsize * 5, 100, "L", 0xffffff);
		DrawString(200 + fontsize * 6, 100, "E", 0xffffff);
		DrawString(200 + fontsize * 7, 100, "A", 0xffffff);
		DrawString(200 + fontsize * 8, 100, "R", 0x0000ff);

		SetFontSize(48);
		DrawString(285, 436, "TITLE", 0xffffff);
		DrawString(890, 436, "END", 0xffffff);

		Location circleLoc;

		if (cursor == 0) {
			circleLoc.x = 350.f;
			circleLoc.y = 460.f;
		}
		else {
			circleLoc.x = 930.f;
			circleLoc.y = 460.f;
		}

		DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

		Location base;
		base.x = circleLoc.x;
		base.y = circleLoc.y;

		Location l[3];
		l[0].x = base.x;
		l[0].y = base.y - 40.f * 2.3f;

		l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

		l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

		l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


		DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
		DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
		DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);
	}

	//ゲームオーバー
	if (game_over_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000,true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		
		DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
		DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);
		DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
		DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

		int fontsize = 192 / 2;
		SetFontSize(fontsize * 2);
		DrawString(240, 100, "G", 0xff0000);
		DrawString(240 + fontsize * 1, 100, "A", 0xffffff);
		DrawString(240 + fontsize * 2, 100, "M", 0xffffff);
		DrawString(240 + fontsize * 3, 100, "E", 0xffffff);
		DrawString(240 + fontsize * 4, 100, "O", 0x00ff00);
		DrawString(240 + fontsize * 5, 100, "V", 0xffffff);
		DrawString(240 + fontsize * 6, 100, "E", 0xffffff);
		DrawString(240 + fontsize * 7, 100, "R", 0x0000ff);

		SetFontSize(48);
		DrawString(260, 436, "RESTART", 0xffffff);
		DrawString(860, 436, "TITLE", 0xffffff);

		Location circleLoc;

		if (cursor == 0) {
			circleLoc.x = 350.f;
			circleLoc.y = 460.f;
		}
		else {
			circleLoc.x = 930.f;
			circleLoc.y = 460.f;
		}

		DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

		Location base;
		base.x = circleLoc.x;
		base.y = circleLoc.y;

		Location l[3];
		l[0].x = base.x;
		l[0].y = base.y - 40.f * 2.3f;

		l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

		l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

		l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


		DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
		DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
		DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);
	}

	if (game_pause_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
		DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);
		DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
		DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

		int fontsize = 192 / 2;
		SetFontSize(fontsize * 2);
		DrawString(400, 100, "P", 0xff0000);
		DrawString(400 + fontsize * 1, 100, "A", 0xffffff);
		DrawString(400 + fontsize * 2, 100, "U", 0x00ff00);
		DrawString(400 + fontsize * 3, 100, "S", 0xffffff);
		DrawString(400 + fontsize * 4, 100, "E", 0x0000ff);

		SetFontSize(48);
		DrawString(300, 436, "BACK", 0xffffff);
		DrawString(860, 436, "TITLE", 0xffffff);

		Location circleLoc;

		if (cursor == 0) {
			circleLoc.x = 350.f;
			circleLoc.y = 460.f;
		}
		else {
			circleLoc.x = 930.f;
			circleLoc.y = 460.f;
		}

		DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

		Location base;
		base.x = circleLoc.x;
		base.y = circleLoc.y;

		Location l[3];
		l[0].x = base.x;
		l[0].y = base.y - 40.f * 2.3f;

		l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

		l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

		l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


		DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
		DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
		DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);
	}
	
#ifdef _DEBUG
	DrawFormatString(100, 100, 0xffffff, "Object数:%d", object_num);
	DrawFormatString(100, 120, 0xffffff, "Updeteが呼ばれているObject数:%d", move_object_num);

	test->Draw();
	//チュートリアル表示テスト
	SetFontSize(50);
	//DrawString(KeyInput::GetMouseCursor().x - camera_location.x, stage_height- KeyInput::GetMouseCursor().y - camera_location.y, "aaaaa", 0xff0000, TRUE);
	//DrawFormatString(0, 40, 0xff0000, "%0.1f x %0.1f y", camera_location.x, stage_height - camera_location.y);
	DrawFormatString(0, 60, 0xff0000, "%0.1f x %0.1f y",  camera_location.x + KeyInput::GetMouseCursor().x, stage_height - KeyInput::GetMouseCursor().y - camera_location.y);
	DrawFormatString(0, 100, 0xff0000, "%0.1f x %0.1f y", 1830 - camera_location.x, stage_height - 646 - camera_location.y);
#endif

	// 1742 - camera_location.x, stage_height - 560 - camera_location.y
	// 2023 - camera_location.x, stage_height - 357 - camera_location.y

	// チュートリアルテキストはボスエリアで描画しない
	if (now_stage != 2)
	{
		SetFontSize(50);
		DrawBoxAA(515 - camera_location.x, stage_height - 546 - camera_location.y, 903 - camera_location.x, stage_height - 317 - camera_location.y, 0xffffff, FALSE, 3.0f);
		DrawBoxAA(518 - camera_location.x, stage_height - 543 - camera_location.y, 900 - camera_location.x, stage_height - 320 - camera_location.y, 0x555555, TRUE, 3.0f);

		// 左スティック：移動の説明
		DrawCircleAA(580 - camera_location.x, stage_height - 480 - camera_location.y, 25, 100, 0x000000, TRUE);
		DrawCircleAA(580 - camera_location.x, stage_height - 480 - camera_location.y, 25, 100, 0x666666, FALSE, 3.0f);
		DrawCircleAA(580 - camera_location.x + Gdraw_stick_shift.x, stage_height - 480 - camera_location.y + Gdraw_stick_shift.y, 18, 100, 0x666666, TRUE);
		DrawStringF(610 - camera_location.x, stage_height - 500 - camera_location.y, "：移動", 0xffffff);

		if (Gbutton_draw == false)
		{
			//ボタンイメージ描画
			DrawCircleAA(577 - camera_location.x, stage_height - 460 - camera_location.y + 60, 25, 100, 0xff0000, FALSE);
			DrawStringF(577 - camera_location.x - 12, stage_height - 460 - camera_location.y + 38, "A", 0xff0000);
		}
		else
		{
			DrawCircleAA(577 - camera_location.x, stage_height - 460 - camera_location.y + 55, 25, 100, 0xff0000, TRUE);
			DrawCircleAA(577 - camera_location.x, stage_height - 460 - camera_location.y + 60, 25, 100, 0xff0000, TRUE);
			DrawStringF(577 - camera_location.x - 12, stage_height - 460 - camera_location.y + 33, "A", 0x000000);
		}
		DrawStringF(610 - camera_location.x, stage_height - 428 - camera_location.y, "：ジャンプ", 0xffffff);

		DrawBoxAA(1742 - camera_location.x, stage_height - 700 - camera_location.y, 2073 - camera_location.x, stage_height - 397 - camera_location.y, 0xffffff, FALSE, 3.0f);
		DrawBoxAA(1745 - camera_location.x, stage_height - 697 - camera_location.y, 2070 - camera_location.x, stage_height - 400 - camera_location.y, 0x555555, TRUE, 3.0f);
		DrawPlayer();
	}
	

	//プレイヤーを最後に描画
	object[player_object]->Draw();
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
			//if (_erea.height == 1200.f)
			//{
			//	boss_attack[attack_num++] = i;
			//}
			object_num++;
			break;
		}
	}
}

void GameMain::DeleteObject(int i, Object* _object)
{
	if (object[i] != nullptr && _object != nullptr && i >= 0)
	{
		int a;
		a = 0;



		//プレイヤーが消されたなら
		if (i == player_object)
		{
			//player_objectをリセット
			player_object = 0;
		}
		//ボスが消されたなら
		if (i == boss_object)
		{
			//boss_objectをリセット
			boss_object = 0;
		}
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
			if (object[player_object]->GetCenterLocation().x <= (SCREEN_WIDTH / 2))
			{
				lock_pos.x = (SCREEN_WIDTH / 2);
			}
			if (object[player_object]->GetCenterLocation().x >= stage_width - (SCREEN_WIDTH / 2))
			{
				lock_pos.x = stage_width - (SCREEN_WIDTH / 2);
			}
			/*lock_pos.x = object[player_object]->GetCenterLocation().x;*/
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
			if (object[player_object]->GetCenterLocation().y >= stage_height - (SCREEN_HEIGHT / 2) - 10)
			{
				lock_pos.y = stage_height - ((float)SCREEN_HEIGHT / 2) - 10;
			}
			if (object[player_object]->GetCenterLocation().y <= (SCREEN_HEIGHT / 2))
			{
				lock_pos.y = (SCREEN_HEIGHT / 2);
			}
			//lock_pos.y = object[player_object]->GetCenterLocation().y;
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
			camera_location = { 160,40};
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

	//背景クラスのステージも変更
	back_ground->SetNowStage(now_stage);

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
					//プレイヤーリスポーン地点の設定
					//player_respawn = { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT };
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
				effect_spawner->SpawnEffect({ player_respawn.x + PLAYER_WIDTH / 2 ,player_respawn.y + PLAYER_HEIGHT / 2 }, { 20,20 }, PlayerSpawnEffect, 30, object[player_object]->GetColorData());
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
	//壁生成フラグリセット
	create_once = false;
	//カメラのリセット
	ResetCamera();
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
	if (_object != nullptr &&
		(
			(_object->GetObjectType() != ENEMY && 
			 _object->GetLocation().x > camera_location.x - _object->GetErea().width - 80 &&
		     _object->GetLocation().x < camera_location.x + SCREEN_WIDTH + _object->GetErea().width + 80 &&
		     _object->GetLocation().y > camera_location.y - _object->GetErea().height - 80 &&
		     _object->GetLocation().y < camera_location.y + SCREEN_HEIGHT + _object->GetErea().height + 80
		    )
			||
		    (_object->GetObjectType()==ENEMY &&
		     _object->GetLocation().x > camera_location.x - _object->GetErea().width&&
		     _object->GetLocation().x < camera_location.x + SCREEN_WIDTH + _object->GetErea().width&&
		     _object->GetLocation().y > camera_location.y - _object->GetErea().height&&
		     _object->GetLocation().y < camera_location.y + SCREEN_HEIGHT + _object->GetErea().height
		     ) 
		)
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
	if (object[player_object] != nullptr)
	{
		object[player_object]->SetScreenPosition(camera_location);
		object[player_object]->Update(this);
		move_object_num++;

		for (int i = 0; object[i] != nullptr; i++)
		{
			if (object[i]->GetObjectType() != PLAYER && object[i]->GetCanSwap() == TRUE) {
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
		if (GetPlayerLocation().y > stage_height + 100)
		{
			//ステージとプレイヤーをリセット
			//SetStage(now_stage, true);

			game_over_flg = true;
		}
	}
}

void GameMain::BossUpdate()
{
	//ボスをスローモーションにしないならコメント解除
	//if (object[boss_object] != nullptr)
	//{
	//	object[boss_object]->SetScreenPosition(camera_location);
	//	object[boss_object]->Update(this);
	//	move_object_num++;
	//	for (int i = 0; object[i] != nullptr; i++)
	//	{
	//		if (object[i]->GetCanSwap() == TRUE && object[i]->GetObjectType() != PLAYER) {
	//			object[player_object]->SearchColor(object[i]);
	//		}
	//		//各オブジェクトとの当たり判定
	//		if (object[i]->HitBox(object[boss_object]))
	//		{
	//			object[i]->Hit(object[boss_object]);
	//			object[boss_object]->Hit(object[i]);
	//		}
	//	}
	//}

	//for (int i = 0; i < attack_num; i++)
	//{
	//	if (object[boss_attack[i]] != nullptr)
	//	{
	//		object[boss_attack[i]]->Update(this);
	//		move_object_num++;
	//	}
	//}
}

void GameMain::SetNowCurrentObject(Object* _object)
{
	now_current_object = _object;
}

void GameMain::DrawPlayer()const
{
	// 1830 - camera_location.x, stage_height - 646 - camera_location.y 

	//帽子　中央
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, draw_color, true);
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, false);
	////帽子　右側
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, draw_color, true);
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, false);
	////帽子　左側
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, draw_color, true);
	//DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, false);

	//頭
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y, 23, 15, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y, 23, 15, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, false);

	//目
	ResourceManager::DrawRotaBox(1830 - camera_location.x - 6, stage_height - 646 - camera_location.y, 6, 7, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, true);

	//首
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y - 14, 10, 5, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y - 14, 10, 5, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, false);

	//胴体																					
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y - 40, 21, 37, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y - 40, 21, 37, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, false);

	//バッグ
	ResourceManager::DrawRotaBox(1830 - camera_location.x + 15, stage_height - 646 - camera_location.y - 40, 5, 23, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(1830 - camera_location.x + 15, stage_height - 646 - camera_location.y - 40, 5, 23, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, false);
	ResourceManager::DrawRotaBox(1830 - camera_location.x + 15, stage_height - 646 - camera_location.y - 40, 3, 15, 1830 - camera_location.x, stage_height - 646 - camera_location.y, 0, 0x000000, true);

	//腕
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y, 28, 7, 1830 - camera_location.x, stage_height - 616 - camera_location.y - 15, -60.0f, GREEN, true);
	ResourceManager::DrawRotaBox(1830 - camera_location.x, stage_height - 646 - camera_location.y, 28, 7, 1830 - camera_location.x, stage_height - 616 - camera_location.y - 15, -60.0f, 0x000000, false);

	//足												 														
	//ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, GREEN, true);
	//ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, 0x000000, false);
}

Location GameMain::RotationLocation(Location BaseLoc, Location Loc, float r) const
{
	Location l;
	l.x = Loc.x - BaseLoc.x;
	l.y = Loc.y - BaseLoc.y;

	Location ret;
	ret.x = l.x * cosf(r) - l.y * sinf(r);
	ret.y = l.x * sinf(r) + l.y * cosf(r);

	ret.x += BaseLoc.x;
	ret.y += BaseLoc.y;

	return ret;
}
