/*
	@file	Soil.cpp
	@brief	土
	@author	番場 宥輝
*/
#include "Soil.h"
#include "../../Camera/Camera2D.h"

/*
	@brief	コンストラクタ
*/
Soil::Soil()
{
}

/*
	@brief	デストラクタ
*/
Soil::~Soil()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	初期化
*/
void Soil::Init()
{
	m_pSprite = new Sprite;

	m_isBreaked = false;/*!< 最初は壊れてない */

	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);/*!< スプライトサイズ */
}

/*
	@brief	レンダリング
*/
void Soil::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderLeftTop(texture);
}
