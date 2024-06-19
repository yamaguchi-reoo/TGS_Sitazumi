#include "End.h"
#include "DxLib.h"


End::End() :shift_y(0), stop_time(0), up(0), deer_speed(2), bat_loction{ 0.0f,0.0f }, deer_location{ 0.0f,0.0f }
{
	for (int i = 0; i < 7; i++)
	{
		logo_location[i].x = 0;
	}

	for (int i = 0; i < 4; i++) {
		leg_angle[i] = 0.0f;
	}
}

End::~End()
{
}

void End::Initialize()
{
	for (int i = 0; i < 7; i++)
	{
		logo_location[i] = { 0,0 };
		for (int j = 0; j < i; j++)
		{
			logo_location[i].x += 40;
		}
	}

	bat_loction = { 100.f, 200.f };
	deer_location = { 900.f, 500.f };

	shift_y = -600;
}

void End::Finalize()
{

}

AbstractScene* End::Update()
{
	up++;
	//タイトルロゴが止まるように
	if (shift_y > -80 || shift_y > 2400) {
		stop_time++;
	}
	else {
		shift_y += 5;
	}

	//タイトルロゴが止まってから一定時間たったら動くように
	if (stop_time > 120 && shift_y < 2400) {
		shift_y += 5;
	}

	//thank you for playingが止まるように
	if (shift_y >= 2400) {
		ExitNum++;
	}
	//終了処理
	if (ExitNum > 179)
	{
		return nullptr;
	}

	DeerUpdate();
	BatUpdate();

	return this;
}

void End::Draw()const
{
	//DrawFormatString(900, 20, 0xffffff, "shift_y = %d", shift_y);
	/*SetFontSize(90);
	DrawString(0, 10, "End", 0x00ff00);*/

	//エンドロール
	SetFontSize(60);
	//DrawString((SCREEN_WIDTH / 2) - 105, 200 - shift_y, "TITLE", 0xffffff);
	DrawString(435, 240 - shift_y, "「", 0xffffff);
	DrawString(780, 255 - shift_y, "」", 0xffffff);
	for (int i = 0; i < 7; i++)
	{
		DrawFormatStringF(logo_location[i].x + 500, logo_location[i].y + 250 - shift_y, logo_color[i], "%s", logo_string[i]);
	}

	DrawString(340, 2400 + 300 - shift_y, "Thank     for playing", 0xffffff);
	DrawString(530, 2400 + 300 - shift_y, "y", 0xff0000);
	DrawString(560, 2400 + 300 - shift_y, "o", 0x0000ff);
	DrawString(590, 2400 + 300 - shift_y, "u", 0x00ff00);


	SetFontSize(30);
	DrawString(500, 500 + 300 - shift_y, "Production Members", 0xff0000);
	DrawString(530, 600 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 640 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 680 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 720 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);

	DrawString(505, 1020 + 300 - shift_y, "Site of Music Used", 0x0000ff);
	DrawString(535, 1120 + 300 - shift_y, "「無料効果音」", 0xffffff);
	DrawString(535, 1170 + 300 - shift_y, "「効果音ラボ」", 0xffffff);
	DrawString(515, 1220 + 300 - shift_y, "「アルスパーク」", 0xffffff);

	DrawString(550, 1380 + 300 - shift_y, "Title Music", 0x00ff00);
	DrawString(450, 1460 + 300 - shift_y, "sound jewel 「Good Night」", 0xffffff);

	DrawString(520, 1630 + 300 - shift_y, "GameMain Music", 0x00ff00);
	DrawString(400, 1710 + 300 - shift_y, "Dynamedion 「A Chill in the air」", 0xffffff);

	//コウモリ
	BatDraw();
	//シカ
	DeerDraw();

}

void End::DeerDraw() const
{

	//頭
	ResourceManager::DrawRotaBox(deer_location.x + 16.0f, deer_location.y + 10.0f, 30.0f, 20.0f, deer_location.x + 16.0f, deer_location.y + 10.0f, 0, BLUE, true);
	//目
	ResourceManager::DrawRotaBox(deer_location.x + 8.0f, deer_location.y + 10.0f, 8.0f, 9.0f, deer_location.x + 8.0f, deer_location.y + 10.0f, 0, 0x000000, true);

	ResourceManager::DrawRotaBox(deer_location.x + 25.0f, deer_location.y + 38.0f, 13.0f, 24.0f, deer_location.x + 25.0f, deer_location.y + 38.0f, 0.f, BLUE, true);

	//胴体 vr.1
	ResourceManager::DrawRotaBox(deer_location.x + 53.0f, deer_location.y + 63.0f, 65.0f, 15.0f, deer_location.x + 53.0f, deer_location.y + 63.0f, 0.f, BLUE, true);

	//首 vr.2
	ResourceManager::DrawRotaBox(deer_location.x + 25.0f, deer_location.y + 38.0f, 13.0f, 24.0f, deer_location.x + 25.0f, deer_location.y + 38.0f, 0.f, BLUE, true);

	//胴体 vr.2
	ResourceManager::DrawRotaBox(deer_location.x + 34.0f, deer_location.y + 63.0f, 30.0f, 15.0f, deer_location.x + 34.0f, deer_location.y + 63.0f, 0.f, BLUE, true);
	ResourceManager::DrawRotaBox(deer_location.x + 72.0f, deer_location.y + 63.0f, 30.0f, 15.0f, deer_location.x + 72.0f, deer_location.y + 63.0f, 0.f, BLUE, true);

	//足　左から
	ResourceManager::DrawRotaBox(deer_location.x + 27.0f , deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 27.0f, deer_location.y + 88.0f, leg_angle[0], BLUE, true);
	ResourceManager::DrawRotaBox(deer_location.x + 43.0f, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 43.0f, deer_location.y + 88.0f, -leg_angle[1], BLUE, true);
	//ResourceManager::DrawRotaBox(deer_location.x + 35.0f + d_left_leg[1], deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 35.0f, deer_location.y + 88.0f, -leg_angle[1], draw_color, true);
	ResourceManager::DrawRotaBox(deer_location.x + 68.0f, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 68.0f, deer_location.y + 88.0f, leg_angle[2], BLUE, true);
	ResourceManager::DrawRotaBox(deer_location.x + 83.0f, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 83.0f, deer_location.y + 88.0f, -leg_angle[3], BLUE, true);
	//ResourceManager::DrawRotaBox(local_location.x + 75.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 75.0f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);
}

void End::DeerUpdate()
{
	deer_location.x -= 2;
	// 足の回転方向を制御するフラグを追加
	for (int i = 0; i < 4; i++) {
		// 回転方向に基づいて角度を更新
		leg_angle[i] += deer_speed;

		// 角度の範囲を制限する
		if (leg_angle[i] >= 25.0f) {
			leg_angle[i] = 25.0f - 1;
			//回転方向を反転
			deer_speed = -deer_speed;
		}
		if (leg_angle[i] <= -5.0f) {
			leg_angle[i] = -5.0f + 1;
			//回転方向を反転
			deer_speed = -deer_speed;
		}
	}
}

void End::BatDraw() const
{
	float wing_angle = (float)sin(PI * 2.f / 40.f * up) * 20.f; // 30度の振れ幅で周期的に変化させる

	const std::vector<Location> vertices = {
		// 耳
		{bat_loction.x + 46, bat_loction.y}, {bat_loction.x + 46, bat_loction.y + 19}, {bat_loction.x + 55, bat_loction.y + 9},
		{bat_loction.x + 69, bat_loction.y}, {bat_loction.x + 69, bat_loction.y + 19}, {bat_loction.x + 60, bat_loction.y + 9},
		//右翼
		{bat_loction.x + 66, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 117, bat_loction.y + 65},
		{bat_loction.x + 66, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 96, bat_loction.y + 61},
		{bat_loction.x + 65, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 80, bat_loction.y + 57},
		//左翼
		{bat_loction.x + 48, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x - 2, bat_loction.y + 65},
		{bat_loction.x + 48, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x + 18, bat_loction.y + 61},
		{bat_loction.x + 50, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x + 34, bat_loction.y + 57},
		// 頭
		{bat_loction.x + 57, bat_loction.y + 12}, {bat_loction.x + 69, bat_loction.y + 23}, {bat_loction.x + 57, bat_loction.y + 30}, {bat_loction.x + 46, bat_loction.y + 23},
		// 胴体
		{bat_loction.x + 57, bat_loction.y + 34}, {bat_loction.x + 69, bat_loction.y + 52}, {bat_loction.x + 57, bat_loction.y + 73}, {bat_loction.x + 46, bat_loction.y + 52},
	};

	//配列の各頂点を利用して三角形を描画する
	for (int i = 0; i + 2 < vertices.size(); i += 3) {
		//耳
		if (i < 5) {
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, RED, TRUE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, 0x000000, FALSE);
		}
		//右羽
		else if (i >= 6 && i < 14) {
			// 羽の動き
			DrawLineAA(vertices[7].x, vertices[7].y + wing_angle, vertices[8].x + wing_angle, vertices[8].y, 0x000000);
			DrawLineAA(vertices[6].x, vertices[6].y - 2, vertices[7].x, vertices[7].y - 2 + wing_angle, 0x000000);
			//DrawTriangleAA(vertices[i].x - 1, vertices[i].y - 1, vertices[i + 1].x - 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x + wing_angle - 1, vertices[i + 2].y - 1, 0x000000, FALSE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x + wing_angle, vertices[i + 2].y, RED, TRUE);

		}
		//左羽
		else if (i >= 15 && i < 23) {
			// 羽の動き
			DrawLineAA(vertices[16].x, vertices[16].y - 2 + wing_angle, vertices[17].x - wing_angle, vertices[17].y, 0x000000);
			DrawLineAA(vertices[15].x, vertices[15].y - 2, vertices[16].x, vertices[16].y - 2 + wing_angle, 0x000000);
			//DrawTriangleAA(vertices[i].x + 1, vertices[i].y + 1, vertices[i + 1].x + 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x - 3 - wing_angle, vertices[i + 2].y - 3, 0x000000, FALSE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x - wing_angle, vertices[i + 2].y, RED, TRUE);

		}
		//ひし形の描画
		else if (i + 3 < vertices.size())
		{
			DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, RED, TRUE);
			DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, 0x000000, FALSE);
			i++;
		}
	}
}

void End::BatUpdate()
{
	bat_loction.x += 2.f;
	bat_loction.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
}

void End::FrogDraw() const
{
}
