/*
	@file	Lizard.h
	@brief	�g�J�Q
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Larvae/Larvae.h"
#include "../Chrysalis/Chrysalis.h"
#include "../Adult/Adult.h"
#include "../Egg/Egg.h"
#include "../../../../Algorithm/AStar/AStar.h"

/*
	@brief	�g�J�Q�N���X
*/
class Lizard :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Lizard();
	/*! �f�X�g���N�^ */
	~Lizard();

	void Init(INT2 Pos,INT2 Component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Larvae& larvae);
	void Collision(Chrysalis& chrysalis);
	void Collision(Adult& adult);
	void Collision(Soldier& solder);
private:
	bool CheckLay();
	void Lay();
	void IndexUpdate();
	void AnimUpdate();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckMakeNest();
	void MakeNest();
	void OutComponent();
	D3DXVECTOR2 CalculateDirection();
	void StraightMoveIndexUpdate();
	void NestMoveIndexUpdate();
	void StackPushDirection(D3DXVECTOR2 direction);
	void StackInit();
	int StackElementsNum();

	/*! �萔 */
	static const unsigned int MAX_HP;					/*!< �ő�HP */
	static const unsigned int WAIT_COUNT;				/*!< �U���܂ł̃J�E���g */
	static const unsigned int MAKE_NEST_COMPONENT;		/*!< ������邽�߂̕K�v�{�� */
	static const unsigned int MADE_NEST_HP;				/*!< ������邽�߂�HP */
	static const unsigned int LAY_EGG_COMPONENT;		/*!< �����Y�ނ��߂̕K�v�{�� */
	static const unsigned int LAY_EGG_HP;				/*!< �����Y�ނ��߂�HP */
	static const unsigned int STACK_SIZE;				/*!< �X�^�b�N�̃T�C�Y */
	static const D3DXVECTOR2 INITIALIZE_STACK_ELEMENTS;	/* �X�^�b�N�̗v�f�̏����l */

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

	AStar m_aStar;
	bool isMove;
	bool isStraight;//���i�t���O
	bool isMadeNest;//������������ǂ����̃t���O
	INT2 m_NestIndexPos;/*!< ���̍��W */
	INT2 m_GoalIndexPos;/*!< �ړ��ڕW�n�_ */
	D3DXVECTOR2 m_StackDirection[3];
	int m_StackIndex;
	DWORD m_Count;
};

