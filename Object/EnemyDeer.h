#pragma once
#include "Object.h"
#include <iostream>
#include <fstream>

using namespace std;

enum class DeerState {
	LEFT = 0,
	RIGHT,
	DEATH
};

class EnemyDeer :
	public Object
{
private:

	ifstream DeerIfs;
	ofstream DeerOfs;

	fstream Deerfs ;

	DeerState deer_state;

	int NotOpen;

	int KeepMouseX[10], KeepMouseY[10];
	int KeepCursor[99][8];
	bool EnemyFileOpenFlg;
	int EnemyFileHandle;

	char String[100];

	int number;
	int ArrayNumberX;	//Max 6 : 0 2 4 6 
	int ArrayNumberY;	//Max 7 : 1 3 5 7
	int ArrayNumber;	//Max 98
	
	int akusel;

	float DrawTest1, DrawTest2, DrawTest3, DrawTest4;
	int	DrawTest5, DrawTest6;

public:

	EnemyDeer();
	~EnemyDeer();
	void Initialize(Location _location, Erea _erea, int _color_data)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Location _location, Erea _erea, int _type, int _color_data)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void EnemyDeerMove(void);
	void EnemyDeerOfs(void);	
	void EnemyDeerIfs(void);	
	int EnemyDeerfs(void);		
};

