#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
enum class TITLE_MENU
{
    GAME_START = 0,
    GAME_END
};
class Title :
    public AbstractScene
{
private:

public:
    //コンストラクタ
    Title();

    //デストラクタ
    ~Title();

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;
};


