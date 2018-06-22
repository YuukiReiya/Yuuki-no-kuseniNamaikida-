
/*
	@file	Window.h
	@brief	�\������e�L�X�g�E�B���h�E
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	�e�L�X�g�E�B���h�E�N���X
*/
class Window
{
public:
	/*! �R���X�g���N�^ */
	Window();
	/*! �f�X�g���N�^ */
	~Window();

	/*
		@enum	ICON
		@brief	�A�C�R�����ʗp
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
	/*! �萔 */
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

