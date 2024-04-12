#include "../Utility/KeyInput.h"
#include "EnemyDeer.h"

EnemyDeer::EnemyDeer()
{
	for (int i = 0; i < 10; i++)
	{
		KeepMouseX[i] = 0;
		KeepMouseY[i] = 0;
	}

	number = 0;
	akusel = 0;
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Update(GameMain* main)
{
	if (number > 9)number = 0;

	akusel++;

	if (akusel > 60)akusel = 0;

	if (KeyInput::OnMouse(MOUSE_INPUT_LEFT) == 1)
	{
		KeepMouseX[number] = KeyInput::GetMouseCursor().x;
		KeepMouseY[number] = KeyInput::GetMouseCursor().y;

		number++;
	}
}

void EnemyDeer::Draw()const
{
	DrawFormatString(0, 60, GetColor(255, 0, 0), "MouseX : %d MouseY : %d", KeyInput::GetMouseCursor().x, KeyInput::GetMouseCursor().y);
	//DrawFormatString(0, 120, 0xff0000, "QuadRangleBox X1 : %d Y1 : %d", KeepMouseX[number * 0], KeepMouseY[number * 0]); // 1 * 0 = 0
	//DrawFormatString(0, 140, 0xff0000, "QuadRangleBox X2 : %d Y2 : %d", KeepMouseX[number - 1], KeepMouseY[number - 1]); // 2 * 1 - 1 = 1
	//DrawFormatString(0, 160, 0xff0000, "QuadRangleBox X3 : %d Y3 : %d", KeepMouseX[number - 2], KeepMouseY[number - 2]); // 3 * 1 - 1 = 2
	//DrawFormatString(0, 180, 0xff0000, "QuadRangleBox X4 : %d Y4 : %d", KeepMouseX[number * 1 - 1], KeepMouseY[number * 1 - 1]); // 4 * 
	//DrawFormatString(0, 120, GetColor(255, 0, 0), "Flg : %d", KeyInput::OnMouse(MOUSE_INPUT_LEFT));
	DrawFormatString(0, 80, 0xff0000, "number : %d", number);


	if (KeepMouseX[number * 0] && KeepMouseY[number * 0] != 0)
	{
		DrawCircle(KeepMouseX[number * 0], KeepMouseY[number * 0], 2, 0xff0000, 1);

		if (KeepMouseX[number - 1] && KeepMouseY[number - 1] != 0)
		{
			DrawCircle(KeepMouseX[number - 1], KeepMouseY[number - 1], 2, 0x00ff00, 1);

			if (KeepMouseX[number - 3] && KeepMouseY[number - 3] != 0)
			{
				DrawCircle(KeepMouseX[number - 3], KeepMouseY[number - 3], 2, 0xff00ff, 1);

				if (KeepMouseX[number - 4] && KeepMouseY[number - 4] != 0)
				{
					DrawCircle(KeepMouseX[number - 4], KeepMouseY[number - 4], 2, 0x0000ff, 1);

					DrawQuadrangle(KeepMouseX[number - 4], KeepMouseY[number - 4], KeepMouseX[number - 3], KeepMouseY[number - 3],
						KeepMouseX[number - 1], KeepMouseY[number - 1], KeepMouseX[number - 2], KeepMouseY[number - 2], 0xffffff, FALSE);
				}
			}
		}
	}

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