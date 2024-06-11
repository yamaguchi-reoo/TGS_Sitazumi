#pragma once
#include "AbstractScene.h"
#include "DxLib.h"

class Help :
    public AbstractScene
{
private:

    //int hStrWidth, hStrLen;
    int MenuNumber = 1;
    float TriangleXNum = 0.0f, TriangleYNum = 0.0f;
    int MenuY = 1;
    int wt = 0;
    bool flg = false;

public:
    //コンストラクタ
    Help();

    //デストラクタ
    ~Help();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;


};