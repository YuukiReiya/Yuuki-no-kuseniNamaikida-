/*
	@file	Debug.cpp
	@brief	デバッグ	
	@author	番場 宥輝
*/
#include "Debug.h"

/*
	@var	m_hWnd
	@brief	静的メンバ変数
*/
HWND Debug::m_hWnd = NULL;

/*
	@brief	コンストラクタ
*/
Debug::Debug()
{
	m_pLine = NULL;
	m_Font = NULL;
}

/*
	@brief	デストラクタ
*/
Debug::~Debug()
{
}

/*
	@brief	初期化
*/
bool Debug::Init()
{
	HRESULT hr = D3DXCreateLine(DirectDevice::GetInstance().GetDevice(), &m_pLine);
	if (FAILED(hr)) {
		MSG("CreateLineに失敗");
		return false;
	}
	SetLineWidth(3.0);
	SetAntialias();
	hr = D3DXCreateFont(DirectDevice::GetInstance().GetDevice(), 24, 12,
		FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"ＭＳ ゴシック", &m_Font);
	if(FAILED(hr)){
		MSG("CreateFontに失敗");
		return false;
	}
		return true;
}

/*
	@brief	線の太さを設定
*/
void Debug::SetLineWidth(float Width)
{
	m_pLine->SetWidth(Width);
}

/*
	@brief	線のアンチエイリアスを設定
*/
void Debug::SetAntialias(bool isAntiAlias)
{
	m_pLine->SetAntialias(isAntiAlias);
}

/*
	@brief	線の描画
*/
void Debug::DrawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, COLOR color)
{
	D3DXVECTOR2 vector[] = {
		D3DXVECTOR2(start),	/*!< 始点 */
		D3DXVECTOR2(end)	/*!< 終点 */
	};
	m_pLine->Begin();
	m_pLine->Draw(vector, 2, ConvertColor(color));
	m_pLine->End();
}

/*
	@brief	四角形描画
*/
void Debug::DrawSquare(D3DXVECTOR2 Center, float Width, float Height, COLOR color)
{
	DrawLine(D3DXVECTOR2(Center.x - Width / 2, Center.y - Height / 2), D3DXVECTOR2(Center.x + Width / 2, Center.y - Height / 2), color);	//上
	DrawLine(D3DXVECTOR2(Center.x + Width / 2, Center.y - Height / 2), D3DXVECTOR2(Center.x + Width / 2, Center.y + Height / 2), color);	//右
	DrawLine(D3DXVECTOR2(Center.x + Width / 2, Center.y + Height / 2), D3DXVECTOR2(Center.x - Width / 2, Center.y + Height / 2), color);	//下
	DrawLine(D3DXVECTOR2(Center.x - Width / 2, Center.y + Height / 2), D3DXVECTOR2(Center.x - Width / 2, Center.y - Height / 2), color);	//左
}

void Debug::DebugText(std::string str)
{
	//SIFT_JIS→Unicode変換
	
}

/* 
	@brief	色の変換
	@detail	引数をD3DCOLOR型に変換し出力
*/
D3DCOLOR Debug::ConvertColor(COLOR color)
{
	D3DCOLOR cr;
	switch (color)
	{
	case Debug::WHITE:
		cr = D3DCOLOR_ARGB(255, 255, 255, 255);
		break;
	case Debug::RED:
		cr = (D3DCOLOR_ARGB(255, 255, 0, 0));
		break;
	case Debug::GREEN:
		cr = (D3DCOLOR_ARGB(255, 0, 255, 0));
		break;
	case Debug::BLUE:
		cr = (D3DCOLOR_ARGB(255, 0, 0, 255));
		break;
	case Debug::BLACK:
		cr = (D3DCOLOR_ARGB(255, 0, 0, 0));
		break;
	default:
		cr = (D3DCOLOR_ARGB(255, 255, 255, 0));
		break;
	}
	return cr;
}
