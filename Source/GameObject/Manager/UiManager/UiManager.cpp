/*
	@file	UiManager.h
	@brief	�\������UI���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "UiManager.h"
#include "../HeroManager/HeroManager.h"

/*
	@def	�萔�錾
*/
const float UiManager::FONT_WIDTH = 10;
const float UiManager::FONT_HEIGHT = 11;
const float UiManager::NUMBER_WIDTH = 32;
const float UiManager::NUMBER_HEIGHT = 32;
const unsigned int UiManager::MAX_TEXT_LINE = 17;

/*
	@brief	�f�X�g���N�^
*/
UiManager::~UiManager()
{
	Destroy();
}

/*
	@brief	������
*/
void UiManager::Init()
{
	m_pDigPowerSprite = new Sprite;
	m_pStringSprite = new Sprite;
	m_pNumberBSprite = new Sprite;
	m_pNumberWSprite = new Sprite;
	m_pHpSprite = new Sprite;


	m_pDigPowerTexture = new Texture;
	m_pStringTexture = new Texture;
	m_pNumberBTexture = new Texture;
	m_pNumberWTexture = new Texture;
	m_pHpTexture = new Texture;

	window = new Window;

	m_pStringSprite->SetSpriteSize(FONT_WIDTH, FONT_HEIGHT);
	m_pStringSprite->SetSplit(12, 13);
	m_pDigPowerSprite->SetSpriteSize(420, 100);
	m_pDigPowerSprite->SetSizeScaling(0.7f);
	m_pNumberBSprite->SetSpriteSize(32, 32);
	m_pNumberBSprite->SetSizeScaling(2);
	m_pNumberBSprite->SetSplit(10, 1);
	m_pNumberWSprite->SetSpriteSize(120, 120);
	m_pNumberWSprite->SetSizeScaling(0.5f);
	m_pNumberWSprite->SetSplit(10, 1);
	m_pHpSprite->SetSpriteSize(120, 120);
	m_pHpSprite->SetSizeScaling(0.6f);

	if (!m_pDigPowerTexture->Load(L"../Resource/Texture/�x�p���[.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }
	if (!m_pStringTexture->Load(L"../Resource/Texture/font.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }
	if (!m_pNumberBTexture->Load(L"../Resource/Texture/NoB.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }
	if (!m_pNumberWTexture->Load(L"../Resource/Texture/NoW.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }
	if (!m_pHpTexture->Load(L"../Resource/Texture/hp.png")) { MSG("�e�N�X�`���̓ǂݍ��݂Ɏ��s"); }

	CreateFontData();
	CreateNumberData();
	window->Init();
}

/*
	@brief	���
*/
void UiManager::Destroy()
{
	SAFE_DELETE(m_pDigPowerSprite);
	SAFE_DELETE(m_pDigPowerTexture);
	SAFE_DELETE(m_pStringSprite);
	SAFE_DELETE(m_pStringTexture);
	SAFE_DELETE(m_pNumberBSprite);
	SAFE_DELETE(m_pNumberBTexture);
	SAFE_DELETE(m_pNumberWSprite);
	SAFE_DELETE(m_pNumberWTexture);
	SAFE_DELETE(m_pHpSprite);
	SAFE_DELETE(m_pHpTexture);
	SAFE_DELETE(window);
}

/*
	@brief	�x�p���[�\��
*/
void UiManager::DrawDigPower(int power)
{
	D3DXVECTOR2 pos{
		1400,
		30
	};
	m_pDigPowerSprite->SetPos(pos);
	m_pDigPowerSprite->RenderLeftTop(m_pDigPowerTexture->GetTexture());
	
	DrawNumberB(power, 1.7f, { 1800,30 });

}

/*
	@brief	�e�L�X�g�E�B���h�E�ƕ������\��
*/
void UiManager::DrawMessage(std::wstring str, Window::ICON icon)
{
	window->DrawTextWindow(icon);
	DrawString(str, 8.5f, {430,760 });
	window->DrawDownCursor();
}

/*
	@brief	�E�҂̏���\��
*/
void UiManager::DrawHeroData()
{
	auto hero = HeroManager::GetInstance().GetHeroInfo();
	if (!hero) { return; }
	window->DrawHeroWin();
	D3DXVECTOR2 pos{
		65,
		100
	};
	DrawString(L"���E�L ", 5, pos);
	
	pos = {
		200,
		65
	};
	m_pHpSprite->SetPos(pos);
	m_pHpSprite->RenderLeftTop(m_pHpTexture->GetTexture());
	
	pos = {
		380,
		70
	};
	DrawNumberW(hero->GetHp(), 1.5f, pos);
}

/*
	@brief	�J�E���^����
*/
void UiManager::Counter()
{
	window->CursorUpdate();
}

/*
	@brief	������̕`��
	@detail	1�s������̕������𒴂��Ă�����i���𕪂���
*/
void UiManager::DrawString(std::wstring str,float scale, D3DXVECTOR2 pos)
{
	/*! �t�H���g�T�C�Y */
	m_pStringSprite->SetSpriteSize(FONT_WIDTH, FONT_HEIGHT);
	m_pStringSprite->SetSizeScaling(scale);

	UINT line = str.size() / (MAX_TEXT_LINE + 1) + 1;/*!< �����������s�� */

	/*! �s���[�v */
	for (UINT i = 0; i < line; i++) {
		int spcount = 0;/*!< �J�K�̗�O�I���� */
		/*! �����񃋁[�v */
		for (UINT j = 0; j < MAX_TEXT_LINE; j++) {
			wchar_t c = str[j + i * MAX_TEXT_LINE];
			if (c == '\0') { break; }
			INT2 UV = font[c];
			D3DXVECTOR2 strPos = {
				pos.x + FONT_WIDTH*scale*j,
				pos.y + FONT_HEIGHT*scale*i
			};
			strPos = {
				strPos.x - spcount * 60,
				strPos.y,
			};
			/*! �J���́K */
			if (c == L'�J' || c == L'�K') {
				strPos.x -= 15;
				strPos.y -= 10;
				spcount++;
			}
			m_pStringSprite->SetPos(strPos);
			m_pStringSprite->RenderAtlas(m_pStringTexture->GetTexture(), UV.x, UV.y);
		}
	}

//	str
}

/*
	@brief	���x���\��
*/
void UiManager::DrawLevel(D3DXVECTOR2 pos)
{
	window->DrawWindow(600, 200, pos);
}

/*
	@brief	�E�����O�p�`��
*/
void UiManager::DrawCursolRight(D3DXVECTOR2 pos)
{
	window->DrawRightCursor(pos);
}

/*
	@brief	�������`��
	@detail	�E�l�߂ŕ`��
*/
void UiManager::DrawNumberB(int val, float scale, D3DXVECTOR2 pos)
{
	char dummy[10];/*!< �_�~�[�o�b�t�@ */
	int count = sprintf(dummy, "%d", val);/*!< ���������߂� */

	for (int i = 0; i < count; i++) {
		/*! 1�̈� */
		int index = val % 10;

		/*! �E�l�� */
		D3DXVECTOR2 iPos = {
			pos.x - NUMBER_WIDTH * (scale - 0.2f) * i,
			pos.y,
		};
		m_pNumberBSprite->SetPos(iPos);
		INT2 num = number[index];
		m_pNumberBSprite->RenderAtlasLeftTop(m_pNumberBTexture->GetTexture(), num.x, num.y);
		/*! ���l�� */
		val /= 10;
	}
}

/*
	@brief	�������`��
	@detail	�E�l�߂ŕ`��
*/
void UiManager::DrawNumberW(int val, float scale, D3DXVECTOR2 pos)
{
	char dummy[10];/*!< �_�~�[�o�b�t�@ */
	int count = sprintf(dummy, "%d", val);/*!< ���������߂� */

	for (int i = 0; i < count; i++) {
		/*! 1�̈� */
		int index = val % 10;

		/*! �E�l�� */
		D3DXVECTOR2 iPos = {
			pos.x - 36 * (scale) * i,
			pos.y,
		};
		m_pNumberWSprite->SetPos(iPos);
		INT2 num = number[index];
		m_pNumberWSprite->RenderAtlasLeftTop(m_pNumberWTexture->GetTexture(), num.x, num.y);
		/*! ���l�� */
		val /= 10;
	}
}

/*
	@brief	�t�H���g�f�[�^�̍쐬
	@detail	�A�z�z��ɕ����̓o�^
*/
void UiManager::CreateFontData()
{
	/*! ���`�� */
	font[L'��'] = { 0,0 };
	font[L'��'] = { 1,0 };
	font[L'��'] = { 2,0 };
	font[L'��'] = { 3,0 };
	font[L'��'] = { 4,0 };
	font[L'��'] = { 5,0 };
	font[L'��'] = { 6,0 };
	font[L'��'] = { 7,0 };
	font[L'��'] = { 8,0 };
	font[L'��'] = { 9,0 };
	font[L'��'] = { 10,0 };
	font[L'��'] = { 11,0 };
	/*! ���`�� */
	font[L'��'] = { 0,1 };
	font[L'��'] = { 1,1 };
	font[L'��'] = { 2,1 };
	font[L'��'] = { 3,1 };
	font[L'��'] = { 4,1 };
	font[L'��'] = { 5,1 };
	font[L'��'] = { 6,1 };
	font[L'��'] = { 7,1 };
	font[L'��'] = { 8,1 };
	font[L'��'] = { 9,1 };
	font[L'��'] = { 10,1 };
	font[L'��'] = { 11,1 };
	/*! �́`�� */
	font[L'��'] = { 0,2 };
	font[L'��'] = { 1,2 };
	font[L'��'] = { 2,2 };
	font[L'��'] = { 3,2 };
	font[L'��'] = { 4,2 };
	font[L'��'] = { 5,2 };
	font[L'��'] = { 6,2 };
	font[L'��'] = { 7,2 };
	font[L'��'] = { 8,2 };
	font[L'��'] = { 9,2 };
	font[L'��'] = { 10,2 };
	font[L'��'] = { 11,2 };
	/*! ��`�� */
	font[L'��'] = { 0,3 };
	font[L'��'] = { 1,3 };
	font[L'��'] = { 2,3 };
	font[L'��'] = { 3,3 };
	font[L'��'] = { 4,3 };
	font[L'��'] = { 5,3 };
	font[L'��'] = { 6,3 };
	font[L'��'] = { 7,3 };
	font[L'��'] = { 8,3 };
	font[L'��'] = { 9,3 };
	/*! �A�A�C */
	font[L'�A'] = { 10,3 };
	font[L'�C'] = { 11,3 };
	/*! �E�`�Z */
	font[L'�E'] = { 0,4 };
	font[L'�G'] = { 1,4 };
	font[L'�I'] = { 2,4 };
	font[L'�J'] = { 3,4 };
	font[L'�L'] = { 4,4 };
	font[L'�N'] = { 5,4 };
	font[L'�P'] = { 6,4 };
	font[L'�R'] = { 7,4 };
	font[L'�T'] = { 8,4 };
	font[L'�V'] = { 9,4 };
	font[L'�X'] = { 10,4 };
	font[L'�Z'] = { 11,4 };
	/*! �\�`�n */
	font[L'�\'] = { 0,5 };
	font[L'�^'] = { 1,5 };
	font[L'�`'] = { 2,5 };
	font[L'�c'] = { 3,5 };
	font[L'�e'] = { 4,5 };
	font[L'�g'] = { 5,5 };
	font[L'�i'] = { 6,5 };
	font[L'�j'] = { 7,5 };
	font[L'�k'] = { 8,5 };
	font[L'�l'] = { 9,5 };
	font[L'�m'] = { 10,5 };
	font[L'�n'] = { 11,5 };
	/*! �q�`�� */
	font[L'�q'] = { 0,6 };
	font[L'�t'] = { 1,6 };
	font[L'�w'] = { 2,6 };
	font[L'�z'] = { 3,6 };
	font[L'�}'] = { 4,6 };
	font[L'�~'] = { 5,6 };
	font[L'��'] = { 6,6 };
	font[L'��'] = { 7,6 };
	font[L'��'] = { 8,6 };
	font[L'��'] = { 9,6 };
	font[L'��'] = { 10,6 };
	font[L'��'] = { 11,6 };
	/*! ���`�� */
	font[L'��'] = { 0,7 };
	font[L'��'] = { 1,7 };
	font[L'��'] = { 2,7 };
	font[L'��'] = { 3,7 };
	font[L'��'] = { 4,7 };
	font[L'��'] = { 5,7 };
	font[L'��'] = { 6,7 };
	font[L'��'] = { 7,7 };
	/*! 0�`3 */
	font[L'0'] = { 8,7 };
	font[L'1'] = { 9,7 };
	font[L'2'] = { 10,7 };
	font[L'3'] = { 11,7 };
	/*! 4�`9 */
	font[L'4'] = { 0,8 };
	font[L'5'] = { 1,8 };
	font[L'6'] = { 2,8 };
	font[L'7'] = { 3,8 };
	font[L'8'] = { 4,8 };
	font[L'9'] = { 5,8 };
	/*! a�`f */
	font[L'a'] = { 6,8 };
	font[L'b'] = { 7,8 };
	font[L'c'] = { 8,8 };
	font[L'd'] = { 9,8 };
	font[L'e'] = { 10,8 };
	font[L'f'] = { 11,8 };
	/*! g�`r */
	font[L'g'] = { 0,9 };
	font[L'h'] = { 1,9 };
	font[L'i'] = { 2,9 };
	font[L'j'] = { 3,9 };
	font[L'k'] = { 4,9 };
	font[L'l'] = { 5,9 };
	font[L'm'] = { 6,9 };
	font[L'n'] = { 7,9 };
	font[L'o'] = { 8,9 };
	font[L'p'] = { 9,9 };
	font[L'q'] = { 10,9 };
	font[L'r'] = { 11,9 };
	/*! s�`z */
	font[L's'] = { 0,10 };
	font[L't'] = { 1,10 };
	font[L'u'] = { 2,10 };
	font[L'v'] = { 3,10 };
	font[L'w'] = { 4,10 };
	font[L'x'] = { 5,10 };
	font[L'y'] = { 6,10 };
	font[L'z'] = { 7,10 };
	/*! �E,�A�B�� */
	font[L'.'] = { 8,10 };
	font[L','] = { 9,10 };
	font[L'�A'] = { 10,10 };
	font[L'�B'] = { 11,10 };
	/*! ���`�� */
	font[L'��'] = { 0,11 };
	font[L'��'] = { 1,11 };
	font[L'��'] = { 2,11 };
	font[L'��'] = { 3,11 };
	font[L'��'] = { 4,11 };
	font[L'��'] = { 5,11 };
	font[L'��'] = { 6,11 };
	font[L'��'] = { 7,11 };
	font[L'��'] = { 8,11 };
	/*! �J�K�[ */
	font[L'�J'] = { 9,11 };
	font[L'�K'] = { 10,11 };
	font[L'�['] = { 11,11 };
	/*! �u�v�E�I�H�i�j���F�����c */
	font[L'�u'] = { 0,12 };
	font[L'�v'] = { 1,12 };
	font[L'�E'] = { 2,12 };
	font[L'�I'] = { 3,12 };
	font[L'�H'] = { 4,12 };
	font[L'�i'] = { 5,12 };
	font[L'�j'] = { 6,12 };
	font[L'��'] = { 7,12 };
	font[L'�F'] = { 8,12 };
	font[L'��'] = { 9,12 };
	font[L'��'] = { 10,12 };
	font[L'�c'] = { 11,12 };
	/*! �� */
	font[L' '] = { 13,13 };
	font[L'�@'] = { 13,13 };

}

/*
	@brief	�����̃f�[�^�̍쐬
	@detail	�A�z�z��ɐ����̓o�^
*/
void UiManager::CreateNumberData()
{
	number[0] = { 0,0 };
	number[1] = { 1,0 };
	number[2] = { 2,0 };
	number[3] = { 3,0 };
	number[4] = { 4,0 };
	number[5] = { 5,0 };
	number[6] = { 6,0 };
	number[7] = { 7,0 };
	number[8] = { 8,0 };
	number[9] = { 9,0 };
}
