/*
	@file	SoundManager.h
	@brief	�T�E���h���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Sound/WAVE/Wave.h"

#define BGM SoundManager::GetInstance().eBGM
#define SE SoundManager::GetInstance().eSE


/*
	@brief	�T�E���h�̊Ǘ��N���X
*/
class SoundManager
{
public:
	/*! �f�X�g���N�^ */
	~SoundManager();

	/*! �V���O���g�� */
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/*! BGM */
	enum eBGM {
		TITLE,
		GAME,
	};

	/*! SE */
	enum eSE{
		DECIDE,
		MENU,
		DIG,
		DIGMISS
	};

	/*! �֐� */
	bool Init();
	void Destroy();
	void Play(eBGM bgm, bool isLoop);
	void Play(eSE se, bool isLoop);
	void Stop(eBGM bgm);
	void Vol(eBGM bgm,UINT vol);
private:
	/*! �R���X�g���N�^ */
	SoundManager();

	Wave* m_pTitleBGM;
	Wave* m_pGameBGM;

	Wave* m_pDecideSE;
	Wave* m_pMenuSE;
	Wave* m_pDigSE;
	Wave* m_pDigMissSE;
};

