/*
	@file	SoundManager.h
	@brief	サウンドを管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../../Sound/WAVE/Wave.h"

#define BGM SoundManager::GetInstance().eBGM
#define SE SoundManager::GetInstance().eSE


/*
	@brief	サウンドの管理クラス
*/
class SoundManager
{
public:
	/*! デストラクタ */
	~SoundManager();

	/*! シングルトン */
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

	/*! 関数 */
	bool Init();
	void Destroy();
	void Play(eBGM bgm, bool isLoop);
	void Play(eSE se, bool isLoop);
	void Stop(eBGM bgm);
	void Vol(eBGM bgm,UINT vol);
private:
	/*! コンストラクタ */
	SoundManager();

	Wave* m_pTitleBGM;
	Wave* m_pGameBGM;

	Wave* m_pDecideSE;
	Wave* m_pMenuSE;
	Wave* m_pDigSE;
	Wave* m_pDigMissSE;
};

