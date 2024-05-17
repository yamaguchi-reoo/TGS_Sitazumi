#pragma once
#include"BoxCollider.h"

#define DELAY_TIME 90	//指定した天候に変化するまでの待ち時間（プレイヤーに警告を出す時間）

static int weather_color[4]
{
	0x000000,
	0x0000ff,
	0xff0000,
	0x00ff00,
};
class GameMain;

class WeatherManager
{
private:
	int frame;		//フレーム測定
	int now_weather;	//現在の天気
	int old_weather;	//ひとつ前の天気
	int delay;			//指定した天候に変化するまでの待ち時間測定用
	Location draw_location;	//警告表示位置
public:
	WeatherManager();
	~WeatherManager();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();
	
	//天気の設定(0=通常　1=雨　2=火　3=木)
	void SetWeather(int _type);
};

