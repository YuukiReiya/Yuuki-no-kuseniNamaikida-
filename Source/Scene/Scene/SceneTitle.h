/*
	@file	SceneTitle.h
	@brief	タイトルシーン
	@author	番場 宥輝
*/
#pragma once
#include "../SceneBase/Scene.h"
#include "../Scene/SceneMain.h"
#include "../../Common/Common.h"
#include "../../GameObject/Window/Window.h"

/*
	@brief	タイトルシーンクラス
*/
class SceneTitle :
	public Scene
{
public:
	/*! コンストラクタ */
	SceneTitle();
	/*! デストラクタ */
	~SceneTitle();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();

	/*
		@enum	LEVEL
		@brief	ゲームの難易度
	*/
	enum LEVEL {
		EASY,
		NORMAL,
		HARD,
	}static level;

	/*
		@enum	MODE
		@brief	ゲームのフロー
	*/
	enum MODE {
		START,	/*!< ゲーム起動時 */
		SELECT,	/*!< セレクト */
		CLEAR,	/*!< ステージクリア */
		LOSE,	/*! ステージ失敗 */
		PERFECT,/*!< ゲームクリア */
		END,	/*!< ゲーム終了 */
	}static mode;
	static void SetMode(MODE val) { mode = val; }
	static MODE GetMode() { return mode; }
	static LEVEL GetLevel() { return level; }
private:

	void SetUpText();
	bool SelectUpdate();
	void SelectRender();
	void DrawLevel();

	/*! 定数宣言 */
	static std::wstring START_TEXT[];	/*!< START表示テキスト */
	static unsigned int START_ICON[];	/*!< START表示アイコン */
	static std::wstring SELECT_TEXT[];	/*!< SELECT表示テキスト */
	static unsigned int SELECT_ICON[];	/*!< SELECT表示アイコン */
	static std::wstring CLEAR_TEXT[];	/*!< CLEAR表示テキスト */
	static unsigned int CLEAR_ICON[];	/*!< CLEAR表示アイコン */
	static std::wstring LOSE_TEXT[];	/*!< LOSE表示テキスト */
	static unsigned int LOSE_ICON[];	/*!< LOSE表示アイコン */
	static std::wstring PERFECT_TEXT[];	/*!< PERFECT表示テキスト */
	static unsigned int PERFECT_ICON[];	/*!< PERFECT表示アイコン */
	static std::wstring END_TEXT[];		/*!< END表示テキスト */
	static unsigned int END_ICON[];		/*!< END表示アイコン */

	int			m_TextItr;				/*!< テキスト操作のイテレータ */
	UINT		m_IconItr;				/*!< テキストのアイコン操作イテレータ */
	Sprite*		m_pBackGroundSprite;	/*!< 背景スプライト */
	Texture*	m_pBackGroundTexture;	/*!< 背景テクスチャ */
};

