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
	@brief	������
*/
void Location::Init()
{
	Clear();
}

/*
	@brief	���
*/
void Location::Destroy()
{
	
}

/*
	@brief	�N���A
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
