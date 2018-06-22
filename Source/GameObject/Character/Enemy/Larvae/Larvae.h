/*
	@file	Larvae.h
	@brief	�c��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Chrysalis/Chrysalis.h"
#include "../Moss/Moss.h"
#include "../Lizard/Lizard.h"
#include "../../../../Debug/Debug.h"

/*
	@brief	�c��
*/
class Larvae :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Larvae();
	/*! �f�X�g���N�^ */
	~Larvae();

	/*! �֐� */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Moss& moss);
	void Collision(Lizard& lizard);
	void Collision(Soldier& soldier);
private:
	void IndexUpdate();
	void OutComponent();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckGrow();
	void Grow();
	void Predator(Moss&moss);
	void StackInit();
	void StackPushDirection(D3DXVECTOR2 direction);
	void AnimUpdate();
	int StackElementsNum();

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
	static const unsigned int MAX_HP;					/*!< �ő�HP */
	static const unsigned int POWER;					/*!< �U���� */
	static const unsigned int WAIT_COUNT;				/*!< �ҋ@�J�E���g */
	static const unsigned int GROW_COMPONENT;			/*!< �����ɕK�v�ȗ{�� */
	static const unsigned int GROW_HP;					/*!< �����ɕK�v��HP */
	static const unsigned int STACK_SIZE;				/*!< �X�^�b�N�̃T�C�Y */
	static const D3DXVECTOR2 INITIALIZE_STACK_ELEMENTS;	/* �X�^�b�N�̗v�f�̏����l */

	/*! �ϐ� */
	bool isMove;					/*!< �ړ��t���O */
	D3DXVECTOR2 m_StackDirection[4];/*!< �i�H��� */
	int m_StackIndex;				/*!< �X�^�b�N�̃C���f�b�N�X */
	DWORD m_Count;					/*!< �U�����s���܂ł̃J�E���^ */

	Debug m_Debug;
};

