/*
	@file	Tycoon.cpp
	@brief	魔王
	@author	番場 宥輝
*/
#include "Tycoon.h"

/*
	@def	定数宣言
*/
const unsigned int Tycoon::ANIMATION_UPDATE_COUNT = 10;

/*
	@brief	コンストラクタ
*/
Tycoon::Tycoon()
{
}

/*
	@brief	デストラクタ
*/
Tycoon::~Tycoon()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Tycoon::Init()
{
	m_pSprite = new Sprite;
	m_pDebug = new Debug;

	m_IndexPos = INT2{ 29,4 };
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
	m_NextIndexPos = INT2{ -1,-1 };
	m_AnimCount = 0;
	m_AnimIndexU = m_AnimIndexV = 0;
	m_pSprite->SetSplit(3, 5);
	m_pSprite->SetSpriteSize(32, 32);
	m_pSprite->SetSizeScaling(1.8f);
	m_pDebug->Init();
}

/*
	@brief	更新
*/
void Tycoon::Update()
{
	AnimUpdate();
	if (Game::TheSamePositionToGrid(m_Pos))
		IndexUpdate();

	/*
	
		・モンスター全てに当たり判定

	
	*/


	Move();
}

/*
	@brief	インデックス更新
*/
void Tycoon::IndexUpdate()
{
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);
}

/*
	@brief	アニメーション更新
*/
void Tycoon::AnimUpdate()
{
	
	m_AnimCount++;
	/*! 捕縛 */
	if (isCaught) {
		/*! 向き変更 */
			INT2 tmp = m_NextIndexPos - m_IndexPos;			/*!< 移動先と居場所から向きを検出、一時変数に */
			D3DXVECTOR2 dir = Game::ConvertDirection(tmp);	/*!< 変換した一時変数をvector2に変換 */
			if (dir == Game::UP) { m_AnimIndexV = 3; }
			else if (dir == Game::DOWN) { m_AnimIndexV = 1; }
			else if (dir == Game::LEFT) { m_AnimIndexV = 2; }
			else if (dir == Game::RIGHT) { m_AnimIndexV = 4; }

		if (m_AnimCount < ANIMATION_UPDATE_COUNT) { return; }
		m_AnimIndexU++;
		m_AnimCount = 0;
		if (m_AnimIndexU > 2)m_AnimIndexU = 0;
	}
	/*! 健在 */
	else {
		m_AnimIndexV = 0;
		if (m_AnimCount < ANIMATION_UPDATE_COUNT) { return; }
		m_AnimIndexU++;
		m_AnimCount = 0;
		if (m_AnimIndexU > 1)m_AnimIndexU = 0;
	}
	/*
	if (m_Direction == Game::UP) {
		m_AnimIndexV = 3;
	}
	else if (m_Direction == Game::DOWN) {
		m_AnimIndexV = 0;
	}
	else if (m_Direction == Game::LEFT) {
		m_AnimIndexV = 1;
	}
	else if (m_Direction == Game::RIGHT) {
		m_AnimIndexV = 2;
	}
	*/
}

/*
	@brief	レンダリング
*/
void Tycoon::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture,m_AnimIndexU,m_AnimIndexV);
//	m_pDebug->DrawSquare(ConvertView(m_Pos), (float)GRID_SIZE, (float)GRID_SIZE, Debug::GREEN);
}

/*
	@brief	解放
*/
void Tycoon::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pDebug);
}

/*
	@brief	移動
*/
void Tycoon::Move()
{
	/*
		移動先は可変長配列にいれ保存する
		（ASTARで実現）
	*/
	D3DXVECTOR2 dir = {
	(float)(m_NextIndexPos.x - m_IndexPos.x),
	(float)(m_NextIndexPos.y - m_IndexPos.y),
	};

	m_Pos += dir*m_MovePow;
	//座標調整
	{
		bool isOver = false;
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);

		if (dir == Game::UP&&temp.y >= m_Pos.y) { isOver = true; }			//上
		else if (dir == Game::DOWN&&temp.y <= m_Pos.y) { isOver = true; }	//下
		else if (dir == Game::LEFT&&temp.x >= m_Pos.x) { isOver = true; }	//左
		else if (dir == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	//右
		//座標超過判定
		if (isOver)
			m_Pos = temp;
	}
}
