#include "WeatherManager.h"
#include "../Scene/GameMain.h"

WeatherManager::WeatherManager():now_weather(0)
{

}

WeatherManager::~WeatherManager()
{

}

void WeatherManager::Initialize()
{

}

void WeatherManager::Update(GameMain* _g)
{
	frame++;

	switch (_g->GetNowWeather())
	{
	case 0:		//通常
		break;
	case 1:		//雨
		_g->CreateObject(new Weather(WATER), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 20,5 }, BLUE);
		break;
	case 2:		//火
		if (frame % 15 == 0)
		{
			_g->CreateObject(new Weather(FIRE), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 50,50 }, RED);
		}
		break;
	case 3:		//木
		if (frame % 7 == 0)
		{
			_g->CreateObject(new Weather(WOOD), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 10,10 }, GREEN);
		}
		break;
	default:
		break;
	}
}

void WeatherManager::Draw()const
{

}

void WeatherManager::Finalize()
{

}

