/*
	@file	Pickel.cpp
	@brief	�s�b�P��(�c���n�V)
	@author	�ԏ� �G�P
*/
#include "Pickel.h"


/*
	@brief	�R���X�g���N�^
*/
Pickel::Pickel()
{
}

/*
	@brief	�f�X�g���N�^
*/
Pickel::~Pickel()
{
	Destroy();
}

/*
	@brief	������
*/
void Pickel::Init()
{
	m_pSprite	 =	new Sprite;
	m_pTexture	 =	new Texture;

	m_pSprite->SetSpriteSize(32, 32);
	m_pSprite->SetSizeScaling(2.0f);

	if (!m_pTexture->Load(L"../Resource/Texture/��͂�.png")) { MSG("�e�N�X�`���̓ǂݍ��ݎ��s"); }
}

/*
	@brief	�`��
*/
void Pickel::Render(D3DXVECTOR2 pos)
{
	pos += {30, -30};
	m_pSprite->SetPos(pos);
	m_pSprite->Render(m_pTexture->GetTexture());
}

/*
	@brief	���
*/
void Pickel::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pTexture)
}
