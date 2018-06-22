/*
	@file	HeroList.h
	@brief	出現勇者のリスト
	@detail	双方向リストを用いた動的リスト
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Hero/Hero.h"

/*
	@struct	HERO_NODE
	@brief	勇者のノード
*/
struct HERO_NODE
{
	HERO_NODE* pNext;	/*!< 次ポインタ */
	HERO_NODE* pPrev;	/*!< 前ポインタ */

	Hero*		data;	/*!< 勇者 */

	/*! デストラクタ */
	~HERO_NODE() { SAFE_DELETE(data); }
};

/*
	@brief	勇者の双方向リストのクラス
*/
class HeroList
{
public:
	/*! コンストラクタ */
	HeroList();
	/*! デストラクタ */
	~HeroList();

	void Init();
	void Destroy();
	void AddNode(Hero* hero);
	void DeleteNode(HERO_NODE* node);
	
	HERO_NODE* m_pHead;		/*!< 先頭ノード */
	HERO_NODE* m_pTail;		/*!< 末尾ノード */
};

