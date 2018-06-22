/*
	@file	Adult.h
	@brief	����
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Enemy.h"
#include "../Moss/Moss.h"
#include "../Buds/Buds.h"
#include "../Flower/Flower.h"

/*
	@brief	����
*/
class Adult :
	public Enemy
{
public:
	/*! �R���X�g���N�^ */
	Adult();
	/*! �f�X�g���N�^ */
	~Adult();

	/*! �֐� */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Moss& moss);
	void Collision(Buds& buds);
	void Collision(Flower& flower);
	void Collision(Lizard& lizard);
	void Collision(Soldier& soldier);
private:
	void Predator(Moss& moss);
	void Predator(Buds& buds);
	void Predator(Flower& flower);
	void Predator(Larvae& larvae);
	void IndexUpdate();
	void OutComponent();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckBreeding();
	void Breeding();
	void StackInit();
	void StackPushDirection(D3DXVECTOR2 direction);
	int StackElementsNum();
	void AnimUpdate();


	/*! �萔 */
	static const unsigned int MAX_HP;					/*!< �ő�HP */
	static const unsigned int BREEDING_HP;				/*!< �ɐB�ɕK�v��HP */
	static const unsigned int WAIT_COUNT;				/*!< �ҋ@�J�E���g */
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

	/*! �ϐ� */
	bool isMove;					/*!< �ړ��t���O */
	D3DXVECTOR2 m_StackDirection[4];/*!< �i�H��� */
	int m_StackIndex;				/*!< �X�^�b�N�̃C���f�b�N�X */
	DWORD m_Count;					/*!< �s���J�E���^ */


	Debug m_Debug;
};

