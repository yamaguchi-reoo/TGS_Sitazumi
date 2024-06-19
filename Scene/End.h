#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "Title.h"
#include "../Object/EnemyBat.h"


class End :
    public AbstractScene
{
private:

    Location logo_location[7];//タイトル
    int ExitNum = 0;          //終了用
    int shift_y;              //スクロール用
    int stop_time;

    //コウモリ用
    Location bat_loction;
    int up;             //加算用
    //シカ用
    Location deer_location;
    float leg_angle[4];
    float deer_speed;
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

    //シカ描画
    void DeerDraw()const;
    void DeerUpdate();

    //コウモリ描画
    void BatDraw()const;
    void BatUpdate();

    //カエル描画
    void FrogDraw()const;

};