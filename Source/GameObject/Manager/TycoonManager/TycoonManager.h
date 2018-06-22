/*
	@file	TycoonManager.h
	@brief	�������Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Character/Tycoon/Tycoon.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Texture/Texture.h"
#include "../../../Debug/Debug.h"
#include "../../../Camera/Camera2D.h"
#include "../../Character/Hero/Hero.h"

/*
	@brief	�����̊Ǘ��N���X
*/
class TycoonManager
{
public:
	/*! �R���X�g���N�^ */
	TycoonManager();
	/*! �f�X�g���N�^ */
	~TycoonManager();

	/*! �V���O���g�� */
	static TycoonManager& GetInstance() {
		static TycoonManager instance;
		return instance;
	}

	/*! �֐� */
	void Init();
	void Render();
	void Destroy();
	void SetHeroPos(INT2 pos);
	void Update();
	void SetTycoonPos(INT2 Pos);

	/*! �Q�b�^�[ */
	INT2 GetTycoonPos() { return m_pTycoon->m_IndexPos; }
	bool GetCaughtFlags() { return m_pTycoon->isCaught; }
	bool GetGameOverFlags() { return m_isGameOver; }

	/*! �Z�b�^�[ */
	void SetHeroData(float movePow) { m_pTycoon->m_MovePow = movePow; }
	void SetCaughtFlags(bool flags) { m_pTycoon->isCaught = flags; }
private:

	/*! �ϐ� */
	Tycoon*		m_pTycoon;
	INT2*		m_pHeroPos;
	bool		m_isGameOver;
	Texture*	m_pTexture;
	Debug		m_Debug;
};

