/*
	@file	Chrysalis.h
	@brief	�
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Adult/Adult.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"

/*
	@brief	�
*/
class Chrysalis :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Chrysalis();
	/*! �f�X�g���N�^ */
	~Chrysalis();

	/*! �֐� */
	void Init(INT2 pos, INT2 component);
	void Init(INT2 pos, INT2 component, int hp);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Lizard& lizard);
private:
	void Grow();
	void OutComponent();

	/*! �萔 */
	static const unsigned int ACTION_COUNT;				/*!< �s���J�E���g */
	static const unsigned int GROW_COUNT;				/*!< �����ɕK�v�Ȏ��� */


	/*! �ϐ� */
	DWORD m_Count;
	Debug m_Debug;

};

