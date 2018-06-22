/*
	@file	Pickel.h
	@brief	ピッケル(ツルハシ)
	@author	番場 宥輝
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	ツルハシクラス
*/
class Pickel
{
public:
	/*! コンストラクタ */
	Pickel();
	/*! デストラクタ */
	~Pickel();


	void Init();
	void Render(D3DXVECTOR2 pos);
	void Destroy();
private:
	Sprite*		m_pSprite;
	Texture*	m_pTexture;
};

