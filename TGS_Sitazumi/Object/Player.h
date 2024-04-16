#pragma once
#include "Object.h"
#include "Stage.h"

class Player :
	public Object
{
public:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
private:
	Location vector; //�x�N�g���A���������L�����x�[�X�Ɉړ�����
	bool stageHitFlg[2][4]; //�㉺���E�ǂ��Ɠ������Ă��邩��z��Ɋi�[
							//0�̍s��1�̃u���b�N�ɓ������Ă��邩
							//1�̍s�̓X�e�[�W�S�̂ɑ΂��Ă̓������Ă��邩

	Location aimLoc; //�Ə��̍��W
	Location aimVec; //�Ə��̕���
	Location lineLoc; //���̏I�[���W
	
public:
	Player();
	~Player();
	void Update()override;
	void Draw()const override;
	void Hit(Location _location, Erea _erea, int _type)override;

	void MoveActor();
	void MoveAim();

	bool CheckCollision(Stage* stage);
};

