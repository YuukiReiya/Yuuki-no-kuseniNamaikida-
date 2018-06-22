/*
	@file	EnemyList.h
	@brief	出現モンスターのリスト
	@detail	双方向リストを用いた動的リスト
	@author	番場 宥輝
*/
#pragma once
#include"../../Character/Enemy/Enemy.h"

/*
	@struct ENEMY_NODE
	@brief	モンスターのノード
*/
struct ENEMY_NODE
{
	ENEMY_NODE* pNext;	/*!< 次ポインタ */
	ENEMY_NODE* pPrev;	/*!< 前ポインタ */

	Enemy*		data;	/*!< モンスター */

	/*! デストラクタ */
	~ENEMY_NODE() { SAFE_DELETE(data); }
};

/*
	@brief	モンスターの双方向リストのクラス
*/
class EnemyList
{
public:
	/*! コンストラクタ */
	EnemyList();
	/*! デストラクタ */
	~EnemyList();

	void Init();	
	void Destroy();	
	void AddNode(Enemy* enemy);
	
	ENEMY_NODE* m_pHead;	/*!< 先頭ノード */
	ENEMY_NODE* m_pTail;	/*!< 末尾ノード */
};
