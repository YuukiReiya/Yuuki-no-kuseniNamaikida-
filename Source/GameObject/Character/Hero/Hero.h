/*
	@file	Hero.h
	@brief	�E��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Character.h"
#include "../../../Camera/Camera2D.h"
#include "../../../Sprite/Sprite.h"

/*
	@brief	�E�҂̊��N���X
*/
class Hero :
	public Character
{
public:
	/*! �R���X�g���N�^ */
			 Hero() = default;
	/*! ���z�f�X�g���N�^ */
	virtual ~Hero() = default;

	/*! �������z�֐� */
	virtual void Init()		= 0;
	virtual void Destroy()	= 0;
	virtual void Update()	= 0;
	virtual void Render(IDirect3DTexture9* texture) = 0;
	virtual void IndexUpdate() = 0;
	virtual void Catch() = 0;
	virtual void Damage(int damage) = 0;
	virtual void Collision(Character& character) = 0;

	/*! �Q�b�^�[ */
	int GetHp() { return m_Hp; }
	INT2 GetIndex() { return m_IndexPos; }
	bool GetCaught() { return m_isCaught; }
	D3DXVECTOR2 GetPos() { return m_Pos; }
	float GetMovePow() { return m_Move; }

	/*! �Z�b�^�[ */
	void SetCaught(bool flags) { m_isCaught = flags; }
	void SetParam(int hp, int atk, float movePow) {
		m_Hp = hp, m_Atk = atk, m_Move = movePow;
	}
protected:
	std::string m_Name;			/*!< ���O */
	int			m_Hp;			/*!< �̗� */
	int			m_Atk;			/*!< �U���� */
	float		m_Move;			/*!< �ړ��� */
	D3DXVECTOR2 m_Direction;	/*!< ���� */
	D3DXVECTOR2 m_Pos;			/*!< ���W */
	INT2		m_IndexPos;		/*!< ���W�̃C���f�b�N�X */
	INT2		m_NextIndexPos;	/*!< �ړ�����W�̃C���f�b�N�X */
	bool		m_isCaught;		/*!< �����ߔ��t���O */
	Sprite*		m_pSprite;		/*!< �X�v���C�g */
	int			m_AnimCount;	/*!< �A�j���[�V�����J�E���^ */
	int			m_AnimIndexU;	/*!< UV�摜U�C���f�b�N�X */
	int			m_AnimIndexV;	/*!< UV�摜V�C���f�b�N�X */
	
};