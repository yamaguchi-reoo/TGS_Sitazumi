#pragma once

class GameMain;

class EffectSpawner
{
private:
	int frame;		//フレーム測定
	int spawn_order;	//GameMainからエフェクトのスポーン指示を受け取る
public:
	EffectSpawner();
	~EffectSpawner();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();
};

