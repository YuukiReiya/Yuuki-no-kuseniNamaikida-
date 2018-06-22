#include "Ground.h"


/*
	@brief	コンストラクタ
*/
Ground::Ground()
{
}

/*
	@brief	デストラクタ
*/
Ground::~Ground()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Ground::Init()
{
	m_pSprite = new Sprite;
	m_pTexture = new Texture;

	if (!m_pTexture->Load(L"../Resource/Texture/Ground.png")) { MSG("テクスチャの読み込みに失敗"); }

	m_pSprite->SetSpriteSize(GRID_SIZE*62, GRID_SIZE*9);
	m_pSprite->SetPos({ 0,0 });
}

/* 
	@brief	描画
*/
void Ground::Render()
{
	//m_pSprite->RenderLeftTop(m_pTexture->GetTexture());
	m_pSprite->RenderLeftTop(m_pTexture->GetTexture());
}

/*
	@brief	解放
*/
void Ground::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pTexture);
}
