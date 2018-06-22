/*
	@file	UiManager.h
	@brief	表示するUIを管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "UiManager.h"
#include "../HeroManager/HeroManager.h"

/*
	@def	定数宣言
*/
const float UiManager::FONT_WIDTH = 10;
const float UiManager::FONT_HEIGHT = 11;
const float UiManager::NUMBER_WIDTH = 32;
const float UiManager::NUMBER_HEIGHT = 32;
const unsigned int UiManager::MAX_TEXT_LINE = 17;

/*
	@brief	デストラクタ
*/
UiManager::~UiManager()
{
	Destroy();
}

/*
	@brief	初期化
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

	if (!m_pDigPowerTexture->Load(L"../Resource/Texture/堀パワー.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pStringTexture->Load(L"../Resource/Texture/font.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pNumberBTexture->Load(L"../Resource/Texture/NoB.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pNumberWTexture->Load(L"../Resource/Texture/NoW.png")) { MSG("テクスチャの読み込みに失敗"); }
	if (!m_pHpTexture->Load(L"../Resource/Texture/hp.png")) { MSG("テクスチャの読み込みに失敗"); }

	CreateFontData();
	CreateNumberData();
	window->Init();
}

/*
	@brief	解放
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
	@brief	堀パワー表示
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
	@brief	テキストウィンドウと文字列を表示
*/
void UiManager::DrawMessage(std::wstring str, Window::ICON icon)
{
	window->DrawTextWindow(icon);
	DrawString(str, 8.5f, {430,760 });
	window->DrawDownCursor();
}

/*
	@brief	勇者の情報を表示
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
	DrawString(L"ユウキ ", 5, pos);
	
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
	@brief	カウンタを回す
*/
void UiManager::Counter()
{
	window->CursorUpdate();
}

/*
	@brief	文字列の描画
	@detail	1行当たりの文字数を超えていたら段落を分ける
*/
void UiManager::DrawString(std::wstring str,float scale, D3DXVECTOR2 pos)
{
	/*! フォントサイズ */
	m_pStringSprite->SetSpriteSize(FONT_WIDTH, FONT_HEIGHT);
	m_pStringSprite->SetSizeScaling(scale);

	UINT line = str.size() / (MAX_TEXT_LINE + 1) + 1;/*!< 文字数が何行か */

	/*! 行ループ */
	for (UINT i = 0; i < line; i++) {
		int spcount = 0;/*!< ゛゜の例外的文字 */
		/*! 文字列ループ */
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
			/*! ゛又は゜ */
			if (c == L'゛' || c == L'゜') {
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
	@brief	レベル表示
*/
void UiManager::DrawLevel(D3DXVECTOR2 pos)
{
	window->DrawWindow(600, 200, pos);
}

/*
	@brief	右向き三角描画
*/
void UiManager::DrawCursolRight(D3DXVECTOR2 pos)
{
	window->DrawRightCursor(pos);
}

/*
	@brief	黒数字描画
	@detail	右詰めで描画
*/
void UiManager::DrawNumberB(int val, float scale, D3DXVECTOR2 pos)
{
	char dummy[10];/*!< ダミーバッファ */
	int count = sprintf(dummy, "%d", val);/*!< 桁数を求める */

	for (int i = 0; i < count; i++) {
		/*! 1の位 */
		int index = val % 10;

		/*! 右詰め */
		D3DXVECTOR2 iPos = {
			pos.x - NUMBER_WIDTH * (scale - 0.2f) * i,
			pos.y,
		};
		m_pNumberBSprite->SetPos(iPos);
		INT2 num = number[index];
		m_pNumberBSprite->RenderAtlasLeftTop(m_pNumberBTexture->GetTexture(), num.x, num.y);
		/*! 桁詰め */
		val /= 10;
	}
}

/*
	@brief	白数字描画
	@detail	右詰めで描画
*/
void UiManager::DrawNumberW(int val, float scale, D3DXVECTOR2 pos)
{
	char dummy[10];/*!< ダミーバッファ */
	int count = sprintf(dummy, "%d", val);/*!< 桁数を求める */

	for (int i = 0; i < count; i++) {
		/*! 1の位 */
		int index = val % 10;

		/*! 右詰め */
		D3DXVECTOR2 iPos = {
			pos.x - 36 * (scale) * i,
			pos.y,
		};
		m_pNumberWSprite->SetPos(iPos);
		INT2 num = number[index];
		m_pNumberWSprite->RenderAtlasLeftTop(m_pNumberWTexture->GetTexture(), num.x, num.y);
		/*! 桁詰め */
		val /= 10;
	}
}

/*
	@brief	フォントデータの作成
	@detail	連想配列に文字の登録
*/
void UiManager::CreateFontData()
{
	/*! あ～し */
	font[L'あ'] = { 0,0 };
	font[L'い'] = { 1,0 };
	font[L'う'] = { 2,0 };
	font[L'え'] = { 3,0 };
	font[L'お'] = { 4,0 };
	font[L'か'] = { 5,0 };
	font[L'き'] = { 6,0 };
	font[L'く'] = { 7,0 };
	font[L'け'] = { 8,0 };
	font[L'こ'] = { 9,0 };
	font[L'さ'] = { 10,0 };
	font[L'し'] = { 11,0 };
	/*! す～ね */
	font[L'す'] = { 0,1 };
	font[L'せ'] = { 1,1 };
	font[L'そ'] = { 2,1 };
	font[L'た'] = { 3,1 };
	font[L'ち'] = { 4,1 };
	font[L'つ'] = { 5,1 };
	font[L'て'] = { 6,1 };
	font[L'と'] = { 7,1 };
	font[L'な'] = { 8,1 };
	font[L'に'] = { 9,1 };
	font[L'ぬ'] = { 10,1 };
	font[L'ね'] = { 11,1 };
	/*! の～や */
	font[L'の'] = { 0,2 };
	font[L'は'] = { 1,2 };
	font[L'ひ'] = { 2,2 };
	font[L'ふ'] = { 3,2 };
	font[L'へ'] = { 4,2 };
	font[L'ほ'] = { 5,2 };
	font[L'ま'] = { 6,2 };
	font[L'み'] = { 7,2 };
	font[L'む'] = { 8,2 };
	font[L'め'] = { 9,2 };
	font[L'も'] = { 10,2 };
	font[L'や'] = { 11,2 };
	/*! ゆ～ん */
	font[L'ゆ'] = { 0,3 };
	font[L'よ'] = { 1,3 };
	font[L'ら'] = { 2,3 };
	font[L'り'] = { 3,3 };
	font[L'る'] = { 4,3 };
	font[L'れ'] = { 5,3 };
	font[L'ろ'] = { 6,3 };
	font[L'わ'] = { 7,3 };
	font[L'を'] = { 8,3 };
	font[L'ん'] = { 9,3 };
	/*! ア、イ */
	font[L'ア'] = { 10,3 };
	font[L'イ'] = { 11,3 };
	/*! ウ～セ */
	font[L'ウ'] = { 0,4 };
	font[L'エ'] = { 1,4 };
	font[L'オ'] = { 2,4 };
	font[L'カ'] = { 3,4 };
	font[L'キ'] = { 4,4 };
	font[L'ク'] = { 5,4 };
	font[L'ケ'] = { 6,4 };
	font[L'コ'] = { 7,4 };
	font[L'サ'] = { 8,4 };
	font[L'シ'] = { 9,4 };
	font[L'ス'] = { 10,4 };
	font[L'セ'] = { 11,4 };
	/*! ソ～ハ */
	font[L'ソ'] = { 0,5 };
	font[L'タ'] = { 1,5 };
	font[L'チ'] = { 2,5 };
	font[L'ツ'] = { 3,5 };
	font[L'テ'] = { 4,5 };
	font[L'ト'] = { 5,5 };
	font[L'ナ'] = { 6,5 };
	font[L'ニ'] = { 7,5 };
	font[L'ヌ'] = { 8,5 };
	font[L'ネ'] = { 9,5 };
	font[L'ノ'] = { 10,5 };
	font[L'ハ'] = { 11,5 };
	/*! ヒ～ヨ */
	font[L'ヒ'] = { 0,6 };
	font[L'フ'] = { 1,6 };
	font[L'ヘ'] = { 2,6 };
	font[L'ホ'] = { 3,6 };
	font[L'マ'] = { 4,6 };
	font[L'ミ'] = { 5,6 };
	font[L'ム'] = { 6,6 };
	font[L'メ'] = { 7,6 };
	font[L'モ'] = { 8,6 };
	font[L'ヤ'] = { 9,6 };
	font[L'ユ'] = { 10,6 };
	font[L'ヨ'] = { 11,6 };
	/*! ラ～ン */
	font[L'ラ'] = { 0,7 };
	font[L'リ'] = { 1,7 };
	font[L'ル'] = { 2,7 };
	font[L'レ'] = { 3,7 };
	font[L'ロ'] = { 4,7 };
	font[L'ワ'] = { 5,7 };
	font[L'ヲ'] = { 6,7 };
	font[L'ン'] = { 7,7 };
	/*! 0～3 */
	font[L'0'] = { 8,7 };
	font[L'1'] = { 9,7 };
	font[L'2'] = { 10,7 };
	font[L'3'] = { 11,7 };
	/*! 4～9 */
	font[L'4'] = { 0,8 };
	font[L'5'] = { 1,8 };
	font[L'6'] = { 2,8 };
	font[L'7'] = { 3,8 };
	font[L'8'] = { 4,8 };
	font[L'9'] = { 5,8 };
	/*! a～f */
	font[L'a'] = { 6,8 };
	font[L'b'] = { 7,8 };
	font[L'c'] = { 8,8 };
	font[L'd'] = { 9,8 };
	font[L'e'] = { 10,8 };
	font[L'f'] = { 11,8 };
	/*! g～r */
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
	/*! s～z */
	font[L's'] = { 0,10 };
	font[L't'] = { 1,10 };
	font[L'u'] = { 2,10 };
	font[L'v'] = { 3,10 };
	font[L'w'] = { 4,10 };
	font[L'x'] = { 5,10 };
	font[L'y'] = { 6,10 };
	font[L'z'] = { 7,10 };
	/*! ・,、。空白 */
	font[L'.'] = { 8,10 };
	font[L','] = { 9,10 };
	font[L'、'] = { 10,10 };
	font[L'。'] = { 11,10 };
	/*! ぁ～ょ */
	font[L'ぁ'] = { 0,11 };
	font[L'ぃ'] = { 1,11 };
	font[L'ぅ'] = { 2,11 };
	font[L'ぇ'] = { 3,11 };
	font[L'ぉ'] = { 4,11 };
	font[L'っ'] = { 5,11 };
	font[L'ゃ'] = { 6,11 };
	font[L'ゅ'] = { 7,11 };
	font[L'ょ'] = { 8,11 };
	/*! ゛゜ー */
	font[L'゛'] = { 9,11 };
	font[L'゜'] = { 10,11 };
	font[L'ー'] = { 11,11 };
	/*! 「」・！？（）※：＜＞… */
	font[L'「'] = { 0,12 };
	font[L'」'] = { 1,12 };
	font[L'・'] = { 2,12 };
	font[L'！'] = { 3,12 };
	font[L'？'] = { 4,12 };
	font[L'（'] = { 5,12 };
	font[L'）'] = { 6,12 };
	font[L'※'] = { 7,12 };
	font[L'：'] = { 8,12 };
	font[L'＜'] = { 9,12 };
	font[L'＞'] = { 10,12 };
	font[L'…'] = { 11,12 };
	/*! 空白 */
	font[L' '] = { 13,13 };
	font[L'　'] = { 13,13 };

}

/*
	@brief	数字のデータの作成
	@detail	連想配列に数字の登録
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
