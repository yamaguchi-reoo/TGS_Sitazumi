#pragma once
#include "../Utility/common.h"
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Stage.h"
#include "../Utility/KeyInput.h"

#define OBJECT_TYPE_NUM 24       //配置できるオブジェクトの種類数
#define UI_OBJECT_TYPE_NUM 10     //配置できるオブジェクトの種類数
#define WIDTH_BUTTON_POS_X 100   //横幅調節ボタンのX位置
#define WIDTH_BUTTON_POS_Y 40    //横幅調節ボタンのY位置
#define HEIGHT_BUTTON_POS_X 180  //縦幅調節ボタンのX位置
#define HEIGHT_BUTTON_POS_Y 35   //縦幅調節ボタンのY位置

//エリア区分
enum SelectErea
{
    STAGE_EDIT = 0,
    TOOL_BOX,
    SELECT_TYPE,
    MINIMAP,
};

static char obj_string[UI_OBJECT_TYPE_NUM][256] =
{
    "無",
    "地面",
    "色地面",
    "dmg",
    "プレ",
    "鹿",
    "蝙蝠",
    "蛙",
    "ボス",
    "天気",
};

static int can_select_type[UI_OBJECT_TYPE_NUM][2] =
{
    {0,1},
    {1,2},
    {1,3},
    {1,3},
    {0,1},
    {1,3},
    {1,3},
    {1,3},
    {0,1},
    {1,4},
};
static char block_type_string[UI_OBJECT_TYPE_NUM][5][256] =
{
    {"無"," "," "," "," ",},
    {"白","灰","",""," ",},
    {"赤","緑","青"," "," ",},
    {"炎","木","水"," "," ",},
    {"プレ"," "," "," "," ",},
    {"赤鹿","緑鹿","青鹿"," "," ",},
    {"赤蝙","緑蝙","青蝙"," "," ",},
    {"赤蛙","緑蛙","青蛙"," "," ",},
    {"ボス"," "," "," "," "},
    {"通常","雨","火山","種"," "},
};

//Edit表示用文字色データ
static int draw_block_color[24]
{
    0xffffff,	//無
    0x000000,	//白ブロック
    0xaaaaaa,	//灰ブロック
    0xff0000,	//赤ブロック
    0x00ff00,	//緑ブロック

    0x0000ff,	//青ブロック
    0xcc0000,	//赤エリア
    0x00cc00,	//緑エリア
    0x0000cc,	//青エリア
    0xffff00,	//プレイヤー初期位置

    0x880000,	//赤鹿
    0x008800,	//緑鹿
    0x000088,	//青鹿
    0x770000,	//赤蝙蝠
    0x007700,	//緑蝙蝠

    0x000077,	//青蝙蝠
    0x660000,	//赤蛙
    0x006600,	//緑蛙
    0x000066,	//青蛙
    0xffffff,	//ボス

    0xffffff,	//通常天気
    0x0000ff,	//雨
    0xff0000,	//火球
    0x00ff00,	//種

};

class EditScene :
    public AbstractScene
{

private:
    int now_select_erea;                                  //現在選択中のエリア(0=ツールボックス 1=ステージ)
    int now_stage;                                        //現在編集中のステージ
    CURSOR cursor;                                        //マウスカーソル
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];    //ステージのデータ格納用
    int old_stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];//ステージの変更前データ格納用
    Stage* stage[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];      //床のオブジェクト
    int current_type;                                     //今選ばれているオブジェクトタイプ
    int ui_current_type;                                  //今選ばれているオブジェクトタイプ(見た目)
    Location width_button_location;                       //ステージ幅変更用ボタンの位置
    Location height_button_location;                      //ステージ高さ変更用ボタンの位置
    bool current_leftbutton_flg;                          //ステージサイズ変更用の左ボタンが選ばれているか
    bool current_rightbutton_flg;                         //ステージサイズ変更用の右ボタンが選ばれているか
    bool current_upbutton_flg;                            //ステージサイズ変更用の上ボタンが選ばれているか
    bool current_downbutton_flg;                          //ステージサイズ変更用の下ボタンが選ばれているか
    int button_interval;                                  //ステージ幅変更のインターバル
    Location tool_location;                               //ツールボックスの左上座標
    Erea tool_size;                                       //左上のツールボックスの幅
    bool tool_pickup_flg;                                 //ツールボックスを持っているか判断
    bool select_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];  //そのデータが現在選択中かどうか
    int current_type_select;                              //ブロックのタイプ選択画面が開かれているかどうか(-1=何も開いていない 0以降=その数字の選択画面を開いている)
    int disp_num;                                         //現在選択されているブロックのタイプ
    int now_current_type;                                 //現在置けるブロックのタイプ
    Location current_type_location;                       //ブロックのタイプを選択するボックスの座標
    Erea current_type_erea;                               //ブロックのタイプを選択するボックスの大きさ

    int stage_width_num;                                  //ステージのブロックの横の個数 
    int stage_height_num;                                 //ステージのブロックの縦の個数
    int double_click;                                     //ダブルクリックの受付時間測定
    Location player_spawn_location;                       //プレイヤースポーンブロックの位置

    Location minimap_location;                            //ミニマップの位置
    float minimap_size;                                   //ミニマップの大きさ
    bool minimap_pickup_flg;                              //ミニマップを持っているか判断
public:
    //コンストラクタ
    EditScene(int _stage);
    //デストラクタ
    ~EditScene();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    // 描画以外の更新を実装
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    //ステージを生成する
    void LoadStageData(int _stage);

    //ステージのファイルを更新する
    void UpdateStageData(int _stage);

    //ステージの横幅、立幅を更新する
    void UpdateStage(int _width, int _height);

    //ひとつ前のデータの保持
    void SaveOldData();

    //ステージ全体を縦にずらす
    void StageShift(int _num);

    //今どのエリアにカーソルがあるか
    int ChechSelectErea();

    //ツールボックスが画面外に出ないように移動する
    void MoveInsideScreen();

    //ミニマップが画面外に出ないように移動する
    void MiniMapMoveInsideScreen();

    //どの箱が選択中かの情報をリセットする
    void ResetSelectData();

    //そのオブジェクトが画面内に居るか判断
    bool CheckInScreen(Stage* _stage)const;
};



