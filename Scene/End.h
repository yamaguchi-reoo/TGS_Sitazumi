#pragma once
#include "AbstractScene.h"
#include "DxLib.h"

class End :
    public AbstractScene
{
private:

    int ExitNum = 0;

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