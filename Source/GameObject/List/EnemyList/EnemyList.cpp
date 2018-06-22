/*
	@file	EnemyList.cpp
	@brief	出現モンスターのリスト
	@detail	双方向リストを用いた動的リスト
	@author	番場 宥輝
*/
#include "EnemyList.h"


/*
	@brief	コンストラクタ
*/
EnemyList::EnemyList()
{
}

/*
	@brief	デストラクタ
*/
EnemyList::~EnemyList()
{
	Destroy();
}

/*
	@brief	初期化
*/
void EnemyList::Init()
{
	/*! インスタンス生成 */
	m_pHead = new ENEMY_NODE;
	m_pTail = new ENEMY_NODE;

	/*! 初期化 */
	m_pHead->data = m_pTail->data = NULL;	/*!< 先頭と末尾のノードはデータは持たない */
	m_pHead->pNext = m_pTail;				/*!< 先頭の次ポインタを末尾ノード */
	m_pHead->pPrev = NULL;					/*!< 先頭の前ポインタは存在しない */
	m_pTail->pNext = NULL;					/*!< 末尾の次ポインタは存在しない */
	m_pTail->pPrev = m_pHead;				/*!< 末尾の前ポインタを先頭ノード */

	ENEMY_NODE *p, *del;
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
void EnemyList::Destroy()
{
	ENEMY_NODE *p, *del;
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
void EnemyList::AddNode(Enemy * enemy)
{
	/*! リストの後ろへ挿入していく */
	ENEMY_NODE* p = new ENEMY_NODE;
	enemy->SetEnemyNode(p);
	p->data = enemy;

	/*! ノードの繋ぎ変え */
	p->pPrev = m_pTail->pPrev;
	p->pPrev->pNext = p;		
	m_pTail->pPrev = p;			
	p->pNext = m_pTail;			
}
