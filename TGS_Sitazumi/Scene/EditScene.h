#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Stage.h"
#include "../Utility/PadInput.h"

#define OBJECT_TYPE_NUM 8  //配置できるオブジェクトの種類数
#define WIDTH_BUTTON_POS_X 100  //横幅調節ボタンのX位置
#define WIDTH_BUTTON_POS_Y 40  //横幅調節ボタンのY位置
#define HEIGHT_BUTTON_POS_X 180  //縦幅調節ボタンのX位置
#define HEIGHT_BUTTON_POS_Y 35 //縦幅調節ボタンのY位置

//エリア区分
enum SelectErea
{
    STAGE_EDIT = 0,
    TOOL_BOX,
};

static char obj_string[OBJECT_TYPE_NUM][256] =
{
    "無",
    "地面",
    "赤",
    "青",
    "緑",
    "炎",
    "水",
    "木",
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
    Location width_button_location;                       //ステージ幅変更用ボタンの位置
    Location height_button_location;                       //ステージ高さ変更用ボタンの位置
    bool current_leftbutton_flg;                          //ステージサイズ変更用の左ボタンが選ばれているか
    bool current_rightbutton_flg;                         //ステージサイズ変更用の右ボタンが選ばれているか
    bool current_upbutton_flg;                            //ステージサイズ変更用の上ボタンが選ばれているか
    bool current_downbutton_flg;                          //ステージサイズ変更用の下ボタンが選ばれているか
    int button_interval;                                  //ステージ幅変更のインターバル
    Location tool_location;                               //ツールボックスの左上座標
    Erea tool_size;                                       //左上のツールボックスの幅
    bool tool_pickup_flg;                                 //ツールボックスを持っているか判断
    bool select_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];  //そのデータが現在選択中かどうか

    int stage_width_num;        //ステージのブロックの横の個数 
    int stage_height_num;       //ステージのブロックの縦の個数
public:
    //コンストラクタ
    EditScene(int _stage);
    //デストラクタ
    ~EditScene();

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

    //どの箱が選択中かの情報をリセットする
    void ResetSelectData();
};



