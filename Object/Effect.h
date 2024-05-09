#pragma once
#include "Object.h"
class Effect :
	public Object
{
private:
	int frame;			//測定用
	int  effect_type;	//エフェクトの種類
	bool touch_ground;	//地面に触れたか
	float angle;		//移動方向
	int effect_time;	//消えるまでの時間測定用
public:
	//_effect_type=エフェクトの種類 _angle=移動する向き _time=消えるまでのフレーム数
	Effect(int _effect_type,float _angle,int _time);
	~Effect();
	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;
	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}
};