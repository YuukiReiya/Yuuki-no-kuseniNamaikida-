/*
	@file	HeroManager.h
	@brief	勇者を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../List/HeroList/HeroList.h"
#include "../../../Texture/Texture.h"
#include "../TycoonManager/TycoonManager.h"
#include "../../../Debug/Debug.h"

/*
	@brief	勇者の管理クラス
*/
class HeroManager
{
public:
	/*! コンストラクタ */
	HeroManager();
	/*! デストラクタ */
	~HeroManager();

	/*! シングルトン */
	static HeroManager& GetInstance() {
		static HeroManager instance;
		return instance;
	}

	void Init();
	void Update();
	void Render();
	void Destroy();
	void AddHero(Hero* hero);
	Hero* GetHero(INT2 pos);
	Hero* GetHeroInfo();
	bool GetClearFlag() { return isClear; }
private:
	void TextureLoad();
	bool isClear;
	HeroList* m_pHeroList;
	Texture* m_pHeroTexture;
	Debug m_Debug;
};