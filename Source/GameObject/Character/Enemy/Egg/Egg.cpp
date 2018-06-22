/*
	@file	Egg.h
	@brief	卵
	@author	番場 宥輝
*/
#include "Egg.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Egg::HATCH_COUNT = FPS * 6;

/*
	@brief	コンストラクタ
*/
Egg::Egg()
{
}

/*
	@brief	デストラクタ
*/
Egg::~Egg()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Egg::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_isDelete = false;
	m_Type = Type::EGG;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.0f);
	m_pSprite->SetSplit(4, 2);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Count = 0;

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	解放
*/
void Egg::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Egg::Update()
{
	m_Count++;

	/*! 孵化に必要な時間が経ったら */
	if (HATCH_COUNT < m_Count) {
		Hatch();
	}
}

/*
	@brief	描画
*/
void Egg::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Egg::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		m_isDelete = true;
		/*!< データの削除 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	}
}

/*
	@brief	衝突
*/
void Egg::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	孵化
*/
void Egg::Hatch()
{
	Lizard* hatch = new Lizard;
	hatch->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(hatch);
	m_isDelete = true;
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}

