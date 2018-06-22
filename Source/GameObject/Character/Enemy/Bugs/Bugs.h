/*
	@file	Bugs.cpp
	@brief	íéóﬁ
	@author	î‘èÍ óGãP
*/
#pragma once
#include "../Enemy.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"
#include "../Moss/Moss.h"
#include "../Lizard/Lizard.h"

class Bugs :
	public Enemy
{
public:
	Bugs();
	~Bugs();

	void Init(INT2 Pos, INT2 Component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Enemy& enemy);
	void Collision(Moss& moss);
	void Collision(Bugs& bugs) {};
	void Collision(Lizard& lizard);
	void Collision(Soldier& solder) { isMove = false; }
private:
	void Growing();
	void Breeding();
	void IndexUpdate();
	void AnimUpdate();
	void Move();
	void StackPushDirection(D3DXVECTOR2 direction);
	void StackInit();
	int StackElementsNum();
	void LarvaeUpdate();
	void ChrysalisUpdate();
	void AdultUpdate();

	enum GROW {
		LARVAE,		//ócíé
		CHRYSALIS,	//Âå
		ADULT,		//ê¨íé
	};
	DWORD m_AtkCount;
	DWORD m_GrowCount;
	GROW m_Grow;
	bool isMove;
	D3DXVECTOR2 m_StackDirection[3];
	const int STACK_SIZE = sizeof(m_StackDirection) / sizeof(D3DXVECTOR2);
	const D3DXVECTOR2 INITIALIZE_STACK_ELEMENTS = { 0,0 };
	int m_StackIndex;

	const int MAX_HP_LARVE = 64;
	const int MAX_HP_ADULT = 76;
};

