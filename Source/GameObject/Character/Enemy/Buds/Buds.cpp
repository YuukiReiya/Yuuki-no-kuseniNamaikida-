/*
	@file	Buds.cpp
	@brief	蕾
	@author	番場 宥輝
*/
#include "Buds.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Buds::ACTION_COUNT = 60;
const unsigned int Buds::ABSORPTION_OVER_LIMIT = 11;
const unsigned int Buds::ABSORPTION_UNDER_LIMIT = 0;
const unsigned int Buds::GROW_COMPONENT = 8;

/*
	@brief	コンストラクタ
*/
Buds::Buds()
{
}

/*
	@brief	デストラクタ
*/
Buds::~Buds()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Buds::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;

	m_isDelete = false;
	m_Type = Type::BUDS;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = D3DXVECTOR2(0, 0);
	m_MoveWeight = 0;
	
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSizeScaling(0.8f);
	m_Count = 0;
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Debug.Init();

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	初期化
	@detail	発生時のHPを指定できる
*/
void Buds::Init(INT2 pos, INT2 component, int hp)
{
	Init(pos, component);
	m_Hp = hp;
}

/*
	@brief	解放
*/
void Buds::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Buds::Update()
{
	m_Count++;/*!< カウンタの加算 */

	/*! カウンタが規定値未満なら行動しない */
	if (m_Count < ACTION_COUNT) { return; }
	m_Count = 0;/*!< カウンタのリセット */

	/*! 養分の吸収 */
	if (!Absorption()) {
		Damage(1);/*!< ダメージ */
	}
	
	/*! 成長に必要な養分を保持していたら */
	if (m_Component.x >= GROW_COMPONENT) {
		Grow();
	}
}

/*
	@brief	描画
*/
void Buds::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Buds::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		OutComponent();
		m_isDelete = true;
		/*!< データの削除 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	}
}

/*
	@brief	衝突
*/
void Buds::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	成虫との衝突
*/
void Buds::Collision(Adult & adult)
{
	/*! 衝突先に向きを変える */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	養分吸収
*/
bool Buds::Absorption()
{
	/*! 判定する周囲 */
	INT2 around[] = {
		{ -2,-2 },{ -1,-2 },{ 0,-2 },{ 1,-2 },{ 2,-2 },
		{ -2,-1 },{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 2,-1 },
		{ -2, 0 },{ -1, 0 }		,	 { 1, 0 },{ 2, 0 },
		{ -2, 1 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 2, 1 },
		{ -2, 2 },{ -1, 2 },{ 0, 2 },{ 1, 2 },{ 2, 2 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;

		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	 { continue; }/*!< x座標の範囲外 */
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	 { continue; }/*!< y座標の範囲外 */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }/*!< ブロックが破壊 */

		auto component = TerrainManager::GetInstance().GetComponent(pos);
		/*! 養分の吸収可能上限/下限限界 */
		if (component.x <= ABSORPTION_UNDER_LIMIT ||
			component.x >= ABSORPTION_OVER_LIMIT) {
			continue;
		}

		/*! 吸収 */
		component.x -= 1;
		m_Component.x += 1;
		m_Hp += 1;
		if (component.x < 0) { component.x = 0; }
		TerrainManager::GetInstance().SetComponent(pos, component);

		return true;
	}
	return false;
}

/*
	@brief	成長
*/
void Buds::Grow()
{
	/*! 花への処理 */
	Flower* grow = new Flower;
	grow->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(grow);
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	保有養分・魔分周囲への放出
*/
void Buds::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{  0,-1 },{ 1,-1 },
		{ -1, 0 },			{ 1, 0 },
		{ -1, 1 },{  0, 1 },{ 1, 1 },
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
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)	{ continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y)	{ continue; }
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
