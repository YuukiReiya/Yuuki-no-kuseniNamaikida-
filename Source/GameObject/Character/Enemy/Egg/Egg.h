/*
	@file	Egg.h
	@brief	��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Lizard/Lizard.h"

/*
	@brief	���N���X
*/
class Egg :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Egg();
	/*! �f�X�g���N�^ */
	~Egg();

	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);

private:
	void Hatch();

	/*! �萔 */
	static const unsigned int HATCH_COUNT;				/*!< �z���ɕK�v�Ȏ��� */
	
	/*! �ϐ� */
	DWORD m_Count;
	Debug m_Debug;
};

