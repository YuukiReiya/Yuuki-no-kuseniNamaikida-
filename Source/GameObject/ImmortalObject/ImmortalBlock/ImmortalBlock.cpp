/*
	@file	ImmortalBlock.cpp
	@brief	破壊不能ブロック
	@author	番場 宥輝
*/
#include "ImmortalBlock.h"


/*
	@brief	コンストラクタ
*/
ImmortalBlock::ImmortalBlock()
{
}

/*
	@brief	デストラクタ
*/
ImmortalBlock::~ImmortalBlock()
{
	Destroy();
}
/*
	@brief	初期化
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
	@brief	レンダリング
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
	@brief	解放
*/
void ImmortalBlock::Destroy()
{
	for (int x = 0; x < 2; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++)
			SAFE_DELETE(m_pSprite[x][y]);
}
