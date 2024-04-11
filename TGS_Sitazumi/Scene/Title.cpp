#include "Title.h"
#include "DxLib.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

Title::Title()
{
}

Title::~Title()
{
}

AbstractScene* Title::Update()
{
	return this;
}

void Title::Draw()const
{
	DrawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Title", 0x00ff00);
}
