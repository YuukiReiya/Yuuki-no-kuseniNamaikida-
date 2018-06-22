/*
	@file	Chrysalis.h
	@brief	蛹
	@author	番場 宥輝
*/
#include "Chrysalis.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Chrysalis::ACTION_COUNT = 60;
const unsigned int Chrysalis::GROW_COUNT = FPS * 8;

/*
	@brief	コンストラクタ
*/
Chrysalis::Chrysalis()
{
}

/*
	@brief	デストラクタ
*/
Chrysalis::~Chrysalis()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Chrysalis::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;

	m_isDelete = false;
	m_Type = Type::CHRYSALIS;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.1f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Count = 0;

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	初期化
	@detail	発生時のHPを指定できる
*/
void Chrysalis::Init(INT2 pos, INT2 component, int hp)
{
	Init(pos, component);
	m_Hp = hp;
}

/*
	@brief	解放
*/
void Chrysalis::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Chrysalis::Update()
{
	m_Count++;

	/*! 成長に必要な時間が経ったら */
	if (GROW_COUNT < m_Count) {
		Grow();
	}
}

/*
	@brief	描画
*/
void Chrysalis::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Chrysalis::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		OutComponent();
		/*!< データの削除 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
		m_isDelete = true;
	}
}

/*
	@brief	衝突
*/
void Chrysalis::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	トカゲとの衝突
*/
void Chrysalis::Collision(Lizard & lizard)
{
	/*! 衝突先に向きを変える */
	INT2 dir = lizard.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	成長
*/
void Chrysalis::Grow()
{
	Adult* child = new Adult;
	child->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(child);
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	保有養分・魔分周囲への放出
*/
void Chrysalis::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 },{ 1, 0 },
		{ -1, 1 },{ 0, 1 },{ 1, 1 },
	};

	/*! 放出できるか判定 */
	int output = false;

	/*! 周囲に放出可能なブロックがあるか判定 */
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		/*! 外部参照 */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		/*! ブロック有無 */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		output = true;
		break;
	}

	/*! 放出出来なければ処理しない */
	if (!output) { return; }

	for (int i = 0;; i++) {
		/*! 放出する養分・魔分が無い */
		if (m_Component == INT2(0, 0)) { break; }/*!< 処理を抜ける */
		/*! 放出位置 */
		INT2 pos = m_IndexPos + around[i % (sizeof(around) / sizeof(INT2))];
		/*! 外部参照 */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		/*! ブロック有無 */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! 養分・魔分のどちらが多いか比較 */
		INT2 tmp = TerrainManager::GetInstance().GetComponent(pos);
		int component = tmp.x >= tmp.y ? tmp.x : tmp.y;
		if (component >= MAX_COMPONENT) { continue; }

		/*! 放出先のブロックの養分・魔分が上限の場合加算しない */
		if (component >= MAX_COMPONENT) {

			/*! 養分が多い */
			if (tmp.x >= tmp.y) {
				m_Component.x -= 1;
			}
			/*! 魔分が多い */
			else {
				m_Component.y -= 1;
			}
			continue;
		}
		component += 1;

		/*! 養分が多い */
		if (tmp.x >= tmp.y) {
			m_Component.x -= 1;
			TerrainManager::GetInstance().
				SetComponent(pos, { component,tmp.y });
		}
		/*! 魔分が多い */
		else {
			m_Component.y -= 1;
			TerrainManager::GetInstance().
				SetComponent(pos, { tmp.x,component });
		}
	}
}
