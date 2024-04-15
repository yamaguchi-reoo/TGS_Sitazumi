#pragma once
#include "../Object/CharaBase.h"
#include <iostream>
#include <fstream>

using namespace std;

class EnemyDeer :
	public CharaBase
{
private:

	ifstream DeerIfs;
	ofstream DeerOfs;

	fstream Deerfs ;

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

public:

	EnemyDeer();
	~EnemyDeer();
	void Update(GameMain* main)override;
	void Draw()const override;

	void EnemyDeerOfs(void);	
	void EnemyDeerIfs(void);	
	int EnemyDeerfs(void);		
};

