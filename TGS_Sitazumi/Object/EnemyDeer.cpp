#include "../Utility/KeyInput.h"
#include "EnemyDeer.h"

EnemyDeer::EnemyDeer()
{
	type = ENEMY;
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
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Update()
{
	NotOpen = EnemyDeerfs();

	//添え字の初期化
	/*if (ArrayNumber > 98)
	{
		ArrayNumber = 0;
	}*/

	//左クリックが押されたら座標を配列に格納したい
	if (KeyInput::OnMouse(MOUSE_INPUT_LEFT) == 1)
	{
		KeepCursor[ArrayNumber][ArrayNumberX] = KeyInput::GetMouseCursor().x; //偶数の配列に格納する [0][0] [0][2] [0][4] [0][6] [0][8]
		KeepCursor[ArrayNumber][ArrayNumberY] = KeyInput::GetMouseCursor().y; //奇数の配列に格納する [0][1] [0][3] [0][5] [0][7] [0][9]

		if (Deerfs)
		{
			Deerfs << KeepCursor[ArrayNumber][ArrayNumberX] << "\n";
			Deerfs << KeepCursor[ArrayNumber][ArrayNumberY] << "\n";
		}

		if (ArrayNumberX >= 8 && ArrayNumberY >= 9)
		{
			ArrayNumber++;
			ArrayNumberX = 0;
			ArrayNumberY = 1;
		}

		ArrayNumberX += 2;
		ArrayNumberY += 2;
	}

	Deerfs.close();
}

void EnemyDeer::Draw()const
{
	DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	DrawFormatString(0, 80, 0xff0000, "File open? %d", NotOpen);	//ファイルが開けているか確認 TRUE or FALSE
	DrawFormatString(0, 120, 0xff0000, "Number : %d X number : %d Y wnumber : %d", ArrayNumber, ArrayNumberX - 2, ArrayNumberY - 2);
	DrawFormatString(0, 140, 0xff0000, "Number : %d X : %d Y : %d", ArrayNumber, KeepCursor[ArrayNumber][ArrayNumberX - 2], KeepCursor[ArrayNumber][ArrayNumberY - 2]);

	DrawQuadrangle(KeepCursor[ArrayNumber][0], KeepCursor[ArrayNumber][1], KeepCursor[ArrayNumber][2], KeepCursor[ArrayNumber][3],
		KeepCursor[ArrayNumber][4], KeepCursor[ArrayNumber][5], KeepCursor[ArrayNumber][6], KeepCursor[ArrayNumber][7], 0xffffff, FALSE);

	//角 左から
	//DrawQuadrangle(295, 260, 310, 260, 310, 290, 300, 290, 0xffffff, FALSE);
	//頭
	DrawBox(300 + akusel, 300, 330 + akusel, 320, GetColor(255, 255, 255), FALSE);
	//首
	DrawBox(315 + akusel, 325, 330 + akusel, 350, GetColor(255, 255, 255), FALSE);
	//胴体
	DrawBox(315 + akusel, 355, 390 + akusel, 375, 0xfffffff, FALSE);
	//足 左から
	//DrawBox(315, 380, 325, 420, 0xffffff, FALSE);
	DrawQuadrangle(315 + akusel, 380, 325 + akusel, 380, 315 + akusel, 410, 305 + akusel, 410, 0xffffff, FALSE);
	DrawBox(330 + akusel, 380, 340 + akusel, 410, 0xffffff, FALSE);
	DrawBox(360 + akusel, 380, 370 + akusel, 410, 0xffffff, FALSE);
	DrawQuadrangle(380 + akusel, 380, 390 + akusel, 380, 400 + akusel, 410, 390 + akusel, 410, 0xffffff, FALSE);

	/*DrawQuadrangle(KeepMouseX[number - 1], KeepMouseY[number - 1], KeepMouseX[number - 2], KeepMouseY[number - 2],
		KeepMouseX[number - 3], KeepMouseY[number - 3], KeepMouseX[number - 4], KeepMouseY[number - 4], 0xffffff, FALSE);*/
}

void EnemyDeer::Hit(Location _location, Erea _erea, int _type)
{

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
	}
	else //ファイルが開けた
	{
		return TRUE;
	}
}