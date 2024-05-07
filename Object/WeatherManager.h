#pragma once

class GameMain;

class WeatherManager
{
private:
	int frame;		//フレーム測定
	int now_weather;
public:
	WeatherManager();
	~WeatherManager();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();

	//天気の設定(0=通常　1=雨　2=火　3=木)
	void SetWeather(int _type) { now_weather = _type; }
};

