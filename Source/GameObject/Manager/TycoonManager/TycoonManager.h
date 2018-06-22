/*
	@file	TycoonManager.h
	@brief	魔王を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../Character/Tycoon/Tycoon.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Texture/Texture.h"
#include "../../../Debug/Debug.h"
#include "../../../Camera/Camera2D.h"
#include "../../Character/Hero/Hero.h"

/*
	@brief	魔王の管理クラス
*/
class TycoonManager
{
public:
	/*! コンストラクタ */
	TycoonManager();
	/*! デストラクタ */
	~TycoonManager();

	/*! シングルトン */
	static TycoonManager& GetInstance() {
		static TycoonManager instance;
		return instance;
	}

	/*! 関数 */
	void Init();
	void Render();
	void Destroy();
	void SetHeroPos(INT2 pos);
	void Update();
	void SetTycoonPos(INT2 Pos);

	/*! ゲッター */
	INT2 GetTycoonPos() { return m_pTycoon->m_IndexPos; }
	bool GetCaughtFlags() { return m_pTycoon->isCaught; }
	bool GetGameOverFlags() { return m_isGameOver; }

	/*! セッター */
	void SetHeroData(float movePow) { m_pTycoon->m_MovePow = movePow; }
	void SetCaughtFlags(bool flags) { m_pTycoon->isCaught = flags; }
private:

	/*! 変数 */
	Tycoon*		m_pTycoon;
	INT2*		m_pHeroPos;
	bool		m_isGameOver;
	Texture*	m_pTexture;
	Debug		m_Debug;
};

