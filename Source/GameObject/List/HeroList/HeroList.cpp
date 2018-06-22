/*
	@file	HeroList.cpp
	@brief	出現勇者のリスト
	@detail	双方向リストを用いた動的リスト
	@author	番場 宥輝
*/
#include "HeroList.h"


/*
	@brief	コンストラクタ
*/
HeroList::HeroList()
{
}

/*
	@brief	デストラクタ
*/
HeroList::~HeroList()
{
	Destroy();
}

/*
	@brief	初期化
*/
void HeroList::Init()
{
	/*! インスタンス生成 */
	m_pHead = new HERO_NODE;
	m_pTail = new HERO_NODE;

	/*! 初期化 */
	m_pHead->data = m_pTail->data = NULL;	/*!< 先頭と末尾のノードはデータは持たない */
	m_pHead->pNext = m_pTail;				/*!< 先頭の次ポインタを末尾ノード */
	m_pHead->pPrev = NULL;					/*!< 先頭の前ポインタは存在しない */
	m_pTail->pNext = NULL;					/*!< 末尾の次ポインタは存在しない */
	m_pTail->pPrev = m_pHead;				/*!< 末尾の前ポインタを先頭ノード */

	HERO_NODE *p, *del;
	p = m_pHead->pNext;
	while (p != NULL&&p != m_pTail)
	{
		/*! ノードの初期化 */
		del = p;
		p = p->pNext;
		m_pHead->pNext = p->pNext;
		SAFE_DELETE(del);
	}
}

/*
	@brief	解放
*/
void HeroList::Destroy()
{

	HERO_NODE *p, *del;
	p = m_pHead->pNext;
	while (p != NULL&&p != m_pTail)
	{
		/*! ノードの初期化 */
		del = p;
		p = p->pNext;
		m_pHead->pNext = p->pNext;
		SAFE_DELETE(del);
	}
	SAFE_DELETE(m_pHead);
	SAFE_DELETE(m_pTail);
}

/*
	@brief	ノードの追加
*/
void HeroList::AddNode(Hero * hero)
{
	/*! リストの後ろへ挿入していく */
	HERO_NODE* p = new HERO_NODE;
	p->data = hero;
	/*! ノードの繋ぎ変え */
	p->pPrev = m_pTail->pPrev;	
	p->pPrev->pNext = p;		
	m_pTail->pPrev = p;			
	p->pNext = m_pTail;			
}

/*
	@brief	ノードの削除
*/
void HeroList::DeleteNode(HERO_NODE * node)
{
	/*! 前ノードの次ポインタを次ノードの前ポインタ */
	node->pPrev->pNext = node->pNext;
	/*! 次ノードの前ポインタを前ノードの次ポインタ */
	node->pNext->pPrev = node->pPrev;
	SAFE_DELETE(node);
}
