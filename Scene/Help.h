#pragma once
#include "AbstractScene.h"
#include "DxLib.h"

class Help :
    public AbstractScene
{
private:

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