/*
	@file	ImmortalBlock.h
	@brief	破壊不能ブロック
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Camera/Camera2D.h"
#include "../../../Texture/Texture.h"
#include "../../../Sprite/Sprite.h"

/*
	@brief	破壊不可能ブロッククラス
*/
class ImmortalBlock
{
public:
	/*! コンストラクタ */
	ImmortalBlock();
	/*! デストラクタ */
	~ImmortalBlock();

	void Init();
	void Render(IDirect3DTexture9* texture);
	void Destroy();
private:
	Sprite* m_pSprite[2][SOIL_HEIGHT];
};

