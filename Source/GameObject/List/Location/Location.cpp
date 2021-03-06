#include "Location.h"

/*
	@brief
*/
Location::Location()
{
}

/*
	@brief
*/
Location::~Location()
{
}

/*
	@brief	初期化
*/
void Location::Init()
{
	Clear();
}

/*
	@brief	解放
*/
void Location::Destroy()
{
	
}

/*
	@brief	クリア
*/
void Location::Clear()
{
	for (int x = 0; x < SOIL_WIDTH; x++) {
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			m_EnemyLocation[x][y].enemy.clear();
			m_EnemyLocation[x][y].enemy.shrink_to_fit();
		}
	}
}
