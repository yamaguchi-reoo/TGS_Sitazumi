#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"


class Player;

class GameMain :
    public AbstractScene
{
private:

public:

    //コンストラクタ(_stage＝読み込むステージ)
    GameMain(int _stage);

    //デストラクタ
    ~GameMain();

    // 描画以外の更新を実装
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    
};

