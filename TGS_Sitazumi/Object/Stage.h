#pragma once
#include "BoxCollider.h"
class Stage :
	public BoxCollider
{
private:
	int type;		//” ‚Ìí—Ş(0=–³ 1=’n–Ê 2=–Ø 3=Šâ 4=‰_)
	bool inv_flg;		//‰½‚à•`‰æ‚µ‚È‚¢‚©”»’f
	int stage_img[2];	//‰æ‘œŠi”[—p
	bool end_flg[4];	//’[‚©‚Ç‚¤‚©”»’f
	Location draw_location[4];	//Še’[‚Ì•`‰æˆÊ’u
	float draw_rad[4];	//Še’[‚Ì•`‰æŠp“x
	float draw_angle;			//Še’[‚Ì•`‰æŠp“x
	bool debug_flg;		//” ‚Ìí—Ş‚ğ•\¦‚·‚é‚©
public:
	Stage(float _x, float _y, float _width, float _height, int _type);
	~Stage();
	void Update();
	void Draw()const;

	//í—Ş‚ğæ“¾(0=–³ 1=’n–Ê 2=–Ø 3=Šâ 4=‰_)
	int GetStageCollisionType();
	//í—Ş‚ğİ’è
	void SetStageType(int _type) { type = _type; }
	//í—Ş‚ğæ“¾
	int GetStageType() { return type; }
	//” ‚Ìí—Ş‚ğ•\¦‚·‚é
	void SetDebugFlg() { debug_flg = true; }
	//•`‰æ‚·‚é‰æ‘œ‚Ìí—Ş‚ğ”»’f‚·‚é
	void SetDrawType(int up_type, int left_type, int right_type, int down_type);
};

