#include "SceneManager.h"

AbstractScene* SceneManager::Update() {
	AbstractScene* p = mScene->Update();

	if (p != mScene) {
		delete mScene;
		mScene = p; // ŽŸ‚É•\Ž¦‚µ‚½‚¢ƒV[ƒ“
	};

	return p;
};

void SceneManager::Draw() const {
	mScene->Draw();
};
