/*
	@file	Moss.h
	@brief	�R�P
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"
#include "../../../Manager/HeroManager/HeroManager.h"
#include "../Buds/Buds.h"
#include "../Larvae/Larvae.h"
#include "../Adult/Adult.h"

#include "../../../../Debug/Debug.h"

/*
	@brief	�R�P�N���X
*/	
class Moss :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Moss();
	/*! �f�X�g���N�^ */
	~Moss();

	void Init(INT2 pos,INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Larvae& larvae);
	void Collision(Adult& adult);
	void Collision(Soldier& solder);
private:
	bool Wait();
	void Attack();
	void IndexUpdate();
	void AnimUpdate();
	bool Move();
	bool CheckAbsorption();
	void Absorption();
	bool CheckDivergence();
	void Divergence();	
	void OutComponent();
	bool CheckGrow();
	void Grow();
	D3DXVECTOR2 CalculateDirection();

	/*
		@enum	STATE
		@brief	��Ԃ�\��
	*/
	enum STATE {
		ASSIGN,	/*!< ���蓖�� */
		MOVE,	/*!< �ړ��� */
		ATK,	/*!< �U���� */
		WAIT,	/*!< �ҋ@�� */
		ACTION,	/*!< �A�N�V���� */
	}state;

	/*! �萔 */
	static const unsigned int GROW_COMPONENT;			/*!< �����ɕK�v�ȗ{�� */
	static const unsigned int GROW_HP;					/*!< �����ɕK�v��HP */
	static const unsigned int RECOVERY_POWER;			/*!< �񕜗� */
	static const unsigned int MAX_HP;					/*!< �ő�HP */
	static const unsigned int WAIT_COUNT;				/*!< �ҋ@�J�E���g */

	/*! �ϐ� */
	bool isMove;
	bool isAgain;
	bool isAbsorp;/*!< �z���t���O */
	DWORD m_Count;/*!< �s���J�E���^ */
	int m_AbsorptionPow;/*!< �{���z���� */

	Debug m_Debug;
};

