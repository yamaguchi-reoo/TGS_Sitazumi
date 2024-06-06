#pragma once
#include "AbstractScene.h"
#include"../Object/BoxCollider.h"
#include "DxLib.h"

enum class TITLE_MENU
{
    GAME_START = 0,
    HELP,
    GAME_END
};

static char menu_string[3][256] =
{
    "PLAY",
    "HELP",
    "END"
};

class Title :
    public AbstractScene
{
private:
    int frame;                  //フレーム測定用   
    Location menu_location[3];  //メニュー項目の座標
    Erea menu_size[3];          //メニュー項目の大きさ
    Location player_location;   //プレイヤーの見た目の描画位置
    Location cursor_location;   //カーソルの座標
    Location draw_stick_location;   //描画するスティックの座標
    Location draw_stick_shift;   //描画するスティックをずらす座標
    Location anim_start;        //アニメーション開始前の位置を保存しておく
    bool swap_anim_flg;         //演出開始か判断
    int swap_anim_timer;        //演出の時間測定
    bool button_draw;            //どの画像を描画するか
    int current_menu;           //選択中のメニュー
    float stick_angle;          //スティックのアングルを保存
public:
    //コンストラクタ
    Title();

    //デストラクタ
    ~Title();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;


};


