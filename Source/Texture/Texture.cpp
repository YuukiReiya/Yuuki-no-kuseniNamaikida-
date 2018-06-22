/*
	@file	Texture.cpp
	@brief	�e�N�X�`��
	@author	�ԏ� �G�P
*/
#include "Texture.h"

/*
	@brief	�R���X�g���N�^
*/
Texture::Texture() {
	m_pTexture = NULL;
}

/*
	@brief	�f�X�g���N�^	
*/
Texture::~Texture()
{
	/*! �ǂݍ��܂�Ă�����j�� */
	if (m_pTexture != NULL)
		m_pTexture->Release();
}

/*
	@brief	�摜�t�@�C���̓ǂݍ���
*/
bool Texture::Load(TCHAR* FileName)
{
	/*! �摜�ǂݍ��� */
	if (FAILED(D3DXCreateTextureFromFile(DirectDevice::GetInstance().GetDevice(), FileName, &m_pTexture)))
		/*! �摜�ǂݍ��ݎ��s�i�t�@�C�����Ȃ��\������j */
		return false;

	/*! �摜�ǂݍ��ݐ��� */
	return true;
}
