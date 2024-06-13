#pragma once
#include "AbstractScene.h"
#include"../Object/BoxCollider.h"
#include "DxLib.h"
#include<vector>
#include<random>

#define BG_BLOCK_WIDTH_NUM 32   //ブロックの横の数
#define BG_BLOCK_HEIGHT_NUM 18   //ブロックの横の数

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

struct BackGroundImage {
    bool flg;
    Location location;
    Erea erea;
    bool move_flg;
    Location move_goal;
    float move_speed;
    int color;
    float move_rad;
    int anim_size;
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

    bool end_game_flg;          //ゲーム終了のアニメーション
    int end_game_count;          //ゲーム終了のアニメーション測定
    int title_image_handle;     //タイトル画像のハンドル保管用

    int interval_ = 60;
    int cellSize_ = 50;
    struct XYIdx {
        int xidx, yidx;
    };
    std::mt19937 mt_;
    std::vector<XYIdx> tiles_;

    int logo_img;       //タイトルロゴの格納
    Erea logo_size;         //ロゴ画像の大きさ
    int swap_se;        //交換SE

    BackGroundImage bg[BG_BLOCK_WIDTH_NUM][BG_BLOCK_HEIGHT_NUM];    //背景情報保存
    int bg_handle;
    
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

    //ゲームを終了するときの処理
    void GameEnd();

    //タイトルの背景更新
    void bgUpdate();

    //４色の中から一定確率で抽選
    int GetRandColor();
};


