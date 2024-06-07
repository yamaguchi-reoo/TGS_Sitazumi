#pragma once

#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff
#define WHITE 0xffffff

class ColorData
{
public:
	//色情報
//マクロの4色から入れる
	int color;
	int draw_color;		//描画する色
	//色の交換
	void ChangeColor(ColorData* cd);

	void SetColorData(int c)
	{
		if (this != nullptr && c != 0x000000)
		{
			color = c;
		}
	}

	int GetColorData()
	{
		if (this != nullptr)
		{
			return color;
		}
		return -1;
	}

	void SetDrawColor(int c)
	{
		draw_color = c;
	}
};

