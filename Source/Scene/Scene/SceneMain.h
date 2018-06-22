/*
	@file	SceneMain.cpp
	@brief	���C���V�[��
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#pragma once
#include <iostream>
#include "../SceneBase/Scene.h"
#include "../SceneRoot/SceneRoot.h"
#include "SceneTitle.h"
#include "../../GameObject/Manager/LocationManager/LocationManager.h"
#include "../../GameObject/Manager/TerrainManager/TerrainManager.h"
#include "../../GameObject/Manager/EnemyManager/EnemyManager.h"
#include "../../GameObject/Manager/HeroManager/HeroManager.h"
#include "../../GameObject/Manager/UiManager/UiManager.h"
#include "../../GameObject/Manager/CollisionManager/CollisionManager.h"
#include "../../Texture/Texture.h"
#include "../../Sprite/Sprite.h"
#include "../../Debug/Debug.h"
#include "../../GameObject/Player/Player.h"

/*�f�o�b�O*/
#include "../../GameObject/Character/Hero/Soldier/Soldier.h"

/*
	@brief	���C���V�[���N���X
*/
class SceneMain:public Scene
{
public:
	/*! �R���X�g���N�^ */
	SceneMain();
	/*! �f�X�g���N�^ */
	~SceneMain();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();

	
private:
	void StartUpdate();
	bool ClearUpdate();
	bool LoseUpdate();
	void FreeUpdate();
	void CallUpdate();
	void SettingUpdate();
	void BattleUpdate();

	/*  
		@enum	MODE
		@brief	�Q�[���̃t���[
	*/
	enum MODE {
		START,	/*!< �V�[���J�n */
		FREE,	/*!< �_���W�����J�� */
		CALL,	/*!< �E�ҌĂяo�� */
		SETTING,/*!< �����̈ʒu�Z�b�g */
		BATTLE,	/*!< �E�҂��� */
		WIN,	/*!< ���� */
		LOSE,	/*!< �s�k */
	}mode;


	static std::wstring START_TEXT[];	/*!< START�\���e�L�X�g */
	static unsigned int START_ICON[];	/*!< START�\���A�C�R�� */
	static std::wstring SETTING_TEXT[];	/*!< SETTING�\���e�L�X�g */
	static unsigned int SETTING_ICON[];	/*!< SETTING�\���A�C�R�� */
	static std::wstring WIN_TEXT[];		/*!< WIN�\���e�L�X�g */
	static unsigned int WIN_ICON[];		/*!< WIN�\���A�C�R�� */
	static std::wstring LOSE_TEXT[];	/*!< LOSE�\���e�L�X�g */
	static unsigned int LOSE_ICON[];	/*!< LOSE�\���A�C�R�� */

	/*
		@struct	PARAMETER
		@brief	�p�����[�^�ۑ��\���̐�
	*/
	struct PARAMETER
	{
		int hp;
		int atk;
		float movePow;
	}param;


	/*! �ϐ� */
	Player* m_pPlayer;		/*!< �v���C���[ */
	Debug	m_Debug;
	DWORD	m_Fps;			/*!< FPS�J�E���g */
	DWORD	m_HeroComeFps;	/*!< �E�҂�����܂ł̃t���[�� */
	bool	m_isStop;		/*!<�Q�[���̍X�V�������~�߂�t���O*/
	int		m_TextItr;		/*!< �e�L�X�g�C�e���[�^ */
	int		m_IconItr;		/*!< �A�C�R���C�e���[�^ */
	
};

