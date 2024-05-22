#include"BoxCollider.h" 

class BackGround
{
private:
	int bg_width;		//背景の幅
	int stage_width;	//ステージの幅
public:
	BackGround();
	~BackGround();

	void Initialize(int _stage_wigth);
	void Finalize();
	void Update();
	void Draw(Location _camera_location)const;
};