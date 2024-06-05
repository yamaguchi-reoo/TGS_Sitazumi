#include "Help.h"
#include "DxLib.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

Help::Help()
{
}

Help::~Help()
{
}

void Help::Initialize()
{

}

void Help::Finalize()
{

}

AbstractScene* Help::Update()
{
	return this;
}

void Help::Draw()const
{
	DrawString(0, 10, "Help", 0x00ff00);
}
