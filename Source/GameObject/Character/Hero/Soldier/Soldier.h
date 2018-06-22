/*
	@file	Soldier.h
	@brief	���m
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Hero.h"
#include "../../../../Algorithm/AStar/AStar.h"
#include "../../Enemy/Enemy.h"
#include "../../Enemy/Moss/Moss.h"
#include "../../Enemy/Bugs/Bugs.h"
#include "../../Enemy/Lizard/Lizard.h"

/*
	@brief	���m�N���X
*/
class Soldier :
	public Hero
{
public:
	/*!�R���X�g���N�^ */
	Soldier();
	/*!�f�X�g���N�^ */
	~Soldier();

	void Init();
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void IndexUpdate();
	void Catch();
	void Damage(int damage);

	/*! �Փ� */
	void Collision(Character& character);
	void Collision(Moss& moss)				{ Battle(moss); }
	void Collision(Buds& buds)				{ Battle(buds); }
	void Collision(Flower& flower)			{ Battle(flower); }
	void Collision(Larvae& larvae)			{ Battle(larvae); }
	void Collision(Chrysalis& chrysalis)	{ Battle(chrysalis); }
	void Collision(Adult& adult)			{ Battle(adult); }
	void Collision(Egg& egg)				{ Battle(egg); }
	void Collision(Lizard& lizard)			{ Battle(lizard); }
private: 
	void Battle(Enemy& enemy);
	void AnimUpdate();
	bool Move();
	bool Wait();

	/*! �萔 */
	static const unsigned int WAIT_COUNT;

	/*
		@enum	STATE
		@brief	��Ԃ�\��
	*/
	enum STATE {
		ASSIGN,	/*!< ���蓖�� */
		MOVE,	/*!< �ړ��� */
		ATK,	/*!< �U���� */
		WAIT,	/*!< �ҋ@�� */
		CATCH,	/*!< �ߔ� */
	}state;


	/*! �ϐ� */
	AStar aStar;			/*!< A* */
	Enemy* m_pEnemy;		/*!< �U���Ώۃ����X�^�[ */
	INT2 m_TargetIndexPos;	/*!< �ړ��ڕW�n�_ */
	DWORD m_Count;			/*!< �s���J�E���^ */
	Debug m_Debug;			/*!< �f�o�b�O */
};
