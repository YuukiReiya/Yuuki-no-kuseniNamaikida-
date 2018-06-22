/*
	@file	Soil.cpp
	@brief	�y
	@author	�ԏ� �G�P
*/
#include "Soil.h"
#include "../../Camera/Camera2D.h"

/*
	@brief	�R���X�g���N�^
*/
Soil::Soil()
{
}

/*
	@brief	�f�X�g���N�^
*/
Soil::~Soil()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	������
*/
void Soil::Init()
{
	m_pSprite = new Sprite;

	m_isBreaked = false;/*!< �ŏ��͉��ĂȂ� */

	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);/*!< �X�v���C�g�T�C�Y */
}

/*
	@brief	�����_�����O
*/
void Soil::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderLeftTop(texture);
}
