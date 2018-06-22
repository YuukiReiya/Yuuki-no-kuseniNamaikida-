/*
	@file	Soil.h
	@brief	�y
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	�y�N���X
*/
class Soil
{
public:
	/*! �R���X�g���N�^ */
	Soil();
	/*! �f�X�g���N�^ */
	~Soil();

	void Init();
	void Render(IDirect3DTexture9* texture);
	
	D3DXVECTOR2			m_Pos;			/*!< �ʒu */
	INT2				m_Component;	/*!< �{���E���� */
	bool				m_isBreaked;	/*!< �u���b�N�̏�� */
private:

	Sprite*				m_pSprite;		/*!< �X�v���C�g */
};

