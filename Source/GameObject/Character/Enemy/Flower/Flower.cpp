/*
	@file	Flower.cpp
	@brief	花
	@author	番場 宥輝
*/
#include "Flower.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Flower::ACTION_COUNT = 60;
const unsigned int Flower::ABSORPTION_POWER = 3;
const unsigned int Flower::ABSORPTION_OVER_LIMIT = 11;
const unsigned int Flower::ABSORPTION_UNDER_LIMIT = 0;
const unsigned int Flower::BREEDING_COMPONENT = 11;
const unsigned int Flower::BREEDING_COUNT = 8;
const unsigned int Flower::MAX_HP = 21;

/*
	@brief　コンストラクタ
*/
Flower::Flower()
{
}

/*
	@brief	デストラクタ
*/
Flower::~Flower()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Flower::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_Type = Type::FLOWER;
	m_isDelete = false;
	m_Hp = 18;
	m_Atk = 9;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_Count = 0;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSizeScaling(0.8f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	解放
*/
void Flower::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Flower::Update()
{
	m_Count++;/*!< カウンタの加算 */
				
	/*! カウンタが規定値未満なら行動しない */
	if (m_Count < ACTION_COUNT) { return; }
	m_Count = 0;/*!< カウンタのリセット */

	/*! 吸収に失敗 */
	if (!Absorption()) {
		m_Hp -= 2;
	}

	/*! 減衰 */
	if (BREEDING_COUNT <= m_AbsorptionCount) {
		m_Hp -= 2;
	}

	/*! 繁殖に必要な養分と吸収回数を満たしているか判定 */
	if (BREEDING_COMPONENT <= m_Component.x&&
		BREEDING_COUNT <= m_AbsorptionCount) {
		Breeding();
	}
}

/*
	@brief	描画
*/
void Flower::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Flower::Damage(int damage)
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
void Flower::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	成虫との当たり判定
*/
void Flower::Collision(Adult & adult)
{
	/*! 衝突先に向きを変える */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	養分吸収
*/
bool Flower::Absorption()
{
	/*! 判定する周囲 */
	INT2 around[] = {
		{ -3,-3 },{ -2,-3 },{ -1,-3 },{ 0,-3 },{ 1,-3 },{ 2,-3 },{ 3,-3 },
		{ -3,-2 },{ -2,-2 },{ -1,-2 },{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 3,-2 },
		{ -3,-1 },{ -2,-1 },{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 2,-1 },{ 3,-1 },
		{ -3, 0 },{ -2, 0 },{ -1, 0 }	 ,	   { 1, 0 },{ 2, 0 },{ 3, 0 },
		{ -3, 1 },{ -2, 1 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 2, 1 },{ 3, 1 },
		{ -3, 2 },{ -2, 2 },{ -1, 2 },{ 0, 2 },{ 1, 2 },{ 2, 2 },{ 3, 2 },
		{ -3, 3 },{ -2, 3 },{ -1, 3 },{ 0, 3 },{ 1, 3 },{ 2, 3 },{ 3, 3 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;

		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }/*!< x座標の範囲外 */
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }/*!< y座標の範囲外 */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }/*!< ブロックが破壊 */

		auto component = TerrainManager::GetInstance().GetComponent(pos);
		/*! 養分の吸収可能上限/下限限界 */
		if (component.x <= ABSORPTION_UNDER_LIMIT ||
			component.x >= ABSORPTION_OVER_LIMIT) {
			continue;
		}

		/*! 吸収 */
		component.x -= ABSORPTION_POWER;
		m_Component.x += ABSORPTION_POWER;
		m_AbsorptionCount++;
		m_Hp += 2;
		if (MAX_HP < m_Hp) { m_Hp = MAX_HP; }
		m_AbsorptionCount++;
		if (component.x < 0)				{ component.x = 0; }				/*!< 養分0未満 */
		if (MAX_COMPONENT <= m_Component.x) { m_Component.x = MAX_COMPONENT; }	/*!< 養分99以上 */
		TerrainManager::GetInstance().SetComponent(pos, component);
		return true;
	}
	return false;
}

/*
	@brief	繁殖
*/
void Flower::Breeding()
{
	/*! HP減衰 */
	m_Hp -= 2;

	/*! HP0以上なら繁殖しない */
	if (0 < m_Hp) { return; }


	/*養分に応じた数繁殖する*/

	int Num = 0;/*!< 繁殖数 */

	if (m_Component.x < 5)		{ Num = 2; }
	else if (m_Component.x < 11){ Num = 3; }
	else if (m_Component.x < 16){ Num = 4; }
	else						{ Num = 5; }

	for (int i = 0; i < Num; i++) {
		Moss* child = new Moss;
		INT2 halfComponent = {
			m_Component.x / 2,
			m_Component.y / 2,
		};
		child->Init(m_IndexPos, halfComponent);
		EnemyManager::GetInstance().AddEnemy(child);
	}
	m_isDelete = true;
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}

/*
	@brief	保有養分・魔分の周囲への放出
*/
void Flower::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 }	  ,	   { 1, 0 },
		{ -1, 1 },{ 0, 1 },{ 1, 1 },
	};

	/*! 放出できるか判定 */
	int output = false;

	/*! 周囲に放出可能なブロックがあるか判定 */
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		/*! 外部参照 */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)	{ continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y)	{ continue; }
		/*! ブロック有無 */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		output = true;
		break;
	}

	/*! 放出できるブロックがない */
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