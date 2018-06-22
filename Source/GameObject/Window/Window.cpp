/*
	@file	Window.cpp
	@brief	表示するテキストウィンドウ
	@author	番場 宥輝
*/
#include "Window.h"

/*
	@def	定数宣言
*/
unsigned int Window::UPDATE_CURSOR = 60;

/*
	@brief	コンストラクタ
*/
Window::Window()
{
}

/*
	@brief	デストラクタ
*/
Window::~Window()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Window::Init()
{
	m_pSprite	=	new Sprite;
	m_pIconSprite = new Sprite;
	m_pCursorSprite = new Sprite;
	m_pTexture	=	new Texture;
	m_pIconTexture = new Texture;
	m_pCursorTexture = new Texture;

	m_pSprite->SetSpriteSize(1984, 384);
	m_pSprite->SetSizeScaling(0.95f);
	m_pIconSprite->SetSpriteSize(240, 240);
	m_pIconSprite->SetSizeScaling(1.3f);
	m_pIconSprite->SetSplit(5, 1);
	m_pCursorSprite->SetSpriteSize(16, 16);
	m_pCursorSprite->SetSizeScaling(2.5f);
	m_pCursorSprite->SetSplit(2, 1);

	m_CursorCount = 0;
	isCursorActive = true;

	if (!m_pTexture->Load(L"../Resource/Texture/t50.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pIconTexture->Load(L"../Resource/Texture/Icon.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pCursorTexture->Load(L"../Resource/Texture/cursor.png")) { MSG("テクスチャの読み込みに失敗"); }

}

/*
	@brief	下向きカーソル描画
*/
void Window::DrawDownCursor()
{
	if (!isCursorActive) { return; }
	D3DXVECTOR2 pos = {
		1800,
		1000
	};
	m_pCursorSprite->SetPos(pos);
	m_pCursorSprite->SetSpriteSize(16, 16);
	m_pCursorSprite->SetSizeScaling(2.5f);
	m_pCursorSprite->RenderAtlas(m_pCursorTexture->GetTexture(), 1, 0);
}

/*
	@brief	右向きカーソル描画
*/
void Window::DrawRightCursor(D3DXVECTOR2 pos)
{
	m_pCursorSprite->SetPos(pos);
	m_pCursorSprite->SetSpriteSize(16, 16);
	m_pCursorSprite->SetSizeScaling(5.0f);
	m_pCursorSprite->RenderAtlas(m_pCursorTexture->GetTexture(), 0, 0);
}

void Window::DrawHeroWin()
{
	D3DXVECTOR2 pos{
		30,
		50
	};
	m_pSprite->SetPos(pos);
	m_pSprite->SetSpriteSize(500, 100);
	m_pSprite->RenderLeftTop(m_pTexture->GetTexture());
}

/*
	@brief	ウィンドウの表示
*/
void Window::DrawWindow(float width, float height, D3DXVECTOR2 pos)
{
	m_pSprite->SetSpriteSize(width, height);
	m_pSprite->SetPos(pos);
	m_pSprite->Render(m_pTexture->GetTexture());
}

/*
	@brief	テキストウィンドウ描画
*/
void Window::DrawTextWindow(ICON icon)
{
	UINT u, v = 0;
	switch (icon)
	{
	case Window::NONE:
		u = 0;
		break;
	case Window::P1:
		u = 1;
		break;
	case Window::P2:
		u = 2;
		break;
	case Window::P3:
		u = 3;
		break;
	case Window::P4:
		u = 4;
		break;
	default:
		u = 0;
		break;
	}
	/*! テキストウィンドウ */
	m_pSprite->SetSpriteSize(1984, 384);
	m_pSprite->SetSizeScaling(0.95f);
	D3DXVECTOR2 pos{
		960,
		850
	};
	m_pSprite->SetPos(pos);
	m_pSprite->Render(m_pTexture->GetTexture());

	/*! 表示するアイコン */
	pos = { pos.x - 760,pos.y };
	m_pIconSprite->SetPos(pos);
	m_pIconSprite->RenderAtlas(m_pIconTexture->GetTexture(), u, v);
}

/*
	@brief	カーソルの表示更新
	@detail	点滅
*/
void Window::CursorUpdate()
{
	m_CursorCount++;
	if (m_CursorCount < UPDATE_CURSOR) { return; }
	m_CursorCount = 0;
	isCursorActive = !isCursorActive;
}


/*
	@brief	描画
*/
void Window::Render(D3DXVECTOR2 pos)
{
	m_pSprite->SetPos(pos);
	m_pSprite->Render(m_pTexture->GetTexture());

}

/*
	@brief	解放
*/
void Window::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pTexture);
	SAFE_DELETE(m_pIconSprite);
	SAFE_DELETE(m_pIconTexture);
	SAFE_DELETE(m_pCursorSprite);
	SAFE_DELETE(m_pCursorTexture);
}
