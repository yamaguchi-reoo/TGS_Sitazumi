#include"BoxCollider.h" 

class BackGround
{
private:
	Erea bg_erea;		//背景の幅
	Erea stage_erea;	//ステージの幅
public:
	BackGround();
	~BackGround();

	void Initialize(Erea _stage_erea);
	void Finalize();
	void Update();
	void Draw(Location _camera_location)const;
	void DrawWood(Location _shift_location, float move_speed)const;
	void DrawMountain(Location _shift_location, float move_speed)const;
};