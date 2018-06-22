/*
	@file	ImmortalBlock.h
	@brief	�j��s�\�u���b�N
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Camera/Camera2D.h"
#include "../../../Texture/Texture.h"
#include "../../../Sprite/Sprite.h"

/*
	@brief	�j��s�\�u���b�N�N���X
*/
class ImmortalBlock
{
public:
	/*! �R���X�g���N�^ */
	ImmortalBlock();
	/*! �f�X�g���N�^ */
	~ImmortalBlock();

	void Init();
	void Render(IDirect3DTexture9* texture);
	void Destroy();
private:
	Sprite* m_pSprite[2][SOIL_HEIGHT];
};

