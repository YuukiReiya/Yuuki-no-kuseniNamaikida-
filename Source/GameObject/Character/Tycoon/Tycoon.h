/*
	@file	Tycoon.h
	@brief	����
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Character.h"
#include "../../../Sprite/Sprite.h"
#include ".././../../Camera/Camera2D.h"
#include "../../../Debug/Debug.h"
#include "../../../Algorithm/AStar/AStar.h"

/*
	@brief	�����N���X
*/
class Tycoon :
	public Character
{
public:
	/*! �R���X�g���N�^ */
	Tycoon();
	/*! �f�X�g���N�^ */
	~Tycoon();

	
	void Init();								/*!< ������ */
	void Update();								/*!< �X�V */
	void Render(IDirect3DTexture9* texture);	/*!< �`�� */
	void Destroy();								/*!< ��� */
	void Move();								/*!< �ړ� */
	bool isCaught;								/*!< �ߔ��t���O */

	/*! �Փ� */
	void Collision(Character& character) {};	
	void Collision(Soldier& solder) {};			

	/*! �ϐ� */
	D3DXVECTOR2 m_Pos;
	INT2		m_IndexPos;
	INT2		m_NextIndexPos;
	float		m_MovePow;
private:
	void IndexUpdate();							/*!< �C���f�b�N�X�X�V */
	void AnimUpdate();							/*!< �A�j���[�V�����X�V */

	/*! �萔 */
	static const unsigned int ANIMATION_UPDATE_COUNT;

	DWORD			m_AnimCount;
	unsigned int	m_AnimIndexU;
	unsigned int	m_AnimIndexV;
	Sprite*			m_pSprite;
	Debug*			m_pDebug;
};

