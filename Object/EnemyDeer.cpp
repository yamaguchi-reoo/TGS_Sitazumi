#include "../Utility/KeyInput.h"
#include "EnemyDeer.h"

EnemyDeer::EnemyDeer()
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;

	//deer_state = DeerState::LEFT;
	deer_state = DeerState::GRAVITY;

	//location == 当たり判定用 local_location == 画像を動かすだけの物
	//locationの中に値を入れると 上記の変数のどちらでも値を使用できる
	// { x , y }
	//location = { 500, 850 };

	deer_spawn = true;
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	//一旦引数はパス 鹿がどこにでるかの座標 
	//地面と完全に座標が一致していると地面に引っかかって動かなくなる 859
	location = _location;
	//当たり判定の大きさを変更できる
	//erea = {100, 100};
	erea = _erea;
	color = _color_data;

	object_pos = _object_pos;
}

void EnemyDeer::Update(GameMain* _g)
{
	EnemyDeerMove();
}

void EnemyDeer::Draw()const
{
	if (deer_state == DeerState::LEFT || deer_state == DeerState::GRAVITY || deer_state == DeerState::IDLE || deer_state == DeerState::DEATH)
	{
		//角
		//DrawQuadrangleAA(local_location.x - 5.0f, local_location.y - 20.0f, local_location.x + 2.0f, local_location.y - 25.0f, local_location.x + 15.0f, local_location.y + 2.0f, local_location.x + 7.0f, local_location.y + 5.0f, color, TRUE);
		//DrawQuadrangleAA()

		//頭
		DrawBoxAA(local_location.x, local_location.y, local_location.x + 30.0f, local_location.y + 20.0f, color, TRUE);

		//首
		DrawBoxAA(local_location.x + 13.0f, local_location.y + 25.0f, local_location.x + 30.0f, local_location.y + 35.0f, color, TRUE);
		DrawBoxAA(local_location.x + 20.0f, local_location.y + 40.0f, local_location.x + 30.0f, local_location.y + 50.0f, color, TRUE);

		//胴体
		DrawBoxAA(local_location.x + 20.0f, local_location.y + 55.0f, local_location.x + 85.0f, local_location.y + 70.0f, color, TRUE);

		//足　左から
		DrawQuadrangleAA(local_location.x + 20.0f, local_location.y + 75.0f, local_location.x + 30.0f, local_location.y + 75.0f, local_location.x + 25.0f, local_location.y + 100.0f, local_location.x + 15.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 35.0f, local_location.y + 75.0f, local_location.x + 45.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 60.0f, local_location.y + 75.0f, local_location.x + 70.0f, local_location.y + 100.0f, color, TRUE);
		DrawQuadrangleAA(local_location.x + 75.0f, local_location.y + 75.0f, local_location.x + 85.0f, local_location.y + 75.0f, local_location.x + 95.0f, local_location.y + 100.0f, local_location.x + 85.0f, local_location.y + 100.0f, color, TRUE);
	}
	else if (deer_state == DeerState::RIGHT)
	{
		//角
		//DrawQuadrangleAA(local_location.x - 5.0f, local_location.y - 20.0f, local_location.x + 2.0f, local_location.y - 25.0f, local_location.x + 15.0f, local_location.y + 2.0f, local_location.x + 7.0f, local_location.y + 5.0f, color, TRUE);

		//頭
		DrawBoxAA(local_location.x + 70.0f, local_location.y, local_location.x + 100.0f, local_location.y + 20.0f, color, TRUE);

		//首
		DrawBoxAA(local_location.x + 70.0f, local_location.y + 25.0f, local_location.x + 87.0f, local_location.y + 35.0f, color, TRUE);
		DrawBoxAA(local_location.x + 70.0f, local_location.y + 40.0f, local_location.x + 80.0f, local_location.y + 50.0f, color, TRUE);

		//胴体
		DrawBoxAA(local_location.x + 15.0f, local_location.y + 55.0f, local_location.x + 80.0f, local_location.y + 70.0f, color, TRUE);

		//足　左から
		DrawQuadrangleAA(local_location.x + 70.0f, local_location.y + 75.0f, local_location.x + 80.0f, local_location.y + 75.0f, local_location.x + 85.0f, local_location.y + 100.0f, local_location.x + 75.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 55.0f, local_location.y + 75.0f, local_location.x + 65.0f, local_location.y + 100.0f, color, TRUE);
		DrawBoxAA(local_location.x + 30.0f, local_location.y + 75.0f, local_location.x + 40.0f, local_location.y + 100.0f, color, TRUE);
		DrawQuadrangleAA(local_location.x + 15.0f, local_location.y + 75.0f, local_location.x + 25.0f, local_location.y + 75.0f, local_location.x + 20.0f, local_location.y + 100.0f, local_location.x + 10.0f, local_location.y + 100.0f, color, TRUE);
	}
	
	//マウス座標
	//DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);

	//当たり判定のBox
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, GetColor(255, 255, 255), FALSE);

	DrawFormatString(50, 60, 0xff0000, "スクリーン(location) X：%0.1f + %0.1f Y：%0.1f + %0.1f", location.x, erea.width, location.y, erea.height);
	DrawFormatString(50, 80, 0xff0000, "ワールド(local) X     ：%0.1f Y：%0.1f", local_location.x, local_location.y);
	DrawFormatString(50, 120, 0xff0000, "相手のlocation.x : %0.1f", DrawTest1);
	DrawFormatString(50, 140, 0xff0000, "相手のlocation.y : %0.1f", DrawTest2);
	DrawFormatString(50, 160, 0xff0000, "erea.height: %0.1f", DrawTest3);
	DrawFormatString(50, 180, 0xff0000, "erea.width : %0.1f", DrawTest4);
	DrawFormatString(50, 200, 0xff0000, "type	       : %d", DrawTest5);
	DrawFormatString(50, 220, 0xff0000, "color_data : %d", DrawTest6);

	DrawFormatString(50, 20, 0xff0000, "DeerState ; %d", deer_state);

	DrawCircleAA(local_location.x, local_location.y, 2, 32, 0x00ff00, TRUE);
	DrawCircleAA(local_location.x, local_location.y + erea.height, 2, 32, 0xff00ff, TRUE);
	//DrawCircleAA(local_location.x, local_location.y + erea.height, 2, 32, 0x00ff00, TRUE);

	if (deer_death == true)
	{
		DrawFormatString(local_location.x, local_location.y, 0xffffff, "DEATH");
	}
}

void EnemyDeer::EnemyDeerMove()
{
	switch (deer_state)
	{
	case DeerState::GRAVITY:

		location.y += 2;

		break;

	case DeerState::IDLE:

		location.x;
		location.y;

		break;

	case DeerState::LEFT:

		location.x -= 5;

		break;

	case DeerState::RIGHT:

		location.x += 1;

		break;
	}
}

void EnemyDeer::Finalize()
{

}

//何かと当たった時の処理 _
//location=当たった相手の座標 _erea=当たった相手の大きさ _type=当たった相手の種類
// (0=ステージ(BLOCK) 1=プレイヤー(PLAYER) 2=敵 3=炎(FIRE) 4=木(WOOD) 5=水(WATER))
//_color_data=当たった相手も色情報
//自分の当たり判定を取っている部分は 一番下の辺
void EnemyDeer::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	DrawTest1 = _location.x;
	DrawTest2 = _location.y;
	DrawTest3 = _erea.height;
	DrawTest4 = _erea.width;
	DrawTest5 = _type;
	DrawTest6 = _color_data;

	switch (deer_state)
	{
	case DeerState::GRAVITY:

		//空中から重力により地面に当たると待ち状態に移行
		if (location.y + erea.height >= _location.y && _type == BLOCK && _color_data == -1)
		{
			deer_state = DeerState::IDLE;
		}

		break;

	case DeerState::IDLE:

		//左に動くように変更
		//deer_state = DeerState::DEATH;

		break;

	case DeerState::LEFT:

		/*if (location.x < _location.x + _erea.width && local_location.y - erea.height == _location.y &&_type == BLOCK && _color_data == -1)
		{
			deer_state = DeerState::RIGHT;
		}*/

		break;

	case DeerState::RIGHT:
		break;

	case DeerState::DEATH:
		break;
	}

	if ((_type == FIRE && this->color == GREEN) || (_type == WATER && this->color == RED) || (_type == WOOD && this->color == BLUE))
	{
		deer_death = true;
	}
}