#include"EnemyBat.h"
#include"../Utility/KeyInput.h"

EnemyBat::EnemyBat()
{
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Update(GameMain* main)
{

}

void EnemyBat::Draw() const
{
	DrawFormatString(200, 200, GetColor(255, 255, 225), "%d   %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawQuadrangle
}
