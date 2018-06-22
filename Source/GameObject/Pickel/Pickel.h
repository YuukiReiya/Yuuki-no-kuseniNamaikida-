/*
	@file	Pickel.h
	@brief	�s�b�P��(�c���n�V)
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	�c���n�V�N���X
*/
class Pickel
{
public:
	/*! �R���X�g���N�^ */
	Pickel();
	/*! �f�X�g���N�^ */
	~Pickel();


	void Init();
	void Render(D3DXVECTOR2 pos);
	void Destroy();
private:
	Sprite*		m_pSprite;
	Texture*	m_pTexture;
};

