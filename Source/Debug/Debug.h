/*
	@file	Debug.h
	@brief	デバッグ
	@author	番場 宥輝
*/
#pragma once
#include "../Common/Common.h"
/*
	@brief	デバッグクラス
*/
class Debug
{
public:
	/*! コンストラクタ */
	Debug();
	/*! デストラクタ */
	~Debug();

	/*
		@enum	COLOR
		@brief	色
	*/
	enum COLOR {
		WHITE,
		RED,
		GREEN,
		BLUE,
		BLACK
	};

	bool Init();
	void SetLineWidth(float Width);
	void SetAntialias(bool isAntiAlias = true);
	void DrawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, COLOR color = WHITE);
	void DrawSquare(D3DXVECTOR2 Center,float Width,float Height,COLOR color = WHITE);
	void DebugText(std::string str);

	static HWND m_hWnd;
private:
	ID3DXLine*	m_pLine;
	LPD3DXFONT  m_Font;
	D3DCOLOR ConvertColor(COLOR color);
};

