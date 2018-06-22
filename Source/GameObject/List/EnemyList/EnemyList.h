/*
	@file	EnemyList.h
	@brief	�o�������X�^�[�̃��X�g
	@detail	�o�������X�g��p�������I���X�g
	@author	�ԏ� �G�P
*/
#pragma once
#include"../../Character/Enemy/Enemy.h"

/*
	@struct ENEMY_NODE
	@brief	�����X�^�[�̃m�[�h
*/
struct ENEMY_NODE
{
	ENEMY_NODE* pNext;	/*!< ���|�C���^ */
	ENEMY_NODE* pPrev;	/*!< �O�|�C���^ */

	Enemy*		data;	/*!< �����X�^�[ */

	/*! �f�X�g���N�^ */
	~ENEMY_NODE() { SAFE_DELETE(data); }
};

/*
	@brief	�����X�^�[�̑o�������X�g�̃N���X
*/
class EnemyList
{
public:
	/*! �R���X�g���N�^ */
	EnemyList();
	/*! �f�X�g���N�^ */
	~EnemyList();

	void Init();	
	void Destroy();	
	void AddNode(Enemy* enemy);
	
	ENEMY_NODE* m_pHead;	/*!< �擪�m�[�h */
	ENEMY_NODE* m_pTail;	/*!< �����m�[�h */
};
