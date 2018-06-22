/*
	@file	CollisionManager.cpp
	@brief	オブジェクトの衝突を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "CollisionManager.h"


/*
	@brief	コンストラクタ
*/
CollisionManager::CollisionManager()
{
}

/*
	@brief	デストラクタ
*/
CollisionManager::~CollisionManager()
{
}

/*
	@brief	更新
*/
void CollisionManager::Update()
{
	if (m_List.empty()) { return; }
	/*! リストに登録された衝突を順に呼び出す */
	for (auto itr : m_List) {
		itr.A->Collision(*itr.B);
	}
	
}

/*
	@brief	初期化
*/
void CollisionManager::Init()
{
	Clear();
}

/*
	@brief	衝突リストへ追加
*/
void CollisionManager::Add(Character & A, Character & B)
{
	CollisionInfo info{
		&A,&B
	};
	m_List.push_back(info);
}

/*
	@brief	リストを空にする
*/
void CollisionManager::Clear()
{
	if (!m_List.empty()) {
		m_List.clear();			/*!< 要素の解放 */
		m_List.shrink_to_fit();	/*!< キャパの解放 */
	}
}
