#include "SceneManager.h"
#include "../Utility/PadInput.h"
#include "DxLib.h"

void SceneManager::Initialize()
{
	mScene->Initialize();
	p = nullptr;
	QuitFlg = false;
	cursor = 0;
}

void SceneManager::Finalize()
{
	mScene->Finalize();
}

AbstractScene* SceneManager::Update() {
	if (PadInput::OnButton(XINPUT_BUTTON_BACK)) {
		QuitFlg = !QuitFlg;
	}
	if (!QuitFlg)p = mScene->Update();
	if (QuitFlg) {
		if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
		{
			cursor = 0;
		}
		else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
		{
			cursor = 1;
		}

		if (PadInput::OnButton(XINPUT_BUTTON_B)) {
			if (cursor == 0)
			{
				QuitFlg = false;
			}
			else
			{
				p = nullptr;
			}
		}
	}

	if (p != mScene && p != nullptr) {
		mScene->Finalize();
		delete mScene;
		mScene = p; // 次に表示したいシーン
		mScene->Initialize();
	};

	return p;
};

void SceneManager::Draw() const {
	mScene->Draw();
	if (QuitFlg) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, 1280, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		SetFontSize(32);
		DrawString(540, 320, "QuitGame?", 0xffffff);

		DrawString(540, 390, "No", 0xffffff);
		DrawString(620, 390, "Yes", 0xffffff);

		DrawBox(540 + cursor * 80, 390, 600 + cursor * 80, 420, 0xffffff, FALSE);
	}
};
