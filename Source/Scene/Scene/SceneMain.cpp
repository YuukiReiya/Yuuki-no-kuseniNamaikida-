/*
	@file	SceneMain.cpp
	@brief	メインシーン
	@date	2017/12/04
	@author	番場 宥輝
*/
#include "SceneMain.h"
#include "../../GameObject/Manager/InputManager/InputManager.h"
#include "../../GameObject/Manager/SoundManager/SoundManager.h"

/*
	@brief	名前空間
*/
using namespace std;

/*
	@def	定数宣言
*/

/**************/
/*    START   */
/**************/
wstring SceneMain::START_TEXT[] = {
	L"おまちしておりました、      はかいしんさま。",
	L"と゛うやら、ちかくにゆうしゃか゛ きているようて゛す。",
	L"モンスターをうみた゛し、せまりくるゆうしゃをけ゛きたいするのて゛す。",
};
unsigned int SceneMain::START_ICON[] = {
	1,
	3,
	1,
};
/**************/
/*  SETTING   */
/**************/
wstring SceneMain::SETTING_TEXT[] = {
	L"と゛うやら、           ゆうしゃか゛きてしまったようて゛す。",
	L"わたしか゛、てきにつかまってしまうとまけてしまいます。よくかんか゛えてきめてくた゛さいね。",
	L"※つちか゛あるところには、　　   わたしはおけません。",
	L"それて゛は            はいち              よろしくおねか゛いします。",
};
unsigned int SceneMain::SETTING_ICON[] = {
	2,
	3,
	3,
	1,
};
/**************/
/*    WIN     */
/**************/
wstring SceneMain::WIN_TEXT[] = {
	L"おお、はかいしんさま       おみこ゛とて゛す。",
	L"つき゛も、このちょうして゛    たのみますそ゛",
	L"タイトルにもと゛ります。 ",
};
unsigned int SceneMain::WIN_ICON[] = {
	1,
	3,
	0,
};
/**************/
/*    LOSE    */
/**************/
wstring SceneMain::LOSE_TEXT[] = {
	L"ああ、はかいしんさま。      つかまってしまいました。",
	L"つき゛はもっといいは゛しょに   はいちしてくた゛さい！ ",
	L"タイトルにもと゛ります。 ",
};
unsigned int SceneMain::LOSE_ICON[] = {
	3,
	2,
	0,
};

/*
	@brief	コンストラクタ
*/
SceneMain::SceneMain()
{
	/*! インスタンスの生成 */
	m_pPlayer = new Player;
}

/*
	@brief	デストラクタ
*/
SceneMain::~SceneMain()
{
	Destroy();
}

/*
	@brief	初期化
*/
void SceneMain::Init()
{
	TerrainManager::GetInstance().Init();
	for (int i = 0; i < 5; i++) {
		TerrainManager::GetInstance().Break({ 28,i });
	}
	TerrainManager::GetInstance().Break({ 29,4 });



	/*! 各管理クラスの初期化処理呼び出し */
	LocationManager::GetInstance().Init();
	EnemyManager::GetInstance().Init();		/*!< モンスター */
	HeroManager::GetInstance().Init();		/*!< 勇者 */
	TycoonManager::GetInstance().Init();	/*!< 魔王 */
	CollisionManager::GetInstance().Init();	/*!< 衝突 */
	UiManager::GetInstance().Init();		/*!< UI */

	/*! プレイヤーの初期化 */
	m_pPlayer->Init();	

	/*! メンバ変数の初期化 */
	mode = MODE::START;
	m_TextItr = m_IconItr = 0;
	m_Fps = 0;
	m_isStop = false;
	m_HeroComeFps = 0;
	if (!m_Debug.Init()) { MSG("debug失敗"); }

	/*! 難易度別 */
	if (SceneTitle::GetLevel() == SceneTitle::LEVEL::EASY) {
		m_pPlayer->SetDigPower(100);
		m_HeroComeFps = FPS * 30;	/*!< 30秒 */
		param.hp = 84;
		param.atk = 15;
		param.movePow = 1.0f;
	}
	else if (SceneTitle::GetLevel() == SceneTitle::LEVEL::NORMAL) {
		m_pPlayer->SetDigPower(300);
		m_HeroComeFps = FPS * 90;	/*!< 90秒 */
		param.hp = 150;
		param.atk = 35;
		param.movePow = 1.2f;
	}
	else if (SceneTitle::GetLevel() == SceneTitle::LEVEL::HARD) {
		m_pPlayer->SetDigPower(500);
		m_HeroComeFps = FPS * 180;	/*!< 180秒 */
		param.hp = 999;
		param.atk = 90;
		param.movePow = 1.5f;
	}

	/*! BGM再生 */
	SoundManager::GetInstance().Play(BGM::GAME, true);
}

/*
	@brief 更新
*/
SceneBase* SceneMain::Update(SceneRoot* root)
{
	SceneBase* next = this;

	/*! ゲームクリア */
	if (HeroManager::GetInstance().GetClearFlag()) {
		mode = MODE::WIN;
	}

	/*! ゲームオーバー */
	if (TycoonManager::GetInstance().GetGameOverFlags()) {
		mode = MODE::LOSE;
	}

	
	TerrainManager::GetInstance().Update();		/*!< 地形更新 */
	TycoonManager::GetInstance().Update();		/*!< 魔王 */

	/*! フロー */
	switch (mode)
	{
	/*! シーンの開始時 */
	case SceneMain::START:
		StartUpdate();
		break; 
	/*! ダンジョン制作 */
	case SceneMain::FREE:
		FreeUpdate();
		break;
	/*! 勇者呼び出し中 */
	case SceneMain::CALL:
		CallUpdate();
		break;
	/*! 魔王の位置設定 */
	case SceneMain::SETTING:
		SettingUpdate();
		break;
	/*! 勇者がダンジョン内にいる */
	case SceneMain::BATTLE:
		BattleUpdate();
		break;
	/*! クリア時 */
	case SceneMain::WIN:
		if (ClearUpdate()) {
			SceneTitle::SetMode(SceneTitle::MODE::CLEAR);
			SoundManager::GetInstance().Stop(BGM::GAME);
			return new SceneTitle;
		}
		break;
	/*! 敗北時 */
	case SceneMain::LOSE:
		if (LoseUpdate()) {
			SceneTitle::SetMode(SceneTitle::MODE::LOSE);
			SoundManager::GetInstance().Stop(BGM::GAME);
			return new SceneTitle;
		}
		break;
	default:
		break;
	}

	
	/*! プレイヤーのインデックスの更新 */
	m_pPlayer->Update();
	
	EnemyManager::GetInstance().Update();		/*!< モンスター更新 */
	CollisionManager::GetInstance().Update();	/*!< 衝突処理 */

	/*! リストのクリア */
	CollisionManager::GetInstance().Clear();	/*!< 衝突情報をクリア */
	EnemyManager::GetInstance().HitListClear();	/*!< 衝突判定リストクリア */

	
	return next;
}

/*
	@brief	描画
*/

void SceneMain::Render()
{
	TerrainManager::GetInstance().Render();
	EnemyManager::GetInstance().Render();
	HeroManager::GetInstance().Render();
	TycoonManager::GetInstance().Render();
	m_pPlayer->Render();
	m_pPlayer->DrawDigPower();
	
	UiManager::GetInstance().DrawHeroData();

	/*! フロー */
	switch (mode)
	{
	/*! シーンの開始時 */
	case SceneMain::START:
		UiManager::GetInstance().DrawMessage(START_TEXT[m_TextItr], (Window::ICON)START_ICON[m_IconItr]);
		break;
	/*! ダンジョン制作 */
	case SceneMain::FREE:
		break;
	/*! 勇者呼び出し中 */
	case SceneMain::CALL:
		break;
	/*! 魔王の位置設定 */
	case SceneMain::SETTING:
		/*! テキスト数とイテレータを比較 */
		if (sizeof(SETTING_TEXT) / sizeof(wstring) > m_TextItr) {
			UiManager::GetInstance().DrawMessage(SETTING_TEXT[m_TextItr], (Window::ICON)SETTING_ICON[m_IconItr]);
		}
		break;
	/*! 勇者がダンジョン内にいる */
	case SceneMain::BATTLE:
		break;
	/*! クリア時 */
	case SceneMain::WIN:
		SceneTitle::SetMode(SceneTitle::MODE::CLEAR);
		UiManager::GetInstance().DrawMessage(WIN_TEXT[m_TextItr], (Window::ICON)WIN_ICON[m_IconItr]);
		break;
	/*! 敗北時 */
	case SceneMain::LOSE:
		UiManager::GetInstance().DrawMessage(LOSE_TEXT[m_TextItr], (Window::ICON)LOSE_ICON[m_IconItr]);
		break;
	default:
		break;
	}
}

/*
	@brief	解放
*/
void SceneMain::Destroy()
{
	TerrainManager::GetInstance().Destroy();
	LocationManager::GetInstance().Destroy();
	EnemyManager::GetInstance().Destroy();
	HeroManager::GetInstance().Destroy();
	TycoonManager::GetInstance().Destroy();
	UiManager::GetInstance().Destroy();
	SAFE_DELETE(m_pPlayer);
}

/*
	@brief	シーン開始時の更新処理
*/
void SceneMain::StartUpdate()
{	
	/*! テキスト送り */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(START_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			mode = MODE::FREE;/*!< フリーモードへ */
			m_TextItr = 0;
			m_IconItr = 0;
			SoundManager::GetInstance().Play(BGM::GAME, false);
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
}

/*
	@brief	クリア時の更新処理
*/
bool SceneMain::ClearUpdate()
{
	/*! テキスト送り */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(WIN_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			return true;
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
	return false;
}

/*
	@brief	敗北時の更新処理
	@return	テキストの終了
*/
bool SceneMain::LoseUpdate()
{
	/*! テキスト送り */
	UiManager::GetInstance().Counter();
	if (input.GetKeyDown(input.DECIDE)) {
		SoundManager::GetInstance().Play(SE::DECIDE, false);
		if (sizeof(LOSE_TEXT) / sizeof(wstring) - 1 <= m_TextItr) {
			UiManager::GetInstance().CounterReset();
			return true;
		}
		else {
			m_TextItr++;
			m_IconItr++;
		}
	}
	return false;
}

/*
	@brief	ダンジョン制作中の更新処理
*/
void SceneMain::FreeUpdate()
{
	/*! 勇者来訪までのタイム */
	if (m_Fps == m_HeroComeFps) {
		m_Fps = 0;
		mode = MODE::SETTING;
	}

	m_Fps++;
	/*! プレイヤーの処理 */
	m_pPlayer->Move();		/*!< カーソル移動 */
	m_pPlayer->Action();	/*!< ブロックの破壊 */
}

/*
	@brief	勇者呼び出し中の更新
*/
void SceneMain::CallUpdate()
{
}

/*
	@brief	魔王の位置を設定する時の更新
*/
void SceneMain::SettingUpdate()
{
	/*! テキスト処理 */
	if (sizeof(SETTING_TEXT) / sizeof(wstring) - 1 >= m_TextItr) {
		UiManager::GetInstance().Counter();
		if (input.GetKeyDown(input.DECIDE)) {
			SoundManager::GetInstance().Play(SE::DECIDE, false);
			m_TextItr++;
			m_IconItr++;
		}
	}
	/*! 魔王の位置決定 */
	else {
		m_pPlayer->Move();	/*!< カーソル移動 */
		/*! 魔王の位置セット */
		if (m_pPlayer->SetTycoon()) {
			Soldier* sol = new Soldier;
			sol->SetParam(param.hp, param.atk, param.movePow);
			HeroManager::GetInstance().AddHero(sol);
			m_TextItr = 0;
			m_IconItr = 0;
			UiManager::GetInstance().CounterReset();
			mode = MODE::BATTLE;
		}

	}
}

/*
	@brief	勇者がダンジョン内に居る時の更新
*/
void SceneMain::BattleUpdate()
{
	/*! マネージャー */
	HeroManager::GetInstance().Update();	/*!< 勇者 */

	/*! プレイヤー */
	m_pPlayer->Move();	/*!< カーソル移動 */
	m_pPlayer->Action();/*!< ブロックの破壊 */
}