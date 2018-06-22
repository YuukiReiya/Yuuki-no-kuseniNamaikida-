/*
	@file	Debug.h
	@brief	�f�o�b�O
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Common/Common.h"
/*
	@brief	�f�o�b�O�N���X
*/
class Debug
{
public:
	/*! �R���X�g���N�^ */
	Debug();
	/*! �f�X�g���N�^ */
	~Debug();

	/*
		@enum	COLOR
		@brief	�F
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

