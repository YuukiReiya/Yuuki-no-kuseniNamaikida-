#include "TycoonManager.h"

/*
	@brief	コンストラクタ
*/
TycoonManager::TycoonManager()
{
	
}

/*
	@brief	デストラクタ
*/
TycoonManager::~TycoonManager()
{
	Destroy();
}

/*
	@brief	初期化
*/
void TycoonManager::Init()
{
	m_pTycoon	=	new Tycoon;
	m_pHeroPos	=	NULL;
	m_pTexture	=	new Texture;

	m_pTycoon->Init();
	m_pTycoon->m_IndexPos = m_pTycoon->m_NextIndexPos = { -1,-1 };
	m_pTycoon->m_MovePow = 0.0f;
	m_pTycoon->isCaught = false;

	if (!m_pTexture->Load(L"../Resource/Texture/魔王.png")) { 
		MSG("テクスチャの読み込みに失敗"); 
	}
	m_isGameOver = false;
	m_Debug.Init();
}

/*
	@brief	描画
*/
void TycoonManager::Render()
{
	m_pTycoon->Render(m_pTexture->GetTexture());
}

/*
	@brief	解放
*/
void TycoonManager::Destroy()
{
	SAFE_DELETE(m_pHeroPos);
	SAFE_DELETE(m_pTycoon);
	SAFE_DELETE(m_pTexture);
}

/*
	@brief	勇者の位置更新
*/
void TycoonManager::SetHeroPos(INT2 pos)
{
	m_pTycoon->m_NextIndexPos = pos;
}

/*
	@brief	更新
*/
void TycoonManager::Update()
{
	INT2 way[3] = {
		{-1,0 },
		{ 1,0 },
		{ 0,1 },
	};
	INT2 gameOverPos = { GAME_OVER_X,GAME_OVER_Y };
	for (int i = 0; i < sizeof(way) / sizeof(INT2); i++) {
		way[i] = gameOverPos + way[i];
		if (m_pTycoon->m_IndexPos == way[i]) {
			m_isGameOver = true;
			break;
		}
	}
	//if (m_pTycoon->m_IndexPos == gameOverPos)m_isGameOver = true;

	m_pTycoon->Update();
}


/*
	@brief	魔王の配置場所を決める
*/
void TycoonManager::SetTycoonPos(INT2 Pos)
{
	m_pTycoon->m_IndexPos = Pos;
	m_pTycoon->m_Pos = Game::ConvertIndexToPosition(m_pTycoon->m_IndexPos);
}
