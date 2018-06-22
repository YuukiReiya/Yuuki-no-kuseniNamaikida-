/*
	@file	UiManager.h
	@brief	�\������UI���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Texture/Texture.h"
#include "../../Window/Window.h"
#include <Map>

/*
	@brief	UI�̊Ǘ��N���X
*/
class UiManager
{
public:
	/*! �f�X�g���N�^ */
	~UiManager();

	/*! �V���O���g�� */
	static UiManager&GetInstance() {
		static UiManager instance;
		return instance;
	}

	/*! �֐� */
	void Init();
	void Destroy();
	void DrawDigPower(int power);
	void DrawMessage(std::wstring str, Window::ICON icon = Window::ICON::NONE);
	void DrawHeroData();
	void Counter();
	void CounterReset() { window->ResetCount(); }
	void DrawString(std::wstring str,float scale,D3DXVECTOR2 pos);
	void DrawLevel(D3DXVECTOR2 pos);
	void DrawCursolRight(D3DXVECTOR2 pos);
private:
	void DrawNumberB(int val, float scale, D3DXVECTOR2 pos);
	void DrawNumberW(int val, float scale, D3DXVECTOR2 pos);
	void CreateFontData();
	void CreateNumberData();
	std::map<wchar_t, INT2>font;
	std::map<int, INT2>number;

	/*! �R���X�g���N�^ */
	UiManager() = default;

	/*! �萔 */
	static const float FONT_WIDTH;				/*!< �t�H���g�̉��� */
	static const float FONT_HEIGHT;				/*!< �t�H���g�̏c�� */
	static const float NUMBER_WIDTH;			/*!< �t�H���g�̉��� */
	static const float NUMBER_HEIGHT;			/*!< �t�H���g�̏c�� */
	static const unsigned int MAX_TEXT_LINE;	/*!< 1�s�Ɏ��܂镶���� */

	Window* window;

	float m_DigPowerFontSize;
	Sprite* m_pDigPowerSprite;
	Sprite* m_pStringSprite;
	Sprite* m_pNumberBSprite;
	Sprite* m_pNumberWSprite;
	Sprite* m_pHpSprite;

	Texture* m_pDigPowerTexture;
	Texture* m_pNumberBTexture;
	Texture* m_pNumberWTexture;
	Texture* m_pStringTexture;
	Texture* m_pHpTexture;

};

