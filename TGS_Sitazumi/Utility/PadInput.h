#pragma once
#include"DxLib.h"

#define KEY_MAX 256
#define MOUSE_MAX 256

#define BUTTONS 16
#define STICKL_X 1		
#define STICKL_Y 2		
#define MAXL_X 32767.f  //左スティックX軸の最大値(float型)
#define MAXL_Y 32767.f  //左スティックY軸の最大値(float型)

//スティック
struct Stick
{
	short ThumbX;	//横軸値
	short ThumbY;	//縦軸値
};

//マウスカーソル
struct CURSOR
{
	int x;	//横軸値
	int y;	//縦軸値
};

class PadInput
{
private:
	static char NowKey[BUTTONS]; //今回の入力キー
	static char OldKey[BUTTONS]; //前回の入力キー
	static XINPUT_STATE Input; //パッド
	static Stick Rstick; //右スティック
	static Stick Lstick; //左スティック
	
public:
	//パッド情報の更新
	static void UpdateKey()
	{
		// 入力キー取得
		GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
		for (int i = 0; i < BUTTONS; i++)
		{
			OldKey[i] = NowKey[i];
			NowKey[i] = Input.Buttons[i];
		}

		//左スティック
		Lstick.ThumbX = Input.ThumbLX;
		Lstick.ThumbY = Input.ThumbLY;
	}

	//ボタンを押された瞬間
	static bool OnButton(int button)
	{
		bool ret = (NowKey[button] == 1 && OldKey[button] == 0);
		return ret;
	}

	//ボタンを押してる間
	static bool OnPressed(int button)
	{
		bool ret = (NowKey[button] == 1);
		return ret;
	}

	//ボタンを離した瞬間
	static bool OnRelease(int button)
	{
		bool ret = (NowKey[button] == 0 && OldKey[button] == 1);
		return ret;
	}

	//スティックの傾き割合
	static float TipLeftLStick(short StickL)
	{
		if (StickL == STICKL_X)
		{
			float ratioL_X = Input.ThumbLX / MAXL_X;

			//左スティックの横軸を最大値を１とした割合
			return ratioL_X;
		}
		else if (StickL == STICKL_Y)
		{
			float ratioL_Y = Input.ThumbLY / MAXL_Y;

			//左スティックの縦軸値の最大値を１とした割合
			return ratioL_Y;
		}

		return 0;
	}

};
//キー入力
class KeyInput
{
private:
	static int NowKey[KEY_MAX];		//今回の入力キー
	static int OldKey[KEY_MAX];		//前回の入力キー
	static int NowMouse[MOUSE_MAX]; //今回の入力マウスボタン
	static int OldMouse[MOUSE_MAX]; //前回の入力マウスボタン
	static CURSOR Cursor; //カーソル
public:
	//パッド情報の更新
	static void UpdateKey() {
		// 入力キー取得
		for (int i = 0; i < KEY_MAX; i++) {
			OldKey[i] = NowKey[i];
			NowKey[i] = CheckHitKey(i);
		}
		for (int i = 0; i < MOUSE_MAX; i++)
		{
			OldMouse[i] = NowMouse[i];
			if (GetMouseInput() == i)
			{
				NowMouse[i] = 1;
			}
			else
			{
				NowMouse[i] = 0;
			}
		}
		GetMousePoint(&Cursor.x, &Cursor.y);
	}
	//ボタンを押した瞬間
	static bool OnKey(int key) {
		return (NowKey[key] == 1 && OldKey[key] == 0);
	}

	//ボタンを押した瞬間
	static bool OnPresed(int key) {
		return (NowKey[key] == 1);

	}
	//ボタンを押した瞬間
	static bool OnRelease(int key) {
		return (NowKey[key] == 0 && OldKey[key] == 1);
	}
	//マウスボタンを押された瞬間
	static bool OnMouse(int mouse)
	{
		return (NowMouse[mouse] == 1 && OldMouse[mouse] == 0);
	}

	//マウスボタンを押してる間
	static bool OnPressedMouse(int mouse)
	{
		return (NowMouse[mouse] == 1);
	}

	//マウスボタンを離した瞬間
	static bool OnReleaseMouse(int mouse)
	{
		return (NowMouse[mouse] == 0 && OldMouse[mouse] == 1);
	}
	//マウスカーソルの位置を返す
	static CURSOR GetMouseCursor()
	{
		return  Cursor;
	}
};
