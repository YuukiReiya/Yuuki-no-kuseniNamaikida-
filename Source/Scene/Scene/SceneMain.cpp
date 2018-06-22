/*
	@file	SceneMain.cpp
	@brief	���C���V�[��
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#include "SceneMain.h"
#include "../../GameObject/Manager/InputManager/InputManager.h"
#include "../../GameObject/Manager/SoundManager/SoundManager.h"

/*
	@brief	���O���
*/
using namespace std;

/*
	@def	�萔�錾
*/

/**************/
/*    START   */
/**************/
wstring SceneMain::START_TEXT[] = {
	L"���܂����Ă���܂����A      �͂������񂳂܁B",
	L"�ƁJ�����A�������ɂ䂤���Ⴉ�J ���Ă���悤�āJ���B",
	L"�����X�^�[�����݂��J���A���܂肭��䂤��������J����������̂āJ���B",
};
unsigned int SceneMain::START_ICON[] = {
	1,
	3,
	1,
};
/**************/
/*  SETTING   */
/**************/
wstring SceneMain::SETTING_TEXT[] = {
	L"�ƁJ�����A           �䂤���Ⴉ�J���Ă��܂����悤�āJ���B",
	L"�킽�����J�A�Ă��ɂ��܂��Ă��܂��Ƃ܂��Ă��܂��܂��B�悭���񂩁J���Ă��߂Ă����J�����ˁB",
	L"�������J����Ƃ���ɂ́A�@�@   �킽���͂����܂���B",
	L"����āJ��            �͂���              ��낵�����˂��J�����܂��B",
};
unsigned int SceneMain::SETTING_ICON[] = {
	2,
	3,
	3,
	1,
};
/**************/
/*    WIN     */
/**************/
wstring SceneMain::WIN_TEXT[] = {
	L"�����A�͂������񂳂�       ���݂��J�ƂāJ���B",
	L"���J���A���̂��傤���āJ    ���݂̂܂����J",
	L"�^�C�g���ɂ��ƁJ��܂��B ",
};
unsigned int SceneMain::WIN_ICON[] = {
	1,
	3,
	0,
};
/**************/
/*    LOSE    */
/**************/
wstring SceneMain::LOSE_TEXT[] = {
	L"�����A�͂������񂳂܁B      ���܂��Ă��܂��܂����B",
	L"���J�͂����Ƃ����́J�����   �͂������Ă����J�����I ",
	L"�^�C�g���ɂ��ƁJ��܂��B ",
};
unsigned int SceneMain::LOSE_ICON[] = {
	3,
	2,
	0,
};

/*
	@brief	�R���X�g���N�^
*/
SceneMain::SceneMain()
{
	/*! �C���X�^���X�̐��� */
	m_pPlayer = new Player;
}

/*
	@brief	�f�X�g���N�^
*/
SceneMain::~SceneMain()
{
	Destroy();
}

/*
	@brief	������
*/
void SceneMain::Init()
{
	TerrainManager::GetInstance().Init();
	for (int i = 0; i < 5; i++) {
		TerrainManager::GetInstance().Break({ 28,i });
	}
	TerrainManager::GetInstance().Break({ 29,4 });



	/*! �e�Ǘ��N���X�̏����������Ăяo�� */
	LocationManager::GetInstance().Init();
	EnemyManager::GetInstance().Init();		/*!< �����X�^�[ */
	HeroManager::GetInstance().Init();		/*!< �E�� */
	TycoonManager::GetInstance().Init();	/*!< ���� */
	CollisionManager::GetInstance().Init();	/*!< �Փ� */
	UiManager::GetInstance().Init();		/*!< UI */

	/*! �v���C���[�̏����� */
	m_pPlayer->Init();	

	/*! �����o�ϐ��̏����� */
	mode = MODE::START;
	m_TextItr = m_IconItr = 0;
	m_Fps = 0;
	m_isStop = false;
	m_HeroComeFps = 0;
	if (!m_Debug.Init()) { MSG("debug���s"); }

	/*! ��Փx�� */
	if (SceneTitle::GetLevel() == SceneTitle::LEVEL::EASY) {
		m_pPlayer->SetDigPower(100);
		m_HeroComeFps = FPS * 30;	/*!< 30�b */
		param.hp = 84;
		param.atk = 15;
		param.movePow = 1.0f;
	}
	else if (SceneTitle::GetLevel() == SceneTitle::LEVEL::NORMAL) {
		m_pPlayer->SetDigPower(300);
		m_HeroComeFps = FPS * 90;	/*!< 90�b */
		param.hp = 150;
		param.atk = 35;
		param.movePow = 1.2f;
	}
	else if (SceneTitle::GetLevel() == SceneTitle::LEVEL::HARD) {
		m_pPlayer->SetDigPower(500);
		m_HeroComeFps = FPS * 180;	/*!< 180�b */
		param.hp = 999;
		param.atk = 90;
		param.movePow = 1.5f;
	}

	/*! BGM�Đ� */
	SoundManager::GetInstance().Play(BGM::GAME, true);
}

/*
	@brief �X�V
*/
SceneBase* SceneMain::Update(SceneRoot* root)
{
	SceneBase* next = this;

	/*! �Q�[���N���A */
	if (HeroManager::GetInstance().GetClearFlag()) {
		mode = MODE::WIN;
	}

	/*! �Q�[���I�[�o�[ */
	if (TycoonManager::GetInstance().GetGameOverFlags()) {
		mode = MODE::LOSE;
	}

	
	TerrainManager::GetInstance().Update();		/*!< �n�`�X�V */
	TycoonManager::GetInstance().Update();		/*!< ���� */

	/*! �t���[ */
	switch (mode)
	{
	/*! �V�[���̊J�n�� */
	case SceneMain::START:
		StartUpdate();
		break; 
	/*! �_���W�������� */
	case SceneMain::FREE:
		FreeUpdate();
		break;
	/*! �E�ҌĂяo���� */
	case SceneMain::CALL:
		CallUpdate();
		break;
	/*! �����̈ʒu�ݒ� */
	case SceneMain::SETTING:
		SettingUpdate();
		break;
	/*! �E�҂��_���W�������ɂ��� */
	case SceneMain::BATTLE:
		BattleUpdate();
		break;
	/*! �N���A�� */
	case SceneMain::WIN:
		if (ClearUpdate()) {
			SceneTitle::SetMode(SceneTitle::MODE::CLEAR);
			SoundManager::GetInstance().Stop(BGM::GAME);
			return new SceneTitle;
		}
		break;
	/*! �s�k�� */
	case SceneMain::LOSE:
		if (LoseUpdate()) {
			SceneTitle::SetMode(SceneTitle::MODE::LOSE);
			SoundManager::GetInstance().Stop(BGM::GAME);
			return new SceneTitle;
		}
		break;
	default:
		break;
	}

	
	/*! �v���C���[�̃C���f�b�N�X�̍X�V */
	m_pPlayer->Update();
	
	EnemyManager::GetInstance().Update();		/*!< �����X�^�[�X�V */
	CollisionManager::GetInstance().Update();	/*!< �Փˏ��� */

	/*! ���X�g�̃N���A */
	CollisionManager::GetInstance().Clear();	/*!< �Փˏ����N���A */
	EnemyManager::GetInstance().HitListClear();	/*!< �Փ˔��胊�X�g�N���A */

	
	return next;
}

/*
	@brief	�`��
*/

void SceneMain::Render()
{
	TerrainManager::GetInstance().Render();
	EnemyManager::GetInstance().Render();
	HeroManager::GetInstance().Render();
	TycoonManager::GetInstance().Render();
	m_pPlayer->Render();
	m_pPlayer->DrawDigPower();
	
	UiManager::GetInstance().DrawHeroData();

	/*! �t���[ */
	switch (mode)
	{
	/*! �V�[���̊J�n�� */
	case SceneMain::START:
		UiManager::GetInstance().DrawMessage(START_TEXT[m_TextItr], (Window::ICON)START_ICON[m_IconItr]);
		break;
	/*! �_���W�������� */
	case SceneMain::FREE:
		break;
	/*! �E�ҌĂяo���� */
	case SceneMain::CALL:
		break;
	/*! �����̈ʒu�ݒ� */
	case SceneMain::SETTING:
		/*! �e�L�X�g���ƃC�e���[�^���r */
		if (sizeof(SETTING_TEXT) / sizeof(wstring) > m_TextItr) {
			UiManager::GetInstance().DrawMessage(SETTING_TEXT[m_TextItr], (Window::ICON)SETTING_ICON[m_IconItr]);
		}
		break;
	/*! �E�҂��_���W�������ɂ��� */
	case SceneMain::BATTLE:
		break;
	/*! �N���A�� */
	case SceneMain::WIN:
		SceneTitle::SetMode(SceneTitle::MODE::CLEAR);
		UiManager::GetInstance().DrawMessage(WIN_TEXT[m_TextItr], (Window::ICON)WIN_ICON[m_IconItr]);
		break;
	/*! �s�k�� */
	case SceneMain::LOSE:
		UiManager::GetInstance().DrawMessage(LOSE_TEXT[m_TextItr], (Window::ICON)LOSE_ICON[m_IconItr]);
		break;
	default:
		break;
	}
}

/*
	@brief	���
*/
void SceneMain::Destroy()
{
	TerrainManager::GetInstance().Destroy();
	LocationManager::GetInstance().Destroy();
	EnemyManager::GetInstance().Destroy();
	HeroManager::GetInstance().Destroy();
	TycoonManager::GetInstance().Destroy();
	UiManager::GetInstance().Destroy();
	SAFE_DELETE(m_pPlayer);
}

/*
	@brief	�V�[���J�n���̍X�V����
*/
void SceneMain::StartUpdate()
{	
	/*! �e�L�X�g���� */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(START_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			mode = MODE::FREE;/*!< �t���[���[�h�� */
			m_TextItr = 0;
			m_IconItr = 0;
			SoundManager::GetInstance().Play(BGM::GAME, false);
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
}

/*
	@brief	�N���A���̍X�V����
*/
bool SceneMain::ClearUpdate()
{
	/*! �e�L�X�g���� */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(WIN_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			return true;
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
	return false;
}

/*
	@brief	�s�k���̍X�V����
	@return	�e�L�X�g�̏I��
*/
bool SceneMain::LoseUpdate()
{
	/*! �e�L�X�g���� */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(LOSE_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			return true;
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
	return false;
}

/*
	@brief	�_���W�������쒆�̍X�V����
*/
void SceneMain::FreeUpdate()
{
	/*! �E�җ��K�܂ł̃^�C�� */
	if (m_Fps == m_HeroComeFps) {
		m_Fps = 0;
		mode = MODE::SETTING;
	}

	m_Fps++;
	/*! �v���C���[�̏��� */
	m_pPlayer->Move();		/*!< �J�[�\���ړ� */
	m_pPlayer->Action();	/*!< �u���b�N�̔j�� */
}

/*
	@brief	�E�ҌĂяo�����̍X�V
*/
void SceneMain::CallUpdate()
{
}

/*
	@brief	�����̈ʒu��ݒ肷�鎞�̍X�V
*/
void SceneMain::SettingUpdate()
{
	/*! �e�L�X�g���� */
	if (sizeof(SETTING_TEXT) / sizeof(wstring) - 1 >= m_TextItr) {
		UiManager::GetInstance().Counter();
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			m_TextItr++;
			m_IconItr++;
		}
	}
	/*! �����̈ʒu���� */
	else {
		m_pPlayer->Move();	/*!< �J�[�\���ړ� */
		/*! �����̈ʒu�Z�b�g */
		if (m_pPlayer->SetTycoon()) {
			Soldier* sol = new Soldier;
			sol->SetParam(param.hp, param.atk, param.movePow);
			HeroManager::GetInstance().AddHero(sol);
			m_TextItr = 0;
			m_IconItr = 0;
			UiManager::GetInstance().CounterReset();
			mode = MODE::BATTLE;
		}

	}
}

/*
	@brief	�E�҂��_���W�������ɋ��鎞�̍X�V
*/
void SceneMain::BattleUpdate()
{
	/*! �}�l�[�W���[ */
	HeroManager::GetInstance().Update();	/*!< �E�� */

	/*! �v���C���[ */
	m_pPlayer->Move();	/*!< �J�[�\���ړ� */
	m_pPlayer->Action();/*!< �u���b�N�̔j�� */
}