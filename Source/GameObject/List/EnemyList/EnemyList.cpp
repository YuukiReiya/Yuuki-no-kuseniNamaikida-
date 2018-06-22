/*
	@file	EnemyList.cpp
	@brief	�o�������X�^�[�̃��X�g
	@detail	�o�������X�g��p�������I���X�g
	@author	�ԏ� �G�P
*/
#include "EnemyList.h"


/*
	@brief	�R���X�g���N�^
*/
EnemyList::EnemyList()
{
}

/*
	@brief	�f�X�g���N�^
*/
EnemyList::~EnemyList()
{
	Destroy();
}

/*
	@brief	������
*/
void EnemyList::Init()
{
	/*! �C���X�^���X���� */
	m_pHead = new ENEMY_NODE;
	m_pTail = new ENEMY_NODE;

	/*! ������ */
	m_pHead->data = m_pTail->data = NULL;	/*!< �擪�Ɩ����̃m�[�h�̓f�[�^�͎����Ȃ� */
	m_pHead->pNext = m_pTail;				/*!< �擪�̎��|�C���^�𖖔��m�[�h */
	m_pHead->pPrev = NULL;					/*!< �擪�̑O�|�C���^�͑��݂��Ȃ� */
	m_pTail->pNext = NULL;					/*!< �����̎��|�C���^�͑��݂��Ȃ� */
	m_pTail->pPrev = m_pHead;				/*!< �����̑O�|�C���^��擪�m�[�h */

	ENEMY_NODE *p, *del;
	p = m_pHead->pNext;
	while (p != NULL&&p != m_pTail)
	{
		/*! �m�[�h�̏����� */
		del = p;
		p = p->pNext;
		m_pHead->pNext = p->pNext;
		SAFE_DELETE(del);
	}
}

/*
	@brief	���
*/
void EnemyList::Destroy()
{
	ENEMY_NODE *p, *del;
	p = m_pHead->pNext;
	while (p != NULL&&p != m_pTail)
	{
		/*! �m�[�h�̏����� */
		del = p;
		p = p->pNext;
		m_pHead->pNext = p->pNext;
		SAFE_DELETE(del);
	}
	SAFE_DELETE(m_pHead);
	SAFE_DELETE(m_pTail);
}

/*
	@brief	�m�[�h�̒ǉ�
*/
void EnemyList::AddNode(Enemy * enemy)
{
	/*! ���X�g�̌��֑}�����Ă��� */
	ENEMY_NODE* p = new ENEMY_NODE;
	enemy->SetEnemyNode(p);
	p->data = enemy;

	/*! �m�[�h�̌q���ς� */
	p->pPrev = m_pTail->pPrev;
	p->pPrev->pNext = p;		
	m_pTail->pPrev = p;			
	p->pNext = m_pTail;			
}
