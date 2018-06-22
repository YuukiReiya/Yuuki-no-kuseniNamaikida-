/*
	@file	Terrain.h
	@brief	地形
	@author	番場 宥輝
*/
#include "Terrain.h"

/*
	@brief	コンストラクタ
*/
Terrain::Terrain()
{
}

/*
	@brief	デストラクタ
*/
Terrain::~Terrain()
{
	Destroy();
}

/*
	@brief	初期化
	@detail	養分をランダムで配置
*/
void Terrain::Init()
{
	for (int x = 0; x < SOIL_WIDTH; x++) {
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			D3DXVECTOR2 Pos{
				(float)GRID_SIZE + (float)GRID_SIZE*x,
				(float)GRID_SIZE*GROUND_HEIGHT + (float)GRID_SIZE*y
			};
			m_pSoils[x][y] = new Soil;
			m_pSoils[x][y]->m_Pos = Pos;
			m_pSoils[x][y]->Init();
			m_pSoils[x][y]->m_Component = INT2{ 0,0 };
		}
	}
	
	/*! 養分の設定 */
	for (int x = 0; x < SOIL_WIDTH; x++) {
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			int nut = rand() % (COMPONENT_LEVEL2 + 1);
			/*! 乱数が6未満のものは養分を0とする */
			if (nut < 6) { nut = 0; }
			m_pSoils[x][y]->m_Component = INT2(nut, 0);
		}
	}
}

/*
	@brief	解放
*/
void Terrain::Destroy()
{
	for (int x = 0; x < SOIL_WIDTH; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++)
			SAFE_DELETE(m_pSoils[x][y]);
}
