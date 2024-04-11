#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"

class Player;

class GameMain :
    public AbstractScene
{
private:
    Stage* stage[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //床のオブジェクト

    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //ステージ情報

    int now_stage;          //現在のステージ数
    int stage_width_num;    //ステージブロックの横数
    int stage_height_num;   //ステージブロックの縦数
    int stage_width;        //ステージ横幅
    int stage_height;       //ステージ縦幅

    Player* player;

public:

    //コンストラクタ(_stage＝読み込むステージ)
    GameMain(int _stage);

    //デストラクタ
    ~GameMain();

    // 描画以外の更新を実装
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    //カメラの更新＆カメラを揺らす
    void UpdateCamera();

    //ステージファイルを読み込む
    void LoadStageData(int _stage);

    //次のステージへ遷移する
    void SetStage(int _stage);

    //ステージに他のステージのtype情報を渡す用
    int GetStageType(int _i, int _j);
};

