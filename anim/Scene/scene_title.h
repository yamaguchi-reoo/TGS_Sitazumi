#pragma once
#include "scene.h"
#include "../Object/Player.h"

class Title :public Scene
{
private:
	Vector2D location[32]; //線の座標偶数が始点、奇数が終点
	int num = 0;
	float angle = 0;

public:
	//�R���X�g���N�^
	Title();
	~Title();
	//�X�V
	Scene* update() override;
	//�`��
	void draw() const override;
};