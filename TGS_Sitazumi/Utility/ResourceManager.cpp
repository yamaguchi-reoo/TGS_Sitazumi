#include "ResourceManager.h"

int ResourceManager::image_data[IMAGE_NUM];
int ResourceManager::div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];
int ResourceManager::sound_data[SOUND_NUM];

void ResourceManager::LoadResource()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		image_data[i] = LoadGraph(image_filepath[i]);
	}

	//分割数等の数値が違う場合forでは対処出来ないので、１つ１つDivGraphを呼び出す(同じならforで)
	for (int i = 0; i < DIV_IMAGE_NUM; i++)
	{
		LoadDivGraph(div_image_filepath[i], 12, 5, 3, 34, 34, div_image_data[i]);
	}

	for (int i = 0; i < SOUND_NUM; i++)
	{
		sound_data[i] = LoadSoundMem(sound_filepath[i]);
	}
}

void ResourceManager::DeleteResource()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		DeleteGraph(image_data[i]);
	}

	for (int i = 0; i < DIV_IMAGE_NUM; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			DeleteGraph(div_image_data[i][j]);
		}
	}

	for (int i = 0; i < SOUND_NUM; i++)
	{
		DeleteSoundMem(sound_data[i]);
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