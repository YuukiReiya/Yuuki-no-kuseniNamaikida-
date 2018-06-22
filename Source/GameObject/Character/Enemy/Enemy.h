/*
	@file	Enemy.h
	@brief	�����X�^�[
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Character.h"
#include "../../../Common/Common.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Camera/Camera2D.h"
#include "../../Manager/TerrainManager/TerrainManager.h"
#include "../Hero/Hero.h"


/*! �O���錾 */
struct ENEMY_NODE;

/*
	@brief	�����X�^�[�̊��N���X
*/
class Enemy :
	public Character
{
public:
	/*! �R���X�g���N�^ */
			 Enemy() = default;
	/*! ���z�f�X�g���N�^ */
	virtual ~Enemy() = default;

	/*
		@enum	Type
		@brief	�����X�^�[�̎��
	*/
	enum Type {
		/*! �R�P�� */
		MOSS,		/*!< �R�P */
		BUDS,		/*!< �Q */
		FLOWER,		/*!< �� */

		/*! ���� */
		LARVAE,		/*!< �c�� */
		CHRYSALIS,	/*!< � */
		ADULT,		/*!< ���� */

		/*! �g�J�Q�� */
		EGG,		/*!< �� */
		LIZARD		/*!< �g�J�Q */
	};

	/*! �������z�֐� */
	virtual void Init(INT2 pos, INT2 component)		= 0;
	virtual void Destroy()							= 0;
	virtual void Update()							= 0;
	virtual void Render(IDirect3DTexture9* texture)	= 0;
	virtual void Damage(int damage)					= 0;
	virtual void Collision(Character& character)	= 0;

	/*! ���z�֐� */
	virtual void Init(INT2 pos, INT2 component, int hp) {};

	/*! �Q�b�^�[ */
	Type		GetType()			{ return m_Type; }
	bool		GetDeleteFlags()	{ return m_isDelete; }
	int			GetHp()				{ return m_Hp; }
	D3DXVECTOR2	GetPos()			{ return m_Pos; }
	D3DXVECTOR2 GetDirection()		{ return m_Direction; }
	INT2		GetIndex()			{ return m_IndexPos; }
	INT2		GetComponent()		{ return m_Component; }
	/*! �Z�b�^�[ */
	void SetEnemyNode(ENEMY_NODE* node) { m_pNode = node; }
	void SetHero(Hero* hero)			{ m_pHero = hero; }
protected:
	Sprite*		m_pSprite;		/*!< �X�v���C�g */
	ENEMY_NODE* m_pNode;		/*!< �m�[�h */
	Type		m_Type;			/*!< ��� */
	Hero*		m_pHero;		/*!< �U���Ώۂ̗E�� */
	int			m_Hp;			/*!< �̗� */
	int			m_Atk;			/*!< �U���� */
	INT2		m_Component;	/*!< �{���E���� */
	D3DXVECTOR2 m_Pos;			/*!< ���W */
	INT2		m_IndexPos;		/*!< ���W�̃C���f�b�N�X */
	INT2		m_NextIndexPos;	/*!< �ړ���C���f�b�N�X���W */
	D3DXVECTOR2 m_Direction;	/*!< �����Ă������ */
	float		m_MoveWeight;	/*!< �ړ��� */
	bool		m_isCollision;	/*!< �Փ˃t���O */
	bool		m_isDelete;		/*!< �폜�t���O */
	bool		m_isDead;		/*!< ���S�t���O */
	DWORD		m_AtkCount;		/*!< �U���J�E���^ */
	int			m_AnimCount;	/*!< �A�j���[�V�����J�E���^ */
	int			m_AnimIndexU;	/*!< UV�摜U�C���f�b�N�X */
	int			m_AnimIndexV;	/*!< UV�摜V�C���f�b�N�X */
};

