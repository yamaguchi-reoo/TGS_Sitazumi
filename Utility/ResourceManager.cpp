#include "ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

char* ResourceManager::image_filepath[IMAGE_NUM];
char* ResourceManager::div_image_filepath[DIV_IMAGE_NUM];
char* ResourceManager::sound_filepath[SOUND_NUM];
int ResourceManager::image_data[IMAGE_NUM];
int ResourceManager::div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];
int ResourceManager::sound_data[SOUND_NUM];

int ResourceManager::anim;			
FireAnim ResourceManager::fire_anim[ANIM_BLOCK_NUM];
WoodAnim ResourceManager::wood_anim[ANIM_BLOCK_NUM];
WaterAnim ResourceManager::water_anim[ANIM_BLOCK_NUM];

void ResourceManager::DeleteResource()
{
	for (int i = 0; image_data[i] != NULL; i++)
	{
		DeleteGraph(image_data[i]);
	}

	for (int i = 0; div_image_data[i][0] != NULL; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			DeleteGraph(div_image_data[i][j]);
		}
	}

	for (int i = 0; sound_data[i] != NULL; i++)
	{
		DeleteSoundMem(sound_data[i]);
	}
}

void ResourceManager::StageAnimUpdate()
{
	//炎エフェクト用
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		if (fire_anim[i].time < 0)
		{
			fire_anim[i].shift.x = 0;
			fire_anim[i].shift.y = 0;
			fire_anim[i].erea.width = 4;
			fire_anim[i].erea.height = 4;
			fire_anim[i].shift.x += 4 * GetRand(10);
			fire_anim[i].time = 30 + GetRand(30);
			fire_anim[i].angle = GetRand(3);
		}
		else
		{
			fire_anim[i].shift.x += 0.05 + (fire_anim[i].angle - 1) / 2;
			fire_anim[i].shift.y -= 1;
			fire_anim[i].time--;
			fire_anim[i].erea.width -= 0.05;
			fire_anim[i].erea.height -= 0.05;
		}
	}

	//木エフェクト用
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		if (anim < 30)
		{
			wood_anim[i].shift2.x += (wood_anim[i].shift / 50);
		}
		else
		{
			wood_anim[i].shift2.x -= (wood_anim[i].shift / 50);
		}
	}

	//水エフェクト更新
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		if (anim <= 30)
		{
			water_anim[i].shift1.y += water_anim[i].shift;
		}
		else
		{
			water_anim[i].shift1.y -= water_anim[i].shift;
		}
	}
}

int ResourceManager::SetGraph(const char* p)
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		//空の配列に画像を格納する
		if (image_data[i] == NULL)
		{
			image_data[i] = LoadGraph(p);
			image_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ画像が既にあるならその格納場所を返す
		else if (image_filepath[i] == p)
		{
			return i;
		}
	}
}

int ResourceManager::SetDivGraph(const char* p, int AllNum, int XNum, int YNum, int  XSize, int YSize)
{
	for (int i = 0; i < DIV_IMAGE_NUM; i++)
	{
		//空の配列に分割画像を格納する
		if (div_image_data[i][0] == NULL)
		{
			LoadDivGraph(p, AllNum, XNum, YNum, XSize, YSize, div_image_data[i]);
			div_image_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ画像が既にあるならその格納場所を返す
		else if (div_image_filepath[i] == p)
		{
			return i;
		}
	}
}

int ResourceManager::SetSound(const char* p)
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		//空の配列に音源を格納する
		if (sound_data[i] == NULL)
		{
			sound_data[i] = LoadSoundMem(p);
			sound_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ音源が既にあるならその格納場所を返す
		else if (sound_filepath[i] == p)
		{
			return i;
		}
	}
}

int ResourceManager::GetGraph(int _num)
{
	return image_data[_num];
}

int ResourceManager::GetDivGraph(int _num1, int _num2)
{
	return div_image_data[_num1][_num2];
}

void ResourceManager::StartSound(int _num)
{
	PlaySoundMem(sound_data[_num], DX_PLAYTYPE_BACK);
}

void ResourceManager::StopSound(int _num)
{
	StopSoundMem(sound_data[_num]);
}

void ResourceManager::DrawRotaBox(float _x, float _y, float _w, float _h, float _cx, float _cy, float _rad, int _color, bool _fill)
{
	float loc[4][2];
	float newloc[4][2];
	float rad;

	//４点のローカル座標を格納
	loc[0][0] = -(_w / 2) + _cx - _x;
	loc[0][1] =-(_h / 2)  + _cy - _y;
	loc[1][0] =(_w / 2)   + _cx - _x;
	loc[1][1] =-(_h / 2)  + _cy - _y;
	loc[2][0] =(_w / 2)   + _cx - _x;
	loc[2][1] =(_h / 2)   + _cy - _y;
	loc[3][0] = -(_w / 2) + _cx - _x;
	loc[3][1] =(_h / 2)   + _cy - _y;

	rad = _rad * M_PI / 180;

	for (int i = 0; i < 4; i++)
	{
		newloc[i][0] = (loc[i][0] * cosf(rad)) - (loc[i][1] * sinf(rad)) + _cx;
		newloc[i][1] = (loc[i][0] * sinf(rad)) + (loc[i][1] * cosf(rad)) + _cy;
	}

	DrawQuadrangle(newloc[0][0], newloc[0][1], newloc[1][0], newloc[1][1], newloc[2][0], newloc[2][1], newloc[3][0], newloc[3][1], _color, _fill);
}