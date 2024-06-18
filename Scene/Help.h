#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Object/BoxCollider.h"

class Help :
    public AbstractScene
{
private:

    Location l_help_red;
    Location l_help_blue;
    Location l_help_fire;
    Location l_help_water;
    Location l_help_wood;
    Location l_help_bamboo;
    Location l_help_respawn;

    int MenuNumber = 2;
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