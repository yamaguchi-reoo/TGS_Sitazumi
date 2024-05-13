#pragma once
#include "../Utility/Vector2D.h"

#define GRAVITY 0.5f;

class Bone
{
private:
	Vector2D BoneLoc[2];		//スタート地点　中心地点を0、0としたローカル座標 ここを中心とする
	Vector2D BoneLocBase[2];		//boneの始めの座標
	//Vector2D goalLoc;		//ゴール地点
	Vector2D centerLoc;		//回転の中心地点　ここを中心とする 中心を変える必要が出てきそうならこれで作る
	
	//Vector2D movedLoc[2];
	float movedAng[32];	//移動後の角度
	float angle;		//今の角度
	float oneFrameAng[32];	//一フレームどれだけ移動するか
	int time[32];		//移動するまでにかかる時間(フレーム)
	int num;		//何かい移動するか
	bool flg;		//移動するかどうか

	int order; //何番目のボーンか
	bool side; //左右どちらに動くか

	bool selected;
	int cursor;
public:
	Bone();
	~Bone();

	void Initialize(Vector2D sl, Vector2D gl);

	void Update();
	void Draw() const;

	void SelectUpdate();
	void DrawUI() const;

	Vector2D GetCenterLocation() {
		return centerLoc;
	}

	Vector2D GetLocation(int n) {
		return BoneLoc[n] + centerLoc;
	}

	bool GetSelectedFlg() const {
		return selected;
	}

	//移動にかかる時間と角度
	void SetMoved(float ang, int t, int n, bool f);

	void SetCenterLocation(Vector2D center) {
		centerLoc = center;
	}
};

