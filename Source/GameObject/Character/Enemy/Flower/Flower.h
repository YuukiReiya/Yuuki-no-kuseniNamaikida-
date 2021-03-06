/*
	@file	Flower.h	
	@brief	Ô
	@author	Ôê GP
*/
#pragma once
#include "../Enemy.h"
#include "../Moss/Moss.h"

/*
	ÔNX
*/
class Flower :
	public Enemy
{
public:
	/*! RXgN^ */
	Flower();
	/*! fXgN^ */
	~Flower();

	/*! Ö */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Adult& adult);
private:
	bool Absorption();
	void Breeding();
	void OutComponent();

	/*! è */
	static const unsigned int ACTION_COUNT;				/*!< s®JEg */
	static const unsigned int ABSORPTION_POWER;			/*!< {ªzûÊ */
	static const unsigned int ABSORPTION_OVER_LIMIT;	/*!< {ªzûÂ\ãÀ */
	static const unsigned int ABSORPTION_UNDER_LIMIT;	/*!< {ªzûÂ\ºÀ */
	static const unsigned int BREEDING_COMPONENT;		/*!< ÉBÉKvÈ{ª */
	static const unsigned int BREEDING_COUNT;			/*!< ÉBÉKvÈzûñ */
	static const unsigned int MAX_HP;					/*!< HPÅål */


	/*! Ï */
	DWORD m_Count;						/*!< s®JE^ */
	int m_AbsorptionCount;				/*!< zûñ */
	Debug m_Debug;
};

