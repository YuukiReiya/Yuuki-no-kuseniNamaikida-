/*
	@file	SceneTitle.cpp	
	@brief	タイトルシーン
	@author	番場 宥輝
*/
#include "SceneTitle.h"
#include "../../GameObject/Manager/UiManager/UiManager.h"
#include "../../GameObject/Manager/InputManager/InputManager.h"
#include "../../GameObject/Manager/SoundManager/SoundManager.h"

/*
	@brief	名前空間
*/
using namespace std;

/*
	@var	グローバル変数
*/
SceneTitle::MODE SceneTitle::mode = MODE::START;
SceneTitle::LEVEL SceneTitle::level;

/*
	@def	定数宣言
*/

/**************/
/*    START   */
/**************/
wstring SceneTitle::START_TEXT[] = {
	L"せ゛んこくのはかいしんのみなさま ろうほうて゛す！！",
	L"あの、た゛いにんきタイトル    「ゆうしゃのくせになまいきた゛」　か゛なんと！！",
	L"か゛くせいさくひんとして     ふっかついたしました。",
	L"いやぁ、か゛くせいによるリメイク なんてうれしいて゛すね。",
	L"と゛うそ゛こころゆくまて゛、   おたのしみくた゛さい。"

};
unsigned int SceneTitle::START_ICON[] = {
	1,
	2,
	1,
	4,
	1,
};
/**************/
/*   SELECT   */
/**************/
wstring SceneTitle::SELECT_TEXT[] = {
	L"さぁ、さっそく          ステーシ゛をえらんて゛くた゛さい。はかいしんさま。",
	L"わたくしは、           ひとあしさきにおもむいております。",
};
unsigned int SceneTitle::SELECT_ICON[] = {
	1,
	1,
};
/**************/
/*    CLEAR   */
/**************/
wstring SceneTitle::CLEAR_TEXT[] = {
	L"かってかふ゛とのおをしめよ。   ゆた゛んしないて゛くた゛さい、　　はかいしんさま。",
	L"つき゛も、            きをひきしめていきましょう。",
};
unsigned int SceneTitle::CLEAR_ICON[] = {
	1,
	1,
};
/**************/
/*    LOSE    */
/**************/
wstring SceneTitle::LOSE_TEXT[] = {
	L"あらら、             しっは゜いしてしまいましたか",
	L"きをとりなおして、        つき゛いきましょう。",
};
unsigned int SceneTitle::LOSE_ICON[] = {
	4,
	3,
};
/**************/
/*   PERFECT  */
/**************/
wstring SceneTitle::PERFECT_TEXT[] = {
	L"さぁ、さっそくステージをえらんでください。      はかいしんさま。",
	L"せかいにへいわは、おとずれない",
};
unsigned int SceneTitle::PERFECT_ICON[] = {
	1,
	2,
};
/**************/
/*    END     */
/**************/
wstring SceneTitle::END_TEXT[] = {
	L"おや、ゲームをやめられるのですか。   ざんねんです。",
	L"また、あそんでくださいね",
};
unsigned int SceneTitle::END_ICON[] = {
	1,
	2,
};


/*
	@brief	コンストラクタ
*/
SceneTitle::SceneTitle()
{
}

/*
	@brief	デストラクタ
*/
SceneTitle::~SceneTitle()
{
	Destroy();
}

/*
	@brief	初期化
*/
void SceneTitle::Init()
{
	m_pBackGroundSprite = new Sprite;
	m_pBackGroundTexture = new Texture;

	m_pBackGroundSprite->SetSpriteSize(478, 270);
	m_pBackGroundSprite->SetSizeScaling(4);
	if (!m_pBackGroundTexture->Load(L"../Resource/Texture/back.png")) { MSG("テクスチャの読み込みに失敗"); }
	
	UiManager::GetInstance().Init();
	SoundManager::GetInstance().Vol(BGM::TITLE, 60);
	SoundManager::GetInstance().Play(BGM::TITLE,true);
	SetUpText();
	m_TextItr = 0;
	m_IconItr = 0;
	level = LEVEL::EASY;
}

/*
	@brief	更新
*/
SceneBase * SceneTitle::Update(SceneRoot * root)
{
	SceneBase* next = this;
	UiManager::GetInstance().Counter();
	/*! フロー */
	switch (mode)
	{
	/*! ゲーム起動時 */
	case SceneTitle::START:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(START_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! ステージセレクト */
	case SceneTitle::SELECT:
		if (SelectUpdate()) {
			UiManager::GetInstance().CounterReset();
			SoundManager::GetInstance().Stop(BGM::TITLE);
			return new SceneMain;
		}
		break;
	/*! ステージクリア */
	case SceneTitle::CLEAR:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(CLEAR_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! ステージ失敗 */
	case SceneTitle::LOSE:
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			if (sizeof(LOSE_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
				m_TextItr = 0;
				m_IconItr = 0;
				mode = SELECT;
			}
			else {
				m_TextItr++;
				m_IconItr++;
			}
		}
		break;
	/*! ゲームクリア */
	case SceneTitle::PERFECT:
		break;
	/*! ゲーム終了 */
	case SceneTitle::END:
		break;
	default:
		break;
	}
	return next;
}

/*
	@brief	レンダリング
*/
void SceneTitle::Render()
{
		/*! フロー */
		switch (mode)
		{
		/*! ゲーム起動時 */
		case SceneTitle::START:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(START_TEXT[m_TextItr], (Window::ICON)START_ICON[m_IconItr]);
			break;
		/*! ステージセレクト */
		case SceneTitle::SELECT:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			SelectRender();
			break;
		/*! ステージクリア */
		case SceneTitle::CLEAR:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(CLEAR_TEXT[m_TextItr], (Window::ICON)CLEAR_ICON[m_IconItr]);
			break;
		/*! ステージ失敗 */
		case SceneTitle::LOSE:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(LOSE_TEXT[m_TextItr], (Window::ICON)LOSE_ICON[m_IconItr]);
			break;
		/*! ゲームクリア */
		case SceneTitle::PERFECT:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(PERFECT_TEXT[m_TextItr], (Window::ICON)PERFECT_ICON[m_IconItr]);
			break;
		/*! ゲーム終了 */
		case SceneTitle::END:
			m_pBackGroundSprite->RenderLeftTop(m_pBackGroundTexture->GetTexture());
			UiManager::GetInstance().DrawMessage(END_TEXT[m_TextItr], (Window::ICON)END_ICON[m_IconItr]);
			break;
		default:
			break;
		}

}

/*
	@brief	解放
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_pBackGroundSprite);
	SAFE_DELETE(m_pBackGroundTexture);
	UiManager::GetInstance().Destroy();
}

/*
	@brief	表示テキストのセットアップ
*/
void SceneTitle::SetUpText()
{
	
}

/*
	@brief	ステージセレクトの更新処理
*/
bool SceneTitle::SelectUpdate()
{
	/*! テキストの表示 */
	if (sizeof(SELECT_TEXT) / sizeof(wstring) > m_TextItr) {
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			m_TextItr++;
			m_IconItr++;
		}
		return false;
	}
	/*! レベル調整 */
	else {
		int l = (int)level;
		if (input.GetKeyDown(input.UP)) {
			SoundManager::GetInstance().Play(SE::MENU, false);
			l--;
			if (l < (int)LEVEL::EASY) {
				l = (int)LEVEL::HARD;
			}
		}
		else if (input.GetKeyDown(input.DOWN)) {
			SoundManager::GetInstance().Play(SE::MENU, false);
			l++;
			if ((int)LEVEL::HARD < l) {
				l = (int)LEVEL::EASY;
			}
		}
		else if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			return true;
		}
		level = (LEVEL)l;
	}
	return false;
}

/*
	@brief	セレクトの描画
*/
void SceneTitle::SelectRender()
{
	if (sizeof(SELECT_TEXT) / sizeof(wstring) > m_TextItr) {
		UiManager::GetInstance().DrawMessage(SELECT_TEXT[m_TextItr], (Window::ICON)SELECT_ICON[m_IconItr]);
	}
	else {
		DrawLevel();
	}
}

/*
	@brief	難易度描画
*/
void SceneTitle::DrawLevel()
{
	D3DXVECTOR2 pos = {
		950,
		250
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		pos.x,
		pos.y + 250,
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		pos.x,
		pos.y + 250,
	};
	UiManager::GetInstance().DrawLevel(pos);

	pos = {
		700,
		(float)(250 + (int)level * 250)
	};
	UiManager::GetInstance().DrawCursolRight(pos);
	pos = {
		pos.x + 130,
		250
	};
	UiManager::GetInstance().DrawString(L"かんたん", 8.5f, pos);
	pos = {
		pos.x + 40,
		500
	};
	UiManager::GetInstance().DrawString(L"ふつう", 8.5f, pos);
	pos = {
		pos.x - 70,
		750
	};
	UiManager::GetInstance().DrawString(L"むす゛かしい", 8.0f, pos);
}
