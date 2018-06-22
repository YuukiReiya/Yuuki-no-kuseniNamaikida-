/*
	@file	UiManager.h
	@brief	表示するUIを管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Texture/Texture.h"
#include "../../Window/Window.h"
#include <Map>

/*
	@brief	UIの管理クラス
*/
class UiManager
{
public:
	/*! デストラクタ */
	~UiManager();

	/*! シングルトン */
	static UiManager&GetInstance() {
		static UiManager instance;
		return instance;
	}

	/*! 関数 */
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

	/*! コンストラクタ */
	UiManager() = default;

	/*! 定数 */
	static const float FONT_WIDTH;				/*!< フォントの横幅 */
	static const float FONT_HEIGHT;				/*!< フォントの縦幅 */
	static const float NUMBER_WIDTH;			/*!< フォントの横幅 */
	static const float NUMBER_HEIGHT;			/*!< フォントの縦幅 */
	static const unsigned int MAX_TEXT_LINE;	/*!< 1行に収まる文字数 */

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

