
/*
	@file	Window.h
	@brief	表示するテキストウィンドウ
	@author	番場 宥輝
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	テキストウィンドウクラス
*/
class Window
{
public:
	/*! コンストラクタ */
	Window();
	/*! デストラクタ */
	~Window();

	/*
		@enum	ICON
		@brief	アイコン識別用
	*/
	enum ICON {
		NONE,
		P1,
		P2,
		P3,
		P4,
	};

	void Init();
	void Render(D3DXVECTOR2 pos);
	void Destroy();
	void DrawTextWindow(ICON icon = ICON::NONE);
	void CursorUpdate();
	void ResetCount() { m_CursorCount = 0; }
	void DrawDownCursor();
	void DrawRightCursor(D3DXVECTOR2 pos);
	void DrawHeroWin();
	void DrawWindow(float width, float height, D3DXVECTOR2 pos);
private:
	/*! 定数 */
	static unsigned int UPDATE_CURSOR;

	bool		isCursorActive;
	DWORD		m_CursorCount;
	Sprite*		m_pSprite;
	Sprite*		m_pIconSprite;
	Sprite*		m_pCursorSprite;
	Texture*	m_pTexture;
	Texture*	m_pIconTexture;
	Texture*	m_pCursorTexture;
};

