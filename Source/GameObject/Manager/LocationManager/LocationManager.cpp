/*
	@file	LocationManager.cpp
	@brief	キャラの居場所を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "LocationManager.h"
#include "../TerrainManager/TerrainManager.h"

/*
	@brief	コンストラクタ
*/
LocationManager::LocationManager()
{
}

/*
	@brief	デストラクタ
*/
LocationManager::~LocationManager()
{
	Destroy();
}

/*
	@brief	初期化
*/
void LocationManager::Init()
{
	m_pLocation = new Location;

	m_pLocation->Init();
}

/*
	@brief	解放
*/
void LocationManager::Destroy()
{
	SAFE_DELETE(m_pLocation);
}

/*
	@brief	動的配列からデータを削除
*/
bool LocationManager::DeleteData(INT2 pos, Enemy* enemy)
{
	/*! イテレータ */
	auto itr = m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.begin();
	/*!  */
	while (itr != m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.end()) {
		if (enemy == *itr) { 
			m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.erase(itr);
			return true;
		}
		itr++;
	}
	return false;
}

/*
	@brief	最前面にいるインデックスのキャラクターを返す
	@param(pos)	判定インデックス
	@return	キャラクターのポインタ ※いなかったらNULLを返す
*/
Enemy * LocationManager::GetTopFront(INT2 pos)
{
	if (m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.empty()) { return NULL; }
	return *m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.begin();
}

/*
	@brief	最背面にいるインデックスのキャラクターdを返す
	@param(pos)	判定インデックス
	@return	キャラクターのポインタ ※いなかったらNULLを返す
*/
Enemy * LocationManager::GetTopBack(INT2 pos)
{
	if (m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.empty()) { return NULL; }
	return *m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.end();
}

/*
	@brief	キャラクターのデータを返す
	@param(pos)	判定インデックス
	@param(*character)	探索対象のキャラクター
	@return	キャラクターのポインタ ※いなかったらNULLを返す
	@detail	引数のキャラクターのインスタンスが動的配列に存在していればその
*/

/*
	@brief	キャラクターの情報をセットする
*/
void LocationManager::SetData(INT2 location, Enemy* enemy)
{
	m_pLocation->m_EnemyLocation[location.x][location.y].enemy.push_back(enemy);
}
