/*
	@file	HeroList.cpp
	@brief	�o���E�҂̃��X�g
	@detail	�o�������X�g��p�������I���X�g
	@author	�ԏ� �G�P
*/
#include "HeroList.h"


/*
	@brief	�R���X�g���N�^
*/
HeroList::HeroList()
{
}

/*
	@brief	�f�X�g���N�^
*/
HeroList::~HeroList()
{
	Destroy();
}

/*
	@brief	������
*/
void HeroList::Init()
{
	/*! �C���X�^���X���� */
	m_pHead = new HERO_NODE;
	m_pTail = new HERO_NODE;

	/*! ������ */
	m_pHead->data = m_pTail->data = NULL;	/*!< �擪�Ɩ����̃m�[�h�̓f�[�^�͎����Ȃ� */
	m_pHead->pNext = m_pTail;				/*!< �擪�̎��|�C���^�𖖔��m�[�h */
	m_pHead->pPrev = NULL;					/*!< �擪�̑O�|�C���^�͑��݂��Ȃ� */
	m_pTail->pNext = NULL;					/*!< �����̎��|�C���^�͑��݂��Ȃ� */
	m_pTail->pPrev = m_pHead;				/*!< �����̑O�|�C���^��擪�m�[�h */

	HERO_NODE *p, *del;
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
void HeroList::Destroy()
{

	HERO_NODE *p, *del;
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
void HeroList::AddNode(Hero * hero)
{
	/*! ���X�g�̌��֑}�����Ă��� */
	HERO_NODE* p = new HERO_NODE;
	p->data = hero;
	/*! �m�[�h�̌q���ς� */
	p->pPrev = m_pTail->pPrev;	
	p->pPrev->pNext = p;		
	m_pTail->pPrev = p;			
	p->pNext = m_pTail;			
}

/*
	@brief	�m�[�h�̍폜
*/
void HeroList::DeleteNode(HERO_NODE * node)
{
	/*! �O�m�[�h�̎��|�C���^�����m�[�h�̑O�|�C���^ */
	node->pPrev->pNext = node->pNext;
	/*! ���m�[�h�̑O�|�C���^��O�m�[�h�̎��|�C���^ */
	node->pNext->pPrev = node->pPrev;
	SAFE_DELETE(node);
}
