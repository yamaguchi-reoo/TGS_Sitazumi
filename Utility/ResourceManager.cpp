#include "ResourceManager.h"
#include "common.h"
#define _USE_MATH_DEFINES
#include <math.h>

char* ResourceManager::image_filepath[IMAGE_NUM];
char* ResourceManager::div_image_filepath[DIV_IMAGE_NUM];
char* ResourceManager::sound_filepath[SOUND_NUM];
int ResourceManager::image_data[IMAGE_NUM];
int ResourceManager::div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];
int ResourceManager::sound_data[SOUND_NUM];
int ResourceManager::sound_freq = 50000;

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

	// 音をすべて削除します
	InitSoundMem();
}

void ResourceManager::StageAnimInitialize()
{
	anim = 0;
	////エフェクトのアニメーション用初期定義
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		fire_anim[i].time = GetRand(20);
		wood_anim[i].shift = (float)GetRand(100) / 10;
		wood_anim[i].shift1.x = (float)((BOX_WIDTH / 10) * i);
		wood_anim[i].shift2.x = (float)((BOX_WIDTH / 10) * i);
		wood_anim[i].initial_position = wood_anim[i].shift2;
		wood_anim[i].shift1.y = BOX_HEIGHT;
		wood_anim[i].shift2.y = (float)(GetRand(190) / 10);
		water_anim[i].shift = (float)GetRand(9)/10;
		water_anim[i].shift1.x = (float)(GetRand(8) * (BOX_WIDTH / 10));
		water_anim[i].shift1.y = (float)(GetRand(8) * (BOX_HEIGHT / 10) - 15);
		water_anim[i].initial_position = water_anim[i].shift1;
		water_anim[i].erea.width = BOX_WIDTH /3;
		water_anim[i].erea.height = BOX_HEIGHT /3;
	}
}

void ResourceManager::StageAnimUpdate()
{
	//アニメーション用変数
	if (++anim > 60)
	{
		anim = 0;
	}
	//炎エフェクト用
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		if (fire_anim[i].time < 0)
		{
			fire_anim[i].shift.x = 0;
			fire_anim[i].shift.y = BOX_HEIGHT;
			fire_anim[i].erea.width = 4;
			fire_anim[i].erea.height = 4;
			fire_anim[i].shift.x += 4 * GetRand(10);
			fire_anim[i].time = 30 + GetRand(30);
			fire_anim[i].angle = (float)(GetRand(30) / 10);
		}
		else
		{
			fire_anim[i].shift.x += 0.05f + (fire_anim[i].angle - 1) / 2;
			fire_anim[i].shift.y -= 1;
			fire_anim[i].time--;
			fire_anim[i].erea.width -= 0.05f;
			fire_anim[i].erea.height -= 0.05f;
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
		if (anim >= 60)
		{
			wood_anim[i].shift2.x = wood_anim[i].initial_position.x;
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
		if (anim >= 60)
		{
			water_anim[i].shift1.y = water_anim[i].initial_position.y;
		}
	}
}

void ResourceManager::StageAnimDraw(Location _location, int _type)
{
	switch (_type)
	{
		//炎
		case FIRE:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
			DrawBoxAA(_location.x, _location.y, _location.x + BOX_WIDTH, _location.y + BOX_HEIGHT, 0xff0000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				if (i % 3 == 0)
				{
					DrawBoxAA(fire_anim[i].shift.x + _location.x,
						fire_anim[i].shift.y + _location.y,
						fire_anim[i].shift.x + fire_anim[i].erea.width + _location.x,
						fire_anim[i].shift.y + fire_anim[i].erea.height + _location.y, 0xff9900, true);
				}
				else
				{
					DrawBoxAA(fire_anim[i].shift.x + _location.x,
						fire_anim[i].shift.y + _location.y,
						fire_anim[i].shift.x + fire_anim[i].erea.width + _location.x,
						fire_anim[i].shift.y + fire_anim[i].erea.height + _location.y, 0xff0000, true);
				}
			}
			break;
			//木
		case WOOD:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
			DrawBoxAA(_location.x, _location.y, _location.x + BOX_WIDTH, _location.y + BOX_HEIGHT, 0x00ff00, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawLineAA(wood_anim[i].shift1.x + _location.x,
					wood_anim[i].shift1.y + _location.y,
					wood_anim[i].shift2.x + _location.x,
					wood_anim[i].shift2.y + _location.y, 0x00ff00, true);
			}
			break;
			//水
		case WATER:
			DrawBoxAA(_location.x, _location.y, _location.x + BOX_WIDTH, _location.y + BOX_HEIGHT, 0x0000ff, true);
			for (int i = 0; i < ANIM_BLOCK_NUM; i++)
			{
				DrawBoxAA(water_anim[i].shift1.x + _location.x,
					water_anim[i].shift1.y + _location.y,
					water_anim[i].shift1.x + water_anim[i].erea.width + _location.x,
					water_anim[i].shift1.y + water_anim[i].erea.height + _location.y, 0x0000ee, true);
			}
			break;
	default:
		break;
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
	return -1;	//画像をどこにも格納できなかった場合
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
	return -1;	//画像をどこにも格納できなかった場合
}

int ResourceManager::SetSound(const char* p)
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		//空の配列に音源を格納する
		if (sound_data[i] == NULL)
		{
			sound_data[i] = LoadSoundMem(p);
			ChangeVolumeSoundMem(100, sound_data[i]);
			sound_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ音源が既にあるならその格納場所を返す
		else if (sound_filepath[i] == p)
		{
			return i;
		}
	}
	return -1;	//音源をどこにも格納できなかった場合
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
	// 再生周波数を設定する
	SetFrequencySoundMem(sound_freq, sound_data[_num]);
	//再生
	PlaySoundMem(sound_data[_num], DX_PLAYTYPE_BACK);
}

void ResourceManager::StopSound(int _num)
{
	StopSoundMem(sound_data[_num]);
}

void ResourceManager::SetSoundFreq(int _num)
{
	sound_freq = _num;
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

	rad = (float)(_rad * M_PI / 180);

	for (int i = 0; i < 4; i++)
	{
		newloc[i][0] = (loc[i][0] * cosf(rad)) - (loc[i][1] * sinf(rad)) + _cx;
		newloc[i][1] = (loc[i][0] * sinf(rad)) + (loc[i][1] * cosf(rad)) + _cy;
	}

	DrawQuadrangleAA(newloc[0][0], newloc[0][1], newloc[1][0], newloc[1][1], newloc[2][0], newloc[2][1], newloc[3][0], newloc[3][1], _color, _fill);
}