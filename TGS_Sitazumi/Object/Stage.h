#pragma once
#include "../Utility/common.h"

#include "BoxCollider.h"
#include "ColorData.h"

#define ANIM_BLOCK_NUM 10 //�A�j���[�V�����Ɏg���u���b�N�̐�
//ColorData�i�[�p
static int color_data[6]
{
	0,
	WHITE,
	0,
	RED,
	GREEN,
	BLUE
};

//�f�o�b�O�\���p�F���
static int text_color[10]
{
	0xffffff,
	0x000000,
	0x000000,
	0x00ffff,
	0xff00ff,
	0xffff00,
	0xff0000,
	0x00ff00,
	0x0000ff,
	0x00ff00,
};

//�A�j���[�V�����p
struct ANIM
{
	Location location;	//���W
	Erea erea;			//�傫��
	int time;			//�폜�܂ł̎���
	float angle;		//�i�s����
};
class Stage :
	public BoxCollider, public ColorData
{
private:
	int frame;					//�t���[������
	int type;					//���̎��(0=�� 1=�D 2=�� 3=�� 4=�� 5=�� 6=�� 7=��)
	bool inv_flg;				//����`�悵�Ȃ������f
	bool debug_flg;				//���̎�ނ�\�����邩
	int swap_flg;				//�F�̌�����o����u���b�N���ǂ���
	int anim;						//�A�j���[�V�����p
	ANIM effect_anim[ANIM_BLOCK_NUM];	//�A�j���[�V�����p���W�ۊ�
public:
	Stage(float _x, float _y, float _width, float _height, int _type);
	~Stage();
	void Update();
	void Draw()const;

	//��ނ�擾(0=�� 1=�D 2=�� 3=�� 4=�� 5=�� 6=�� 7=��)
	int GetStageCollisionType();
	//��ނ�ݒ�
	void SetStageType(int _type);
	//��ނ�擾
	int GetStageType() { return type; }
	//���̎�ނ�\������
	void SetDebugFlg() { debug_flg = true; }
};

