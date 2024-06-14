#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Object/Player.h"

class Help :
    public AbstractScene
{
private:

    int MenuNumber = 3;
    int MenuY = 1;
    int wt = 0;
    bool button_draw = false;
    int frame = 0;

    int decision_se;    //決定SE	  
    int cursor_se;      //カーソルSE

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