/*
	@file	ImmortalBlock.cpp
	@brief	�j��s�\�u���b�N
	@author	�ԏ� �G�P
*/
#include "ImmortalBlock.h"


/*
	@brief	�R���X�g���N�^
*/
ImmortalBlock::ImmortalBlock()
{
}

/*
	@brief	�f�X�g���N�^
*/
ImmortalBlock::~ImmortalBlock()
{
	Destroy();
}
/*
	@brief	������
*/
void ImmortalBlock::Init()
{
	for (int x = 0; x < 2; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			m_pSprite[x][y] = new Sprite;
			m_pSprite[x][y]->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
			//m_pSprite[x][y]->SetSplit(6, 3);
		}

}

/*
	@brief	�����_�����O
*/
void ImmortalBlock::Render(IDirect3DTexture9 * texture)
{
	for (int x = 0; x < 2; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			D3DXVECTOR2 Pos{
				(float)(x*GRID_SIZE*(SOIL_WIDTH+1)),
				(float)(GRID_SIZE*GROUND_HEIGHT + y*GRID_SIZE)
			};
			m_pSprite[x][y]->SetPos(ConvertView(Pos));
			m_pSprite[x][y]->RenderAtlasLeftTop(texture,0,0);
		}
}

/*
	@brief	���
*/
void ImmortalBlock::Destroy()
{
	for (int x = 0; x < 2; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++)
			SAFE_DELETE(m_pSprite[x][y]);
}
