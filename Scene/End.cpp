#include "End.h"
#include "DxLib.h"


End::End():shift_y(0)
{
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
			logo_location[i].x += 90;
		}
	}
}

void End::Finalize()
{

}

AbstractScene* End::Update()
{
	/*if (++ExitNum > 179)
	{
		return nullptr;
	}*/

	return this;
}

void End::Draw()const
{
	SetFontSize(90);
	DrawString(0, 10, "End", 0x00ff00);
	SetFontSize(36);
	DrawString((SCREEN_WIDTH / 2) - 85, 200, "TITLE", 0xffffff);
	for (int i = 0; i < 7; i++)
	{
		DrawFormatStringF((SCREEN_WIDTH / 2) - 85, 200, logo_color[i], "%s", logo_string[i]);
	}
	//DrawString((SCREEN_WIDTH / 2) - 100, 200, "制作メンバー", 0xffffff);
}