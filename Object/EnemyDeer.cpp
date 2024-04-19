#include "../Utility/KeyInput.h"
#include "EnemyDeer.h"

EnemyDeer::EnemyDeer()
{
	type = ENEMY;
	can_swap = TRUE;
	for (int i = 0; i < 10; i++)
	{
		KeepMouseX[i] = 0;
		KeepMouseY[i] = 0;
	}

	for (int i = 0; i < 99; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			KeepCursor[i][j] = 0;
		}
	}

	for (int z = 0; z < 100; z++)
	{
		String[z] = 0;
	}

	number = 0;
	akusel = 0;
	EnemyFileOpenFlg = true;
	EnemyFileHandle = 0;
	NotOpen = TRUE;

	ArrayNumberX = 0;
	ArrayNumberY = 1;
	ArrayNumber = 0;

	deer_state = DeerState::LEFT;

	//location == 当たり判定用 local_location == 画像を動かすだけの物
	//locationの中に値を入れると 上記の変数のどちらでも値を使用できる
	// { x , y }
	location = { 500, 850 };
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Update()
{
	//NotOpen = EnemyDeerfs();

	EnemyDeerMove();

	//添え字の初期化
	/*if (ArrayNumber > 98)
	{
		ArrayNumber = 0;
	}*/

	//左クリックが押されたら座標を配列に格納したい
	if (KeyInput::OnMouse(MOUSE_INPUT_LEFT) == 1)
	{
		//KeepCursor[ArrayNumber][ArrayNumberX] = KeyInput::GetMouseCursor().x; //偶数の配列に格納する [0][0] [0][2] [0][4] [0][6] [0][8]
		//KeepCursor[ArrayNumber][ArrayNumberY] = KeyInput::GetMouseCursor().y; //奇数の配列に格納する [0][1] [0][3] [0][5] [0][7] [0][9]

		//ファイルを開けたら
		/*if (Deerfs)
		{
			Deerfs << KeepCursor[ArrayNumber][ArrayNumberX];
			Deerfs << ",";
			Deerfs << KeepCursor[ArrayNumber][ArrayNumberY];
			if (ArrayNumberX >= 8 && ArrayNumberY >= 9)
			{
				Deerfs << "\n";
			}
			else 
			{
				Deerfs << ",";
			}
		}

		if (ArrayNumberX >= 8 && ArrayNumberY >= 9)
		{
			ArrayNumber++;
			ArrayNumberX = 0;
			ArrayNumberY = 1;
		}

		ArrayNumberX += 2;
		ArrayNumberY += 2;*/
	}

	//akusel++;

	//Deerfs.close();
}

void EnemyDeer::Draw()const
{
	DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawFormatString(0, 80, 0xff0000, "File open? %d", NotOpen);	//ファイルが開けているか確認 TRUE or FALSE
	//DrawFormatString(0, 120, 0xff0000, "Number : %d X number : %d Y wnumber : %d", ArrayNumber, ArrayNumberX - 2, ArrayNumberY - 2);
	//DrawFormatString(0, 140, 0xff0000, "Number : %d X : %d Y : %d", ArrayNumber, KeepCursor[ArrayNumber][ArrayNumberX - 2], KeepCursor[ArrayNumber][ArrayNumberY - 2]);

	//DrawQuadrangle(KeepCursor[ArrayNumber][0], KeepCursor[ArrayNumber][1], KeepCursor[ArrayNumber][2], KeepCursor[ArrayNumber][3],
		//KeepCursor[ArrayNumber][4], KeepCursor[ArrayNumber][5], KeepCursor[ArrayNumber][6], KeepCursor[ArrayNumber][7], 0xffffff, FALSE);

	//角 左から
	//DrawQuadrangle(295, 260, 310, 260, 310, 290, 300, 290, 0xffffff, FALSE);
	//頭
	//DrawBox(300 + akusel, 300, 330 + akusel, 320, GetColor(255, 255, 255), TRUE);
	//首
	//DrawBox(315 + akusel, 325, 330 + akusel, 350, GetColor(255, 255, 255), TRUE);
	//胴体
	//DrawBox(315 + akusel, 355, 390 + akusel, 375, 0xfffffff, TRUE);
	//足 左から
	//DrawQuadrangle(315 + akusel, 380, 325 + akusel, 380, 315 + akusel, 410, 305 + akusel, 410, 0xffffff, TRUE);
	//DrawBox(330 + akusel, 380, 340 + akusel, 410, 0xffffff, TRUE);
	//DrawBox(360 + akusel, 380, 370 + akusel, 410, 0xffffff, TRUE);
	//DrawQuadrangle(380 + akusel, 380, 390 + akusel, 380, 400 + akusel, 410, 390 + akusel, 410, 0xffffff, TRUE);

	/*DrawQuadrangle(KeepMouseX[number - 1], KeepMouseY[number - 1], KeepMouseX[number - 2], KeepMouseY[number - 2],
		KeepMouseX[number - 3], KeepMouseY[number - 3], KeepMouseX[number - 4], KeepMouseY[number - 4], 0xffffff, FALSE);*/
	
	//頭
	DrawBox(local_location.x, local_location.y, local_location.x + 30, local_location.y + 20, 0xffffff, TRUE);

	//首
	DrawBox(local_location.x + 15, local_location.y + 25, local_location.x + 30, local_location.y + 50, 0xffffff, TRUE);

	//胴体
	DrawBox(local_location.x + 15, local_location.y + 55, local_location.x + 90, local_location.y + 75, 0xffffff, TRUE);

	//足　左から
	DrawQuadrangle(local_location.x + 15, local_location.y + 80, local_location.x + 25, local_location.y + 80, local_location.x + 15, local_location.y + 110, local_location.x + 5, local_location.y + 110, 0xffffff, TRUE);
	DrawBox(local_location.x + 30, local_location.y + 80, local_location.x + 40, local_location.y + 110, 0xffffff, TRUE);
	DrawBox(local_location.x + 60, local_location.y + 80, local_location.x + 70, local_location.y + 110, 0xffffff, TRUE);
	DrawQuadrangle(local_location.x + 80, local_location.y + 80, local_location.x + 90, local_location.y + 80, local_location.x + 100, local_location.y + 110, local_location.x + 90, local_location.y + 110, 0xffffff, TRUE);


	/*DrawFormatString(0, 500, 0xff0000, "ワールド座標 X：%0.1f Y：%0.1f", local_location.x, local_location.y + akusel);
	DrawFormatString(30, 520, 0xff0000, "location.x : %0.1f", DrawTest1);
	DrawFormatString(30, 540, 0xff0000, "location.y : %0.1f", DrawTest2);
	DrawFormatString(30, 560, 0xff0000, "erea.height: %0.1f", DrawTest3);
	DrawFormatString(30, 580, 0xff0000, "erea.width : %0.1f", DrawTest4);
	DrawFormatString(30, 600, 0xff0000, "type		   : %d", DrawTest5);
	DrawFormatString(30, 620, 0xff0000, "color_data : %d", DrawTest6);*/
	//DrawFormatString(30, 620, 0xff0000, "color_data : %d", DrawTest6);
}

void EnemyDeer::EnemyDeerMove()
{
	if (deer_state == DeerState::LEFT)
	{
		location.x -= 1;
	}
	else if(deer_state == DeerState::RIGHT)
	{
		location.x += 1;
	}
}

void EnemyDeer::Hit(Location _location, Erea _erea, int _type, int _color_data)
{
	DrawTest1 = _location.x;
	DrawTest2 = _location.y;
	DrawTest3 = _erea.height;
	DrawTest4 = _erea.width;
	DrawTest5 = _type;
	DrawTest6 = _color_data;

	if (location.x < _location.x + _erea.width && _type == BLOCK)
	{
		deer_state = DeerState::RIGHT;
	}
	/*else if (_type == 0)
	{
		deer_state = DeerState::LEFT;
	}*/
}

void EnemyDeer::EnemyDeerIfs()
{
	DeerIfs.open("Resource/Dat/EnemyCoordinate.txt");

	if (!DeerIfs) //  ファイルが開けない場合の処理
	{
		NotOpen = FALSE;
	}
}

void EnemyDeer::EnemyDeerOfs()
{
	DeerOfs.open("Resource/Dat/EnemyCoordinate.txt");

	if (!DeerOfs) //  ファイルが開けない場合の処理
	{
		NotOpen = FALSE;
	}
	else
	{
		DeerOfs << "Hello world!" << endl;    //  ofs に出力
		DeerOfs << "This is a pen." << endl;    //  ofs に出力
	}
}

int EnemyDeer::EnemyDeerfs()
{
	Deerfs.open("Resource/Dat/EnemyCoordinate.txt", ios::app);

	if (!Deerfs)	//ファイルが開けなかった
	{
		return FALSE;
		exit(1);
	}
	else //ファイルが開けた
	{
		return TRUE;
	}
}