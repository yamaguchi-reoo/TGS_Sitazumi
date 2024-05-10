#pragma once
#include"DxLib.h"

#define IMAGE_NUM 100	//画像の最大数
#define DIV_IMAGE_NUM 100	//分割画像の最大数
#define SOUND_NUM 100	//効果音の最大数

#define DIV_IMAGE_MAX 13	//分割画像の最大分割数

class ResourceManager
{
private:
	//使用する画像のパス一覧
	static char* image_filepath[IMAGE_NUM];					//画像パス格納用
	static char* div_image_filepath[DIV_IMAGE_NUM];			//分割画像パス格納用
	static char* sound_filepath[SOUND_NUM];					//音源パス格納用
	static int image_data[IMAGE_NUM];							//画像格納用
	static int div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];	//分割画像格納用
	static int sound_data[SOUND_NUM];							//音源格納用
public:
	//各データ削除（mainで一回呼ぶ）
	static void DeleteResource();

	//画像格納
	static int SetGraph(const char* p);

	//分割画像格納
	static int SetDivGraph(const char* p, int AllNum, int XNum, int YNum, int  XSize, int YSize);

	//音源格納
	static int SetSound(const char* p);

	//画像呼び出し
	static int GetGraph(int _num);

	//分割画像呼び出し
	static int GetDivGraph(int _num1, int _num2);

	//音源再生開始
	static void StartSound(int _num);

	//音源再生終了
	static void StopSound(int _num);

	//回転四角形描画 _x,_y=描画座標 _w,_h=図形サイズ _cx,_cy=回転の中心 _rad=回転角度 _color=色 _fill=塗りつぶすかどうか _x,_yで指定した座標を中心として_rad度回転した四角形を描画する
	static void DrawRotaBox(float _x, float _y, float _w, float _h, float _cx, float _cy,float _rad,int _color,bool _fill);
};
