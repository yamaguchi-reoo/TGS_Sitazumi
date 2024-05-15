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

void Title::Initialize()
{

}

void Title::Finalize()
{

}

AbstractScene* Title::Update()
{
	return this;
}

void Title::Draw()const
{
	DrawString(0, 10, "Title", 0x00ff00);
}
