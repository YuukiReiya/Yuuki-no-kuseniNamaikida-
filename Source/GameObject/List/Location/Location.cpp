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
	@brief	‰Šú‰»
*/
void Location::Init()
{
	Clear();
}

/*
	@brief	‰ğ•ú
*/
void Location::Destroy()
{
	
}

/*
	@brief	ƒNƒŠƒA
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
