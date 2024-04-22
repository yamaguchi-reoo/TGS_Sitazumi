#include "ResourceManager.h"

char* ResourceManager::image_filepath[IMAGE_NUM];
char* ResourceManager::div_image_filepath[DIV_IMAGE_NUM];
char* ResourceManager::sound_filepath[SOUND_NUM];
int ResourceManager::image_data[IMAGE_NUM];
int ResourceManager::div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];
int ResourceManager::sound_data[SOUND_NUM];


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