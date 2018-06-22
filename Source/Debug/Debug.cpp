/*
	@file	Debug.cpp
	@brief	�f�o�b�O	
	@author	�ԏ� �G�P
*/
#include "Debug.h"

/*
	@var	m_hWnd
	@brief	�ÓI�����o�ϐ�
*/
HWND Debug::m_hWnd = NULL;

/*
	@brief	�R���X�g���N�^
*/
Debug::Debug()
{
	m_pLine = NULL;
	m_Font = NULL;
}

/*
	@brief	�f�X�g���N�^
*/
Debug::~Debug()
{
}

/*
	@brief	������
*/
bool Debug::Init()
{
	HRESULT hr = D3DXCreateLine(DirectDevice::GetInstance().GetDevice(), &m_pLine);
	if (FAILED(hr)) {
		MSG("CreateLine�Ɏ��s");
		return false;
	}
	SetLineWidth(3.0);
	SetAntialias();
	hr = D3DXCreateFont(DirectDevice::GetInstance().GetDevice(), 24, 12,
		FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"�l�r �S�V�b�N", &m_Font);
	if(FAILED(hr)){
		MSG("CreateFont�Ɏ��s");
		return false;
	}
		return true;
}

/*
	@brief	���̑�����ݒ�
*/
void Debug::SetLineWidth(float Width)
{
	m_pLine->SetWidth(Width);
}

/*
	@brief	���̃A���`�G�C���A�X��ݒ�
*/
void Debug::SetAntialias(bool isAntiAlias)
{
	m_pLine->SetAntialias(isAntiAlias);
}

/*
	@brief	���̕`��
*/
void Debug::DrawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, COLOR color)
{
	D3DXVECTOR2 vector[] = {
		D3DXVECTOR2(start),	/*!< �n�_ */
		D3DXVECTOR2(end)	/*!< �I�_ */
	};
	m_pLine->Begin();
	m_pLine->Draw(vector, 2, ConvertColor(color));
	m_pLine->End();
}

/*
	@brief	�l�p�`�`��
*/
void Debug::DrawSquare(D3DXVECTOR2 Center, float Width, float Height, COLOR color)
{
	DrawLine(D3DXVECTOR2(Center.x - Width / 2, Center.y - Height / 2), D3DXVECTOR2(Center.x + Width / 2, Center.y - Height / 2), color);	//��
	DrawLine(D3DXVECTOR2(Center.x + Width / 2, Center.y - Height / 2), D3DXVECTOR2(Center.x + Width / 2, Center.y + Height / 2), color);	//�E
	DrawLine(D3DXVECTOR2(Center.x + Width / 2, Center.y + Height / 2), D3DXVECTOR2(Center.x - Width / 2, Center.y + Height / 2), color);	//��
	DrawLine(D3DXVECTOR2(Center.x - Width / 2, Center.y + Height / 2), D3DXVECTOR2(Center.x - Width / 2, Center.y - Height / 2), color);	//��
}

void Debug::DebugText(std::string str)
{
	//SIFT_JIS��Unicode�ϊ�
	
}

/* 
	@brief	�F�̕ϊ�
	@detail	������D3DCOLOR�^�ɕϊ����o��
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
