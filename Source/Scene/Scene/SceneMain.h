/*
	@file	SceneMain.cpp
	@brief	メインシーン
	@date	2017/12/04
	@author	番場 宥輝
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

/*デバッグ*/
#include "../../GameObject/Character/Hero/Soldier/Soldier.h"

/*
	@brief	メインシーンクラス
*/
class SceneMain:public Scene
{
public:
	/*! コンストラクタ */
	SceneMain();
	/*! デストラクタ */
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
		@brief	ゲームのフロー
	*/
	enum MODE {
		START,	/*!< シーン開始 */
		FREE,	/*!< ダンジョン開拓 */
		CALL,	/*!< 勇者呼び出し */
		SETTING,/*!< 魔王の位置セット */
		BATTLE,	/*!< 勇者いる */
		WIN,	/*!< 勝利 */
		LOSE,	/*!< 敗北 */
	}mode;


	static std::wstring START_TEXT[];	/*!< START表示テキスト */
	static unsigned int START_ICON[];	/*!< START表示アイコン */
	static std::wstring SETTING_TEXT[];	/*!< SETTING表示テキスト */
	static unsigned int SETTING_ICON[];	/*!< SETTING表示アイコン */
	static std::wstring WIN_TEXT[];		/*!< WIN表示テキスト */
	static unsigned int WIN_ICON[];		/*!< WIN表示アイコン */
	static std::wstring LOSE_TEXT[];	/*!< LOSE表示テキスト */
	static unsigned int LOSE_ICON[];	/*!< LOSE表示アイコン */

	/*
		@struct	PARAMETER
		@brief	パラメータ保存構造体数
	*/
	struct PARAMETER
	{
		int hp;
		int atk;
		float movePow;
	}param;


	/*! 変数 */
	Player* m_pPlayer;		/*!< プレイヤー */
	Debug	m_Debug;
	DWORD	m_Fps;			/*!< FPSカウント */
	DWORD	m_HeroComeFps;	/*!< 勇者が来るまでのフレーム */
	bool	m_isStop;		/*!<ゲームの更新処理を止めるフラグ*/
	int		m_TextItr;		/*!< テキストイテレータ */
	int		m_IconItr;		/*!< アイコンイテレータ */
	
};

