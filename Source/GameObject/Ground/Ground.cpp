#include "Ground.h"


/*
	@brief	�R���X�g���N�^
*/
Ground::Ground()
{
}

/*
	@brief	�f�X�g���N�^
*/
Ground::~Ground()
{
	Destroy();
}

/*
	@brief	������
*/
void Ground::Init()
{
	m_pSprite = new Sprite;
	m_pTexture = new Texture;

	if (!m_pTexture->Load(L"../Resource/Texture/Ground.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }

	m_pSprite->SetSpriteSize(GRID_SIZE*62, GRID_SIZE*9);
	m_pSprite->SetPos({ 0,0 });
}

/* 
	@brief	�`��
*/
void Ground::Render()
{
	//m_pSprite->RenderLeftTop(m_pTexture->GetTexture());
	m_pSprite->RenderLeftTop(m_pTexture->GetTexture());
}

/*
	@brief	���
*/
void Ground::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pTexture);
}
