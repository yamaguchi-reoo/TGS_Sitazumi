#pragma once
#include"DxLib.h"

#define IMAGE_NUM 1	//画像の数
#define DIV_IMAGE_NUM 1	//分割画像の数
#define SOUND_NUM 1	//効果音の数

#define DIV_IMAGE_MAX 13	//分割画像の最大分割数

//使用する画像のパス一覧
static char image_filepath[IMAGE_NUM][256] =
{
	"Resource/Image/Sigma.png"
};

//使用する分割画像のパス一覧
static char div_image_filepath[DIV_IMAGE_NUM][256] =
{

};

//使用する音源のパス一覧
static char sound_filepath[SOUND_NUM][256] =
{

};

class ResourceManager
{
private:
	static int image_data[IMAGE_NUM];							//画像格納用
	static int div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];	//分割画像格納用
	static int sound_data[SOUND_NUM];							//音源格納用
public:

	//各データ読込（mainで一回呼ぶ）
	static void LoadResource();	

	//各データ削除（mainで一回呼ぶ）
	static void DeleteResource();

	//画像呼び出し
	static int GetGraph(int _num);

	//分割画像呼び出し
	static int GetDivGraph(int _num1, int _num2);	

	//音源再生開始
	static void StartSound(int _num);				

	//音源再生終了
	static void StopSound(int _num);				
};
