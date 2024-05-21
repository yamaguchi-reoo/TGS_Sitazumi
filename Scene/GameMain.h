#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Object/EnemyDeer.h"
#include "../Object/EnemyBat.h"
#include "../Object/EnemyFrog.h"
#include "../Object/Weather.h"
#include "../Object/Object.h"
#include "../Object/WeatherManager.h"
#include "../Object/EffectSpawner.h"
#include "../Object/Boss.h"
#include "../Object/BossAttackWood.h"
#include "../Object/BossAttackFire.h"

class Player;

static int ColorList[4]
{
    0xff0000,   //赤
    0x00ff00,   //緑
    0x0000ff,   //青
    0xffffff   //白
};

struct SwapAnim
{
    bool move_flg;          //移動するか判断
    Location start;          //オブジェクトの座標（アニメーション開始地点）
    Erea erea;               //オブジェクトのサイズ
    Location location;      //交換エフェクト用変数
    float move_rad;        //交換エフェクト移動用
    int color;             //色情報
    int timer;              //エフェクト時間
    int speed;              //エフェクト速度
};

class GameMain :
    public AbstractScene
{
private:
    int frame;                                           //フレーム測定
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //ステージデータ格納
    int player_object;             //プレイヤーが配列の何番目に格納されているか

    Object* object[OBJECT_NUM] = { nullptr };    //オブジェクト格納
    WeatherManager* weather;                        //天気管理オブジェクト
    EffectSpawner* effect_spawner;                  //エフェクト管理オブジェクト

    int object_num;         //オブジェクト数
    int move_object_num;    //Updateされているオブジェクト数
    int now_stage;          //現在のステージ数
    int stage_width_num;    //ステージの横のブロック数
    int stage_height_num;   //ステージの縦のブロック数
    int stage_width;        //ステージの横の大きさ
    int stage_height;       //ステージの縦の大きさ

    bool camera_x_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool camera_y_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool x_pos_set_once;     //カメラのロック位置設定用
    bool y_pos_set_once;     //カメラのロック位置設定用
    Location lock_pos;       //カメラが動けない時に画面揺れが発生した時、カメラの位置が戻る場所
    int fps = 0;
    int now_weather;            //現在の天気(0=通常　1=雨　2=火　3=木)
    int weather_timer;      //天気オブジェクトを生成するスパン

    SwapAnim swap_anim[2];  //交換エフェクト用
    int swap_anim_timer;    //交換エフェクト時間用

    BossAttackFire* test;
public:

    //コンストラクタ
    GameMain(int _stage);

    //デストラクタ
    ~GameMain();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新
    AbstractScene* Update() override;

    //描画の更新
    void Draw() const override;

    //オブジェクトの生成
    void CreateObject(Object* _object, Location _location, Erea _erea, int _color_data);

    //オブジェクトの削除
    void DeleteObject(int i);

    //カメラの更新
    void UpdateCamera();

    //ステージデータの読込
    void LoadStageData(int _stage);

    //ステージの生成
    void SetStage(int _stage);

    //カメラ座標を初期地点に戻す
    void ResetCamera();

    //交換アニメーション
    int Swap(Object* _object1, Object* _object2);

    //プレイヤーが色交換のスローモーション中か取得
    bool GetSearchFlg();

    //プレイヤーのワールド座標を取得
    Location GetPlayerLocation();

    //カメラ座標の取得
    Location GetCameraLocation();

    //天気の取得
    int GetNowWeather() { return now_weather; }

    //エフェクトの生成
    void SpawnEffect(Location _location, Erea _erea, int _type, int _time,int _color);
};

