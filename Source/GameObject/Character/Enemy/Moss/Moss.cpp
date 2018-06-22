/*
	@file	Moss.cpp
	@brief	コケ
	@author	番場 宥輝
*/
#include "Moss.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/	
const unsigned int Moss::GROW_COMPONENT = 2;
const unsigned int Moss::GROW_HP = 2;
const unsigned int Moss::RECOVERY_POWER = 2;
const unsigned int Moss::MAX_HP = 21;
const unsigned int Moss::WAIT_COUNT = 30;

/*
	コンストラクタ
*/
Moss::Moss()
{
}

/*
	デストラクタ
*/
Moss::~Moss()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Moss::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_pHero = NULL;
	m_isCollision = false;
	m_isDead = false;
	m_isDelete = false;
	m_Type = Type::MOSS;
	m_Hp = 16;
	m_Atk = 13;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.3f;
	isMove = true;
	state = STATE::WAIT;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSplit(3, 4);

	m_AbsorptionPow = 3;
	isAgain = false;
	isAbsorp = true;
	m_Count = 0;
	
	m_Debug.Init();
	m_Debug.SetLineWidth(1.0f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;

	/*! 進行先決定 */
	{

		D3DXVECTOR2 dir[] = {
			Game::UP,Game::DOWN,
			Game::LEFT,Game::RIGHT,
		};
		m_Direction = dir[rand() % sizeof(dir) / sizeof(D3DXVECTOR2)];
		m_Direction = CalculateDirection();
		m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
	}

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	解放
*/
void Moss::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Moss::Update()
{

	switch (state)
	{
	/*! 次の移動を割り当て */
	case Moss::ASSIGN:

		break;
	/*! 移動 */
	case Moss::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			state = STATE::MOVE;
			IndexUpdate();
			/*! 養分吸収 */
			if (isAbsorp) {
				/*! 養分吸収出来るか判定 */
				if (CheckAbsorption()) {
					state = STATE::ACTION;
				}
			}
			/*! 養分放出 */
			else {
				/*! 養分放出可能か判定 */
				if (CheckDivergence()) {
					state = STATE::ACTION;
				}
			}
			/*! 成長 */
			if (CheckGrow()) {
				Grow();
			}
		}
		break;
	/*! 攻撃 */
	case Moss::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
		state = STATE::MOVE;
		/*! CollisionはUpdateの後で呼び出しているため衝突していたらstateはATKになる */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
	/*! 待機 */
	case Moss::WAIT:
		if (Wait()) {
			/*! 待機終了 */
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			m_Count = 0;/*!< 待機カウンタをリセット */
			state = STATE::MOVE;
		}
		break;
	/*! アクション */
	case Moss::ACTION:
		if (Wait()) {
			/*! 待機終了 */
			m_Count = 0;/*!< 待機カウンタをリセット */
			if (isAbsorp) {
				Absorption();/*!< 養分吸収 */
			}
			else {
				Divergence();/*!< 養分放出 */
			}

			state = STATE::MOVE;
		}
		break;
	default:
		break;
	}

	/*! アニメーション更新 */
	AnimUpdate();

	/*! 移動フラグを戻す */
	if (!isMove) { isMove = true; }
}

/*
	@brief	描画
*/
void Moss::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Moss::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		/*!< データの削除 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
		m_isDelete = true;
	}
}

/*
	@brief	衝突
*/
void Moss::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	幼虫との当たり判定
*/
void Moss::Collision(Larvae& larvae)
{
	/*! 移動を止める */
	isMove = false;
	/*! 衝突先に向きを変える */
	INT2 dir = larvae.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);

}

/*
	@brief	成虫との当たり判定
*/
void Moss::Collision(Adult & adult)
{
	/*! 移動を止める */
	isMove = false;
	/*! 衝突先に向きを変える */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	戦士との衝突
*/
void Moss::Collision(Soldier & solder)
{
	state = STATE::ATK;
}

/*
	@brief	待機
	@return	終了:未終
*/
bool Moss::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	攻撃
*/
void Moss::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	インデックスの更新
*/
void Moss::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< インデックス更新 */
	
	/*! 居場所を更新 */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< データの削除 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< データのセット */

	/*! HPの減衰処理 */
	Damage(1);

	/*! 進行方向の算出 */
	m_Direction = CalculateDirection();
	INT2 dir = Game::ConvertIndex(m_Direction);
	m_NextIndexPos = m_IndexPos + dir;
}

/*
	@brief	移動
	@return	終了:未終
*/
bool Moss::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}

	/*! 座標が移動先を超えていたら補正 */
	{
		bool isOver = false;/*!< 座標の超過フラグ */
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! 座標超過判定 */
		if (m_Direction == Game::UP&&temp.y >= m_Pos.y)			{ isOver = true; }	/*!< 上 */
		else if (m_Direction == Game::DOWN&&temp.y <= m_Pos.y)	{ isOver = true; }	/*!< 下 */
		else if (m_Direction == Game::LEFT&&temp.x >= m_Pos.x)	{ isOver = true; }	/*!< 左 */
		else if (m_Direction == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	/*!< 右 */

		if (isOver) {
			m_Pos = temp;
		}
	}
	return !Game::TheSamePositionToGrid(m_Pos);
}

/*
	@brief	養分吸収出来るか判定
*/
bool Moss::CheckAbsorption()
{
	INT2 around[] = {
		{  0,-1 },{ 0, 1 },
		{ -1, 0 },{ 1, 0 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		if (pos.x < 0 || pos.x >= SOIL_WIDTH) { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! 隣接マスの養分 */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x == 0) { continue; }/*!< 養分0のブロック */
		return true;/*!< 四方のブロックのどれかは養分吸収が出来る */
	}
	return false;
}

/*
	@brief	養分吸収
*/
void Moss::Absorption()
{
	INT2 way[] = {
		{  0,-1 },{ 0, 1 },
		{ -1, 0 },{ 1, 0 },
	};

	for (auto i : way) {
		INT2 pos = m_IndexPos + i;
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! 隣接マスの養分 */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x == 0) { continue; }/*!< 養分0のブロック */
		/*! 養分の吸収 */
		component.x -= m_AbsorptionPow;
		m_Component.x += m_AbsorptionPow;
		m_Hp += RECOVERY_POWER;/*! 基本回復量 */

		/*! 保有養分が最大値を上回る */
		if (MAX_COMPONENT < m_Component.x) {
			m_Component.x = MAX_COMPONENT;
		}

		/*! ブロックの養分が最小値を下回る */
		if (component.x < 0) {
			m_Component.x += (m_AbsorptionPow - abs(component.x));/*!< 吸収可能分を吸い取る */
			component.x = 0;/* ブロックの養分を0 */
			m_Hp -= 1;		/*!< 吸収量より低いのでHP回復量を減らす */
		}

		/*! ブロックの養分上書き */
		TerrainManager::GetInstance().SetComponent(pos, component);
		isAbsorp = false;
		break;
	}
}

/*
	@brief	養分放出可能か判定
*/
bool Moss::CheckDivergence()
{
	/*! 周囲 */
	INT2 around[4] = {
		{ 0,-1 },{ 0, 1 },
		{ -1,0 },{ 1, 0 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		/*! 範囲外 */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH) { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		/*! ブロックが壊れてる */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! 隣接マス養分 */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x <= 1) { continue; }/*!< 1以下のマスには放出しない */
		return true;
	}
	return false;
}

/*
	@brief	養分放出
*/
void Moss::Divergence()
{
	/*! 周囲 */
	INT2 way[4] = {
		{ 0,-1 },
		{ 0,1 },
		{ -1,0 },
		{ 1, 0 },
	};
	for (int i = 0; i < sizeof(way) / sizeof(INT2); i++) {
		INT2 pos = m_IndexPos + way[i];
		/*! 範囲外 */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)  { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		/*! ブロックが壊れてる */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! 隣接マス養分 */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x <= 1)continue;/*!< 1以下のマスには放出しない */
		/*! 養分の放出 */
		component.x += (m_Component.x - 1);
		m_Component.x = 1;
		TerrainManager::GetInstance().SetComponent(pos, component);
		isAbsorp = true;
	}
}

/*
	@brief	保有養分・魔分の周囲への放出
*/
void Moss::OutComponent()
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

/*
	@brief	成長出来るか判定

*/
bool Moss::CheckGrow()
{
	return m_Hp <= GROW_HP&&GROW_COMPONENT <= m_Component.x;
}

/*
	@brief	成長
*/
void Moss::Grow()
{
	Buds* grow = new Buds;
	grow->Init(m_IndexPos, m_Component, m_Hp);
	EnemyManager::GetInstance().AddEnemy(grow);
	m_isDelete = true;
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}


/*
	@brief	移動可能方向の算出
	@detail	現在向いている方向から移動可能な方向を算出
*/
D3DXVECTOR2 Moss::CalculateDirection()
{
	/*! 周囲 */
	D3DXVECTOR2 way[] = {
		Game::FORWARD,
		Game::RIGHT,
		Game::LEFT,
		Game::BACK,
	};

	for (auto i : way) {
		D3DXVECTOR2 dir = Game::ConvertLocalDirection(m_Direction, i);/*!< ローカルな向き */
		INT2 pos = m_IndexPos + Game::ConvertIndex(dir);/*!< 判定先インデックス */
		/*! 外部参照 */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)  { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { return dir; }
	}
	return D3DXVECTOR2(0, 0);
}

/*
	@brief	アニメーションの更新
*/
void Moss::AnimUpdate()
{
	m_AnimCount++;
	
	if (m_AnimCount > 8) {
		m_AnimIndexU++;
		if (m_AnimIndexU > 2)m_AnimIndexU = 0;
		m_AnimCount = 0;
	}
	
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
	m_AnimIndexU %= 3;
}

