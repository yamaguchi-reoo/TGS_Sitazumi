#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Object/EnemyDeer.h"
#include"../Object/EnemyBat.h"
#include"../Object/Object.h"

class Player;

class GameMain :
    public AbstractScene
{
private:
    Object* object[OBJECT_NUM];    //オブジェクト格納
    int player_object;             //プレイヤーが配列の何番目に格納されているか
    //↓なぜか消したらエラー
    EnemyDeer* enemydeer;  
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //�X�e�[�W���

    int now_stage;          //���݂̃X�e�[�W��
    int stage_width_num;    //�X�e�[�W�u���b�N�̉���
    int stage_height_num;   //�X�e�[�W�u���b�N�̏c��
    int stage_width;        //�X�e�[�W����
    int stage_height;       //�X�e�[�W�c��

    bool camera_x_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool camera_y_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool x_pos_set_once;     //カメラのロック位置設定用
    bool y_pos_set_once;     //カメラのロック位置設定用
    Location lock_pos;       //カメラが動けない時に画面揺れが発生した時、カメラの位置が戻る場所

    int fps = 0;
public:

    //�R���X�g���N�^(_stage���ǂݍ��ރX�e�[�W)
    GameMain(int _stage);

    //�f�X�g���N�^
    ~GameMain();

    // �`��ȊO�̍X�V�����
    AbstractScene* Update() override;

    //�`��Ɋւ��邱�Ƃ����
    void Draw() const override;

    //オブジェクトの生成
    void CreateObject(Object* _object, Location _location, Erea _erea, int _color_data);

    //オブジェクトの削除
    void DeleteObject(int i);

    //�J�����̍X�V���J������h�炷
    void UpdateCamera();

    //�X�e�[�W�t�@�C����ǂݍ���
    void LoadStageData(int _stage);

    //���̃X�e�[�W�֑J�ڂ���
    void SetStage(int _stage);

    //�X�e�[�W�ɑ��̃X�e�[�W��type����n���p
    int GetStageType(int _i, int _j);

    //カメラ座標を初期地点に戻す
    void ResetCamera();
};

