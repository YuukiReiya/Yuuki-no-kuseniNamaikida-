/*
	@file	Texture.cpp
	@brief	テクスチャ
	@author	番場 宥輝
*/
#include "Texture.h"

/*
	@brief	コンストラクタ
*/
Texture::Texture() {
	m_pTexture = NULL;
}

/*
	@brief	デストラクタ	
*/
Texture::~Texture()
{
	/*! 読み込まれていたら破棄 */
	if (m_pTexture != NULL)
		m_pTexture->Release();
}

/*
	@brief	画像ファイルの読み込み
*/
bool Texture::Load(TCHAR* FileName)
{
	/*! 画像読み込み */
	if (FAILED(D3DXCreateTextureFromFile(DirectDevice::GetInstance().GetDevice(), FileName, &m_pTexture)))
		/*! 画像読み込み失敗（ファイルがない可能性あり） */
		return false;

	/*! 画像読み込み成功 */
	return true;
}
