#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "Title.h"


class End :
    public AbstractScene
{
private:

    Location logo_location[7];
    int ExitNum = 0;
    int shift_y;

public:
    //コンストラクタ
    End();

    //デストラクタ
    ~End();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;


};