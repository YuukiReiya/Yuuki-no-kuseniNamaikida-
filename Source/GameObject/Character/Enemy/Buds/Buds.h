/*
	@file	Buds.h
	@brief	�Q
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Flower/Flower.h"

/*
	@brief	�Q�N���X
*/
class Buds :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Buds();
	/*! �f�X�g���N�^ */
	~Buds();

	/*! �֐� */
	void Init(INT2 pos, INT2 component);
	void Init(INT2 pos, INT2 component, int hp);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Adult& adult);
private:
	bool Absorption();
	void Grow();
	void OutComponent();

	/*! �萔 */
	static const unsigned int ACTION_COUNT;				/*!< �s���J�E���g */
	static const unsigned int ABSORPTION_OVER_LIMIT;	/*!< �{���z���\��� */
	static const unsigned int ABSORPTION_UNDER_LIMIT;	/*!< �{���z���\���� */
	static const unsigned int GROW_COMPONENT;			/*!< �����ɕK�v�ȗ{�� */

	/*! �ϐ� */
	DWORD m_Count;
	Debug m_Debug;
};

