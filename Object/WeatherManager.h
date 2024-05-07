#pragma once

class GameMain;

class WeatherManager
{
private:
	int now_weather;
public:
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();
};

