/*
	@file	HeroList.h
	@brief	�o���E�҂̃��X�g
	@detail	�o�������X�g��p�������I���X�g
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Hero/Hero.h"

/*
	@struct	HERO_NODE
	@brief	�E�҂̃m�[�h
*/
struct HERO_NODE
{
	HERO_NODE* pNext;	/*!< ���|�C���^ */
	HERO_NODE* pPrev;	/*!< �O�|�C���^ */

	Hero*		data;	/*!< �E�� */

	/*! �f�X�g���N�^ */
	~HERO_NODE() { SAFE_DELETE(data); }
};

/*
	@brief	�E�҂̑o�������X�g�̃N���X
*/
class HeroList
{
public:
	/*! �R���X�g���N�^ */
	HeroList();
	/*! �f�X�g���N�^ */
	~HeroList();

	void Init();
	void Destroy();
	void AddNode(Hero* hero);
	void DeleteNode(HERO_NODE* node);
	
	HERO_NODE* m_pHead;		/*!< �擪�m�[�h */
	HERO_NODE* m_pTail;		/*!< �����m�[�h */
};

