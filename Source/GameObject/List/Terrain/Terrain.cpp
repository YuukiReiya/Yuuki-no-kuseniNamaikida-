/*
	@file	Terrain.h
	@brief	�n�`
	@author	�ԏ� �G�P
*/
#include "Terrain.h"

/*
	@brief	�R���X�g���N�^
*/
Terrain::Terrain()
{
}

/*
	@brief	�f�X�g���N�^
*/
Terrain::~Terrain()
{
	Destroy();
}

/*
	@brief	������
	@detail	�{���������_���Ŕz�u
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
	
	/*! �{���̐ݒ� */
	for (int x = 0; x < SOIL_WIDTH; x++) {
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			int nut = rand() % (COMPONENT_LEVEL2 + 1);
			/*! ������6�����̂��̂͗{����0�Ƃ��� */
			if (nut < 6) { nut = 0; }
			m_pSoils[x][y]->m_Component = INT2(nut, 0);
		}
	}
}

/*
	@brief	���
*/
void Terrain::Destroy()
{
	for (int x = 0; x < SOIL_WIDTH; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++)
			SAFE_DELETE(m_pSoils[x][y]);
}
