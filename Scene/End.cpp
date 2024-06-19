#include "End.h"
#include "DxLib.h"


End::End():shift_y(0),stop_time(0),stop_flg(false)
{
	for (int i = 0; i < 7; i++)
	{
		logo_location[i].x = 0;
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

	shift_y = -600;
}

void End::Finalize()
{

}

AbstractScene* End::Update()
{
	
	if (shift_y > -80 || shift_y > 2400) {
		stop_time++;
	}
	else {
		shift_y += 5;
	}

	if (stop_time > 120 && shift_y < 2400) {
		shift_y += 5;
	}

	if (shift_y >= 2400) {
		ExitNum++;
	}
	if (ExitNum > 179)
	{
		return nullptr;
	}

	return this;
}

void End::Draw()const
{
	//DrawFormatString(900, 20, 0xffffff, "shift_y = %d", shift_y);
	/*SetFontSize(90);
	DrawString(0, 10, "End", 0x00ff00);*/


	SetFontSize(60);
	//DrawString((SCREEN_WIDTH / 2) - 105, 200 - shift_y, "TITLE", 0xffffff);
	DrawString(435, 240 - shift_y, "「", 0xffffff);
	DrawString(780, 255 - shift_y, "」", 0xffffff);
	for (int i = 0; i < 7; i++)
	{
		DrawFormatStringF(logo_location[i].x + 500, logo_location[i].y + 250 - shift_y, logo_color[i], "%s", logo_string[i]);
	}

	DrawString(340, 2400 + 300 - shift_y, "Thank you for playing", 0xffffff);


	SetFontSize(30);
	DrawString(500, 500 + 300 - shift_y, "Production Members", 0xff0000);
	DrawString(530, 600 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 640 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 680 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
	DrawString(530, 720 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);

	DrawString(505, 1020 + 300 - shift_y, "Site of Music Used", 0x0000ff);
	DrawString(535, 1120 + 300 - shift_y, "「無料効果音」", 0xffffff);
	DrawString(535, 1170 + 300 - shift_y, "「効果音ラボ」", 0xffffff);
	DrawString(535, 1220 + 300 - shift_y, "「アルスパーク」", 0xffffff);

	DrawString(550, 1380 + 300 - shift_y, "Title Music", 0x00ff00);
	DrawString(450, 1460 + 300 - shift_y, "sound jewel 「Good Night」", 0xffffff);

	DrawString(520, 1630 + 300 - shift_y, "GameMain Music", 0x00ff00);
	DrawString(410, 1710 + 300 - shift_y, "Dynamedion 「A Chill in the air」", 0xffffff);



	/*DrawString(410, 2080 + 300 - shift_y, "Dynamedion 「A Chill in the air」", 0xffffff);*/

}