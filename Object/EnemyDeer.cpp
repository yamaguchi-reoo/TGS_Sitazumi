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

void EnemyDeer::Initialize(Location _location, Erea _erea, int _color_data)
{
	//一旦引数はパス 鹿がどこにでるかの座標 
	//地面と完全に座標が一致していると地面に引っかかって動かなくなる 859
	location = _location;
	//当たり判定の大きさを変更できる
	//erea = {110, 100};
	erea = _erea;
	color = _color_data;
}

void EnemyDeer::Update(GameMain* _g)
{
	EnemyDeerMove();
}

void EnemyDeer::Draw()const
{
	if (deer_state == DeerState::LEFT)
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

	//DrawCircleAA(local_location.x - 5.0f, local_location.y - 20.0f, 2, 32, 0xffffff, TRUE);
	//DrawCircleAA(local_location.x + 3.0f, local_location.y - 25.0f, 2, 32, 0xffffff, TRUE);
	//DrawCircleAA(local_location.x + 15.0f, local_location.y + 2.0f, 2, 32, 0xffffff, TRUE);
	//DrawCircleAA(local_location.x + 7.0f, local_location.y + 5.0f, 2, 32, 0xffffff, TRUE);
	//
	//DrawCircleAA(local_location.x + 14.0f, local_location.y + 5.0f, 2, 32, 0xffffff, TRUE);


	//マウス座標
	//DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);

	//当たり判定のBox
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, GetColor(255, 255, 255), FALSE);

	//DrawFormatString(50, 240, 0xff0000, "スクリーン座標 X：%0.1f Y：%0.1f", location.x, location.y);
	DrawFormatString(50, 100, 0xff0000, "ワールド座標 X：%0.1f Y：%0.1f", local_location.x, local_location.y);
	DrawFormatString(50, 120, 0xff0000, "location.x : %0.1f", DrawTest1);
	DrawFormatString(50, 140, 0xff0000, "location.y : %0.1f", DrawTest2);
	DrawFormatString(50, 160, 0xff0000, "erea.height: %0.1f", DrawTest3);
	DrawFormatString(50, 180, 0xff0000, "erea.width : %0.1f", DrawTest4);
	DrawFormatString(50, 200, 0xff0000, "type	       : %d", DrawTest5);
	DrawFormatString(50, 220, 0xff0000, "color_data : %d", DrawTest6);

	DrawFormatString(50, 20, 0xff0000, "DeerState ; %d", deer_state);

	DrawCircleAA(local_location.x - erea.width, local_location.y + erea.height, 2, 32, 0xffffff, TRUE);
	DrawCircleAA(local_location.x + erea.width, local_location.y + erea.height, 2, 32, 0xff0000, TRUE);
}

void EnemyDeer::EnemyDeerMove()
{
	if (deer_state == DeerState::GRAVITY)
	{
		location.y += 10;
	}

	if (deer_state == DeerState::IDLE)
	{
		location.x;
		location.y;
	}

	if (deer_state == DeerState::LEFT)
	{
		location.x -= 2;
	}

	if(deer_state == DeerState::RIGHT)
	{
		location.x += 2;
	}
}

void EnemyDeer::Finalize()
{

}

//何かと当たった時の処理 _
//location=当たった相手の座標 _erea=当たった相手の大きさ _type=当たった相手の種類(0=ステージ 1=プレイヤー 2=敵 3=炎 4=木 5=水)
//_color_data=当たった相手も色情報
void EnemyDeer::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	DrawTest1 = _location.x;
	DrawTest2 = _location.y;
	DrawTest3 = _erea.height;
	DrawTest4 = _erea.width;
	DrawTest5 = _type;
	DrawTest6 = _color_data;

	//空中から地面に当たると待ち状態に移行
	if (deer_state == DeerState::GRAVITY && local_location.y + erea.height + erea.height && _type == BLOCK)
	{
		deer_state = DeerState::IDLE;
		deer_state = DeerState::RIGHT;
	}

	//右下と左下の頂点座標にブロックが無ければ落ちる
	/*if (deer_state == DeerState::RIGHT && local_location.y + erea.height + erea.height && _type != BLOCK 
		|| deer_state == DeerState::LEFT && local_location.x + erea.width + erea.width + erea.height + erea.height && _type != BLOCK)
	{
		deer_state = DeerState::GRAVITY;
	}*/
	
	if (deer_state == DeerState::RIGHT)
	{
		if (local_location.x + erea.width && +local_location.y + erea.height && _type != BLOCK)
		{
			deer_state = DeerState::LEFT;
		}
	}
	
	/*if(deer_state == DeerState::LEFT)
	{
		if (local_location.x - erea.width && local_location.y + erea.height && _type != BLOCK && _color_data == -1)
		{
			deer_state = DeerState::RIGHT;
		}
	}*/

	/*if (location.x < _location.x + _erea.width + _erea.height && _type == BLOCK || location.x + erea.width < _location.x + _erea.width + _erea.height && _type == BLOCK)
	{
		deer_state = DeerState::IDLE;
	}*/

	////_locationは左上の座標を取ってる 左の壁に当たると右に方向転換
	//if (location.x < _location.x + _erea.width && _type == BLOCK)
	//{
	//	deer_state = DeerState::RIGHT;
	//}

	////右の壁に当たると左に方向転換
	//if (location.x + erea.width < _location.x + _erea.width && _type == BLOCK)
	//{
	//	deer_state = DeerState::LEFT;
	//}
}