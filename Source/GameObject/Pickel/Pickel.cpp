/*
	@file	Pickel.cpp
	@brief	ピッケル(ツルハシ)
	@author	番場 宥輝
*/
#include "Pickel.h"


/*
	@brief	コンストラクタ
*/
Pickel::Pickel()
{
}

/*
	@brief	デストラクタ
*/
Pickel::~Pickel()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Pickel::Init()
{
	m_pSprite	 =	new Sprite;
	m_pTexture	 =	new Texture;

	m_pSprite->SetSpriteSize(32, 32);
	m_pSprite->SetSizeScaling(2.0f);

	if (!m_pTexture->Load(L"../Resource/Texture/つるはし.png")) { MSG("テクスチャの読み込み失敗"); }
}

/*
	@brief	描画
*/
void Pickel::Render(D3DXVECTOR2 pos)
{
	pos += {30, -30};
	m_pSprite->SetPos(pos);
	m_pSprite->Render(m_pTexture->GetTexture());
}

/*
	@brief	解放
*/
void Pickel::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pTexture)
}
