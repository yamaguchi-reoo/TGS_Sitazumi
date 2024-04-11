#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include <fstream>
#include <iostream>
#include <string>

static Location camera_location = { 0,0};	//カメラの座標
static Location screen_origin = { (SCREEN_WIDTH / 2),0 };

GameMain::GameMain(int _stage)
{

}

GameMain::~GameMain()
{
}

AbstractScene* GameMain::Update()
{
	return this;
}

void GameMain::Draw() const
{
}

