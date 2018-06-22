/*
	@file	Flower.h	
	@brief	��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Moss/Moss.h"

/*
	�ԃN���X
*/
class Flower :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Flower();
	/*! �f�X�g���N�^ */
	~Flower();

	/*! �֐� */
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

	/*! �萔 */
	static const unsigned int ACTION_COUNT;				/*!< �s���J�E���g */
	static const unsigned int ABSORPTION_POWER;			/*!< �{���z���� */
	static const unsigned int ABSORPTION_OVER_LIMIT;	/*!< �{���z���\��� */
	static const unsigned int ABSORPTION_UNDER_LIMIT;	/*!< �{���z���\���� */
	static const unsigned int BREEDING_COMPONENT;		/*!< �ɐB�ɕK�v�ȗ{�� */
	static const unsigned int BREEDING_COUNT;			/*!< �ɐB�ɕK�v�ȋz���� */
	static const unsigned int MAX_HP;					/*!< HP�ő�l */


	/*! �ϐ� */
	DWORD m_Count;						/*!< �s���J�E���^ */
	int m_AbsorptionCount;				/*!< �z���� */
	Debug m_Debug;
};

