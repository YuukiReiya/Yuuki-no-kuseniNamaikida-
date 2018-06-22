/*
	@file	Texture.h
	@brief	�e�N�X�`��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Common/Common.h"
#include  <d3dx9tex.h>

/*
	@brief	�e�N�X�`���̓ǂݍ��݃N���X
*/
class Texture {
public:
	/*! �R���X�g���N�^ */
	Texture();
	/*! �f�X�g���N�^ */
	~Texture();
	/*! �摜�f�[�^�ǂݍ��� */
	bool Load(TCHAR* FileName);
	/*! �Q�b�^�[ */
	IDirect3DTexture9* GetTexture() { return m_pTexture; }
private:
	IDirect3DTexture9* m_pTexture;
};