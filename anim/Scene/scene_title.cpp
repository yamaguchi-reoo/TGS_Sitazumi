#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "scene_title.h"
#include "DxLib.h"
#include "../Utility/inputCtrl.h"
#include "../Utility/calculation.h"


//�R���X�g���N�^
Title::Title()
{
	for (int i = 0; i < 32; i++)
	{
		location[i] = { 0,0 };
		string[i] = 0;
	}
	
	inputChar = 0;
}

Title::~Title()
{
	
}

//�X�V
Scene* Title::update()
{
	if (InputCtrl::GetMouseState(L) == PRESS) {
		location[num] = {(float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
		num++;
	}

	if (num > 1) {
		location[1] = GetRotaLocation(location[1], location[0], angle);
		angle = 0.1f;
	}

	//strcpy_s(string, 32, "a");

	//文字入力
	inputChar = GetInputChar(TRUE);
	if (inputChar == 0x08 && strlen(string) > 0) { //bacaspace
		string[strlen(string) - 1] = '\0';
	}
	else if (inputChar != 0 && strlen(string) < 32 && inputChar != 0x08) {
		strncat(string, &inputChar, sizeof(inputChar));
	}

	return this;
}

//�`��
void Title::draw() const
{	
	DrawString(0, 0, "title", 0xffffff);

	for (int i = 0; i < num; i += 2)
	{
		if (location[i].x != 0 && location[i].y != 0) 
		{
			DrawLine(location[i].x, location[i].y, location[i + 1].x, location[i + 1].y, 0xffffff, 1);
		}
	}

	//if (string != nullptr) {
		DrawFormatString(0, 40, 0xffffff, "%s", string);
	//}

}

