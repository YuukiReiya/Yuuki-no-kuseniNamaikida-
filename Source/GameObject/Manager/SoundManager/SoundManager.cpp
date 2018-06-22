/*
	@file	SoundManager.cpp
	@brief	�T�E���h���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "SoundManager.h"


/*
	@brief	�R���X�g���N�^
*/
SoundManager::SoundManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
SoundManager::~SoundManager()
{
}

/* 
	@brief	������
*/
bool SoundManager::Init()
{
	m_pTitleBGM		= new Wave;
	m_pGameBGM		= new Wave;
	m_pDecideSE		= new Wave;
	m_pMenuSE		= new Wave;
	m_pDigSE		= new Wave;
	m_pDigMissSE	= new Wave;

	if (!m_pTitleBGM->Init(_T("../Resource/Sound/TitleBGM.wav")))	{ return false; }
	if (!m_pGameBGM->Init(_T("../Resource/Sound/GameBGM.wav")))		{ return false; }
	if (!m_pDecideSE->Init(_T("../Resource/Sound/Decide.wav")))	{ return false; }
	if (!m_pMenuSE->Init(_T("../Resource/Sound/Menu.wav")))		{ return false; }
	if(!m_pDigSE->Init(_T("../Resource/Sound/Dig.wav"))) { return false; }
	if (!m_pDigMissSE->Init(_T("../Resource/Sound/DigMiss.wav"))) { return false; }
	return true;
}

/*
	@brief	���
*/
void SoundManager::Destroy()
{
	SAFE_DELETE(m_pTitleBGM);
	SAFE_DELETE(m_pGameBGM);
	SAFE_DELETE(m_pDecideSE);
	SAFE_DELETE(m_pMenuSE);
	SAFE_DELETE(m_pDigSE);
	SAFE_DELETE(m_pDigMissSE);
}

/*
	@brief	BGM�Đ�
*/
void SoundManager::Play(eBGM bgm, bool isLoop)
{
	switch (bgm)
	{
	case SoundManager::TITLE:
		m_pTitleBGM->Play(isLoop);
		break;
	case SoundManager::GAME:
		m_pGameBGM->Play(isLoop);
		break;
	default:
		break;
	}
}

/*
	@brief	SE�Đ�
*/
void SoundManager::Play(eSE se, bool isLoop)
{
	switch (se)
	{
	case SoundManager::DECIDE:
		m_pDecideSE->Play(isLoop);
		break;
	case SoundManager::MENU:
		m_pMenuSE->Play(isLoop);
		break;
	case SoundManager::DIG:
		m_pDigSE->Play(isLoop);
		break;
	case SoundManager::DIGMISS:
		m_pDigMissSE->Play(isLoop);
		break;
	default:
		break;
	}
}

/*
	@brief	BGM��~
*/
void SoundManager::Stop(eBGM bgm)
{
	switch (bgm)
	{
	case SoundManager::TITLE:
		m_pTitleBGM->Stop();
		break;
	case SoundManager::GAME:
		m_pGameBGM->Stop();
		break;
	default:
		break;
	}
}

/*
	@brief	BGM����
*/
void SoundManager::Vol(eBGM bgm, UINT vol)
{
	switch (bgm)
	{
	case SoundManager::TITLE:
		m_pTitleBGM->SetVol(vol);
		break;
	case SoundManager::GAME:
		m_pGameBGM->SetVol(vol);
		break;
	default:
		break;
	}
}
