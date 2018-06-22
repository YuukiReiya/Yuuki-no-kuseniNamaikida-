/*
	@file	Sprite.h
	@brief	�X�v���C�g
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Common/Common.h"

/*
	@brief	�X�v���C�g�N���X
*/
class Sprite {

public:
	/*! �R���X�g���N�^ */
	Sprite();
	/*! �f�X�g���N�^ */
	~Sprite();

	/*! �Z�b�^�[ */
	void SetPos(D3DXVECTOR2 pos) { m_Pos = pos; }											/*!< �ʒu */
	void SetSpriteSize(float Width, float Height) { m_Width = Width, m_Height = Height; }	/*!< �X�v���C�g�̃T�C�Y */
	void SetSizeScaling(float ScaleMag) { m_Width *= ScaleMag, m_Height *= ScaleMag; }		/*!< �T�C�Y�̃X�P�[�����O(�䗦���ێ������܂܊g�k) */
	bool SetSplit(unsigned int U, unsigned int V);											/*!< �e�N�X�`���̕��� */
	bool SetIndex(unsigned int U, unsigned int V);											/*!< ���������C���f�b�N�X */

	/*! �`�� */
	void Render(IDirect3DTexture9* pTexture, bool isReverse = false);												/*!< �摜�̒��S���� */
	void RenderLeftTop(IDirect3DTexture9* pTexture, bool isReverse = false);										/*!< �摜�̍��ォ�� */
	bool RenderAtlas(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse = false);			/*!< �A�g���X�摜�̕`��i���S�j */
	bool RenderAtlasLeftTop(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse = false);	/*!< �A�g���X�摜�̕`��i����j */
private:
	/*
		@struct	Vertex
		@brief	�|���S�����_���
	*/
	struct Vertex {
		float x, y, z;	/*!< 3�������W */
		float rhw;		/*!< 2D�ϊ��t���O */
		float u, v;		/*!< UV���W */
	};

	/*! FVF(�_��Ȓ��_�\���̐錾)�t���O */
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	/*! �X�v���C�g�ʒu */
	D3DXVECTOR2 m_Pos;

	/*! �X�v���C�g�T�C�Y */
	float m_Width;	/*!< ���� */
	float m_Height;	/*!< �c�� */

	/*! UV������ */
	unsigned int m_SplitU;	/*!< ��:x */
	unsigned int m_SplitV;	/*!< �c:y */

	/*! ��������UV�̃C���f�b�N�X */
	unsigned int m_IndexU;	/*!< ��:x */
	unsigned int m_IndexV;	/*!< �c:y */
};