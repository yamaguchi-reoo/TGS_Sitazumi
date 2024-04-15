#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Object/EnemyDeer.h"
#include"../Object/EnemyBat.h"

class Player;

class GameMain :
    public AbstractScene
{
private:
    Stage* stage[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //���̃I�u�W�F�N�g

    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //�X�e�[�W���

    int now_stage;          //���݂̃X�e�[�W��
    int stage_width_num;    //�X�e�[�W�u���b�N�̉���
    int stage_height_num;   //�X�e�[�W�u���b�N�̏c��
    int stage_width;        //�X�e�[�W����
    int stage_height;       //�X�e�[�W�c��

    EnemyDeer* enemydeer;
    EnemyBat* enemybat;

    Player* player;

public:

    //�R���X�g���N�^(_stage���ǂݍ��ރX�e�[�W)
    GameMain(int _stage);

    //�f�X�g���N�^
    ~GameMain();

    // �`��ȊO�̍X�V�����
    AbstractScene* Update() override;

    //�`��Ɋւ��邱�Ƃ����
    void Draw() const override;

    //�J�����̍X�V���J������h�炷
    void UpdateCamera();

    //�X�e�[�W�t�@�C����ǂݍ���
    void LoadStageData(int _stage);

    //���̃X�e�[�W�֑J�ڂ���
    void SetStage(int _stage);

    //�X�e�[�W�ɑ��̃X�e�[�W��type����n���p
    int GetStageType(int _i, int _j);
};

