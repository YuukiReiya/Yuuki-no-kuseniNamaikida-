/*
	@file	SceneTitle.cpp	
	@brief	�^�C�g���V�[��
	@author	�ԏ� �G�P
*/
#include "SceneTitle.h"
#include "../../GameObject/Manager/UiManager/UiManager.h"
#include "../../GameObject/Manager/InputManager/InputManager.h"
#include "../../GameObject/Manager/SoundManager/SoundManager.h"

/*
	@brief	���O���
*/
using namespace std;

/*
	@var	�O���[�o���ϐ�
*/
SceneTitle::MODE SceneTitle::mode = MODE::START;
SceneTitle::LEVEL SceneTitle::level;

/*
	@def	�萔�錾
*/

/**************/
/*    START   */
/**************/
wstring SceneTitle::START_TEXT[] = {
	L"���J�񂱂��̂͂�������݂̂Ȃ��� �낤�ق��āJ���I�I",
	L"���́A���J���ɂ񂫃^�C�g��    �u�䂤����̂����ɂȂ܂������J�v�@���J�Ȃ�ƁI�I",
	L"���J�����������Ђ�Ƃ���     �ӂ����������܂����B",
	L"���₟�A���J�������ɂ�郊���C�N �Ȃ�Ă��ꂵ���āJ���ˁB",
	L"�ƁJ�����J������䂭�܂āJ�A   �����̂��݂����J�����B"

};
unsigned int SceneTitle::START_ICON[] = {
	1,
	2,
	1,
	4,
	1,
};
/**************/
/*   SELECT   */
/**************/
wstring SceneTitle::SELECT_TEXT[] = {
	L"�����A��������          �X�e�[�V�J�������āJ�����J�����B�͂������񂳂܁B",
	L"�킽�����́A           �ЂƂ��������ɂ����ނ��Ă���܂��B",
};
unsigned int SceneTitle::SELECT_ICON[] = {
	1,
	1,
};
/**************/
/*    CLEAR   */
/**************/
wstring SceneTitle::CLEAR_TEXT[] = {
	L"�����Ă��ӁJ�Ƃ̂������߂�B   �䂽�J�񂵂Ȃ��āJ�����J�����A�@�@�͂������񂳂܁B",
	L"���J���A            �����Ђ����߂Ă����܂��傤�B",
};
unsigned int SceneTitle::CLEAR_ICON[] = {
	1,
	1,
};
/**************/
/*    LOSE    */
/**************/
wstring SceneTitle::LOSE_TEXT[] = {
	L"�����A             �����́K�����Ă��܂��܂�����",
	L"�����Ƃ�Ȃ����āA        ���J�����܂��傤�B",
};
unsigned int SceneTitle::LOSE_ICON[] = {
	4,
	3,
};
/**************/
/*   PERFECT  */
/**************/
wstring SceneTitle::PERFECT_TEXT[] = {
	L"�����A���������X�e�[�W�������ł��������B      �͂������񂳂܁B",
	L"�������ɂւ���́A���Ƃ���Ȃ�",
};
unsigned int SceneTitle::PERFECT_ICON[] = {
	1,
	2,
};
/**************/
/*    END     */
/**************/
wstring SceneTitle::END_TEXT[] = {
	L"����A�Q�[������߂���̂ł����B   ����˂�ł��B",
	L"�܂��A������ł���������",
};
unsigned int SceneTitle::END_ICON[] = {
	1,
	2,
};


/*
	@brief	�R���X�g���N�^
*/
SceneTitle::SceneTitle()
{
}

/*
	@brief	�f�X�g���N�^
*/
SceneTitle::~SceneTitle()
{
	Destroy();
}

/*
	@brief	������
*/
void SceneTitle::Init()
{
	m_pBackGroundSprite = new Sprite;
	m_pBackGroundTexture = new Texture;

	m_pBackGroundSprite->SetSpriteSize(478, 270);
	m_pBackGroundSprite->SetSizeScaling(4);
	if (!m_pBackGroundTexture->Load(L"../Resource/Texture/back.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }
	
	UiManager::GetInstance().Init();
	SoundManager::GetInstance().Vol(BGM::TITLE, 60);
	SoundManager::GetInstance().Play(BGM::TITLE,true);
	SetUpText();
	m_TextItr = 0;
	m_IconItr = 0;
	level = LEVEL::EASY;
}

/*
	@brief	�X�V
*/
SceneBase * SceneTitle::Update(SceneRoot * root)
{
	SceneBase* next = this;
	UiManager::GetInstance().Counter();
	/*! �t���[ */
	switch (mode)
	{
	/*! �Q�[���N���� */
	case SceneTitle::START:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(START_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! �X�e�[�W�Z���N�g */
	case SceneTitle::SELECT:
		if (SelectUpdate()) {
			UiManager::GetInstance().CounterReset();
			SoundManager::GetInstance().Stop(BGM::TITLE);
			return new SceneMain;
		}
		break;
	/*! �X�e�[�W�N���A */
	case SceneTitle::CLEAR:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(CLEAR_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! �X�e�[�W���s */
	case SceneTitle::LOSE:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(LOSE_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! �Q�[���N���A */
	case SceneTitle::PERFECT:
		break;
	/*! �Q�[���I�� */
	case SceneTitle::END:
		break;
	default:
		break;
	}
	return next;
}

/*
	@brief	�����_�����O
*/
void SceneTitle::Render()
{
		/*! �t���[ */
		switch (mode)
		{
		/*! �Q�[���N���� */
		case SceneTitle::START:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(START_TEXT[m_TextItr], (Window::ICON)START_ICON[m_IconItr]);
			break;
		/*! �X�e�[�W�Z���N�g */
		case SceneTitle::SELECT:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			SelectRender();
			break;
		/*! �X�e�[�W�N���A */
		case SceneTitle::CLEAR:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(CLEAR_TEXT[m_TextItr], (Window::ICON)CLEAR_ICON[m_IconItr]);
			break;
		/*! �X�e�[�W���s */
		case SceneTitle::LOSE:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(LOSE_TEXT[m_TextItr], (Window::ICON)LOSE_ICON[m_IconItr]);
			break;
		/*! �Q�[���N���A */
		case SceneTitle::PERFECT:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(PERFECT_TEXT[m_TextItr], (Window::ICON)PERFECT_ICON[m_IconItr]);
			break;
		/*! �Q�[���I�� */
		case SceneTitle::END:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(END_TEXT[m_TextItr], (Window::ICON)END_ICON[m_IconItr]);
			break;
		default:
			break;
		}

}

/*
	@brief	���
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_pBackGroundSprite);
	SAFE_DELETE(m_pBackGroundTexture);
	UiManager::GetInstance().Destroy();
}

/*
	@brief	�\���e�L�X�g�̃Z�b�g�A�b�v
*/
void SceneTitle::SetUpText()
{
	
}

/*
	@brief	�X�e�[�W�Z���N�g�̍X�V����
*/
bool SceneTitle::SelectUpdate()
{
	/*! �e�L�X�g�̕\�� */
	if (sizeof(SELECT_TEXT) / sizeof(wstring) > m_TextItr) {
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			m_TextItr++;
			m_IconItr++;
		}
		return false;
	}
	/*! ���x������ */
	else {
		int l = (int)level;
		if (input.GetKeyDown(input.UP)) {
			SoundManager::GetInstance().Play(SE::MENU, false);
			l--;
			if (l < (int)LEVEL::EASY) {
				l = (int)LEVEL::HARD;
			}
		}
		else if (input.GetKeyDown(input.DOWN)) {
			SoundManager::GetInstance().Play(SE::MENU, false);
			l++;
			if ((int)LEVEL::HARD < l) {
				l = (int)LEVEL::EASY;
			}
		}
		else if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			return true;
		}
		level = (LEVEL)l;
	}
	return false;
}

/*
	@brief	�Z���N�g�̕`��
*/
void SceneTitle::SelectRender()
{
	if (sizeof(SELECT_TEXT) / sizeof(wstring) > m_TextItr) {
		UiManager::GetInstance().DrawMessage(SELECT_TEXT[m_TextItr], (Window::ICON)SELECT_ICON[m_IconItr]);
	}
	else {
		DrawLevel();
	}
}

/*
	@brief	��Փx�`��
*/
void SceneTitle::DrawLevel()
{
	D3DXVECTOR2 pos = {
		950,
		250
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		pos.x,
		pos.y + 250,
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		pos.x,
		pos.y + 250,
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		700,
		(float)(250 + (int)level * 250)
	};
	UiManager::GetInstance().DrawCursolRight(pos);
	pos = {
		pos.x + 130,
		250
	};
	UiManager::GetInstance().DrawString(L"���񂽂�", 8.5f, pos);
	pos = {
		pos.x + 40,
		500
	};
	UiManager::GetInstance().DrawString(L"�ӂ�", 8.5f, pos);
	pos = {
		pos.x - 70,
		750
	};
	UiManager::GetInstance().DrawString(L"�ނ��J������", 8.0f, pos);
}
