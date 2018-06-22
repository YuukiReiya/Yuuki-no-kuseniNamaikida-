/*
	@file	Soil.h
	@brief	土
	@author	番場 宥輝
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	土クラス
*/
class Soil
{
public:
	/*! コンストラクタ */
	Soil();
	/*! デストラクタ */
	~Soil();

	void Init();
	void Render(IDirect3DTexture9* texture);
	
	D3DXVECTOR2			m_Pos;			/*!< 位置 */
	INT2				m_Component;	/*!< 養分・魔分 */
	bool				m_isBreaked;	/*!< ブロックの状態 */
private:

	Sprite*				m_pSprite;		/*!< スプライト */
};

