/*
	@file	SceneTitle.h
	@brief	�^�C�g���V�[��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../SceneBase/Scene.h"
#include "../Scene/SceneMain.h"
#include "../../Common/Common.h"
#include "../../GameObject/Window/Window.h"

/*
	@brief	�^�C�g���V�[���N���X
*/
class SceneTitle :
	public Scene
{
public:
	/*! �R���X�g���N�^ */
	SceneTitle();
	/*! �f�X�g���N�^ */
	~SceneTitle();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();

	/*
		@enum	LEVEL
		@brief	�Q�[���̓�Փx
	*/
	enum LEVEL {
		EASY,
		NORMAL,
		HARD,
	}static level;

	/*
		@enum	MODE
		@brief	�Q�[���̃t���[
	*/
	enum MODE {
		START,	/*!< �Q�[���N���� */
		SELECT,	/*!< �Z���N�g */
		CLEAR,	/*!< �X�e�[�W�N���A */
		LOSE,	/*! �X�e�[�W���s */
		PERFECT,/*!< �Q�[���N���A */
		END,	/*!< �Q�[���I�� */
	}static mode;
	static void SetMode(MODE val) { mode = val; }
	static MODE GetMode() { return mode; }
	static LEVEL GetLevel() { return level; }
private:

	void SetUpText();
	bool SelectUpdate();
	void SelectRender();
	void DrawLevel();

	/*! �萔�錾 */
	static std::wstring START_TEXT[];	/*!< START�\���e�L�X�g */
	static unsigned int START_ICON[];	/*!< START�\���A�C�R�� */
	static std::wstring SELECT_TEXT[];	/*!< SELECT�\���e�L�X�g */
	static unsigned int SELECT_ICON[];	/*!< SELECT�\���A�C�R�� */
	static std::wstring CLEAR_TEXT[];	/*!< CLEAR�\���e�L�X�g */
	static unsigned int CLEAR_ICON[];	/*!< CLEAR�\���A�C�R�� */
	static std::wstring LOSE_TEXT[];	/*!< LOSE�\���e�L�X�g */
	static unsigned int LOSE_ICON[];	/*!< LOSE�\���A�C�R�� */
	static std::wstring PERFECT_TEXT[];	/*!< PERFECT�\���e�L�X�g */
	static unsigned int PERFECT_ICON[];	/*!< PERFECT�\���A�C�R�� */
	static std::wstring END_TEXT[];		/*!< END�\���e�L�X�g */
	static unsigned int END_ICON[];		/*!< END�\���A�C�R�� */

	int			m_TextItr;				/*!< �e�L�X�g����̃C�e���[�^ */
	UINT		m_IconItr;				/*!< �e�L�X�g�̃A�C�R������C�e���[�^ */
	Sprite*		m_pBackGroundSprite;	/*!< �w�i�X�v���C�g */
	Texture*	m_pBackGroundTexture;	/*!< �w�i�e�N�X�`�� */
};

