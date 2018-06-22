/*
	@file	Larvae.cpp
	@brief	幼虫
	@author	番場 宥輝
*/
#include "Larvae.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Larvae::MAX_HP = 64;
const unsigned int Larvae::POWER = 19;
const unsigned int Larvae::WAIT_COUNT = 60;
const unsigned int Larvae::GROW_COMPONENT = 20;
const unsigned int Larvae::GROW_HP = 20;
const unsigned int Larvae::STACK_SIZE = sizeof(Larvae::m_StackDirection) / sizeof(D3DXVECTOR2);
const D3DXVECTOR2 Larvae::INITIALIZE_STACK_ELEMENTS = { 0,0 };

/*
	@brief	namespace
*/
using namespace Game;

/*
	@brief	コンストラクタ
*/
Larvae::Larvae()
{
}

/*
	@brief	デストラクタ
*/
Larvae::~Larvae()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Larvae::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_pHero = NULL;
	m_isCollision = false;
	m_isDead = false;
	m_isDelete = false;
	m_Type = Type::LARVAE;
	m_Hp = 23;
	m_Atk = POWER;
	m_IndexPos = pos;
	m_Pos = ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.6f;
	m_AtkCount = 0;
	isMove = true;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.3f);
	m_pSprite->SetSplit(3, 4);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_AtkCount = 0;
	StackInit();
	state = STATE::WAIT;
	m_Debug.Init();

	/*! 進行先決定 */
	{
		D3DXVECTOR2 dir[] = {
			Game::UP,Game::DOWN,
			Game::LEFT,Game::RIGHT,
		};
		for (auto i : dir) {
			INT2 index = m_IndexPos + Game::ConvertIndex(i);
			if (index.x < 0 || SOIL_WIDTH <= index.x)  { continue; }
			if (index.y < 0 || SOIL_HEIGHT <= index.y) { continue; }
			if (!TerrainManager::GetInstance().GetBreakFlag(index)) { continue; }
			m_Direction = i;
			break;
		}
		m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
	}

	/*! 居場所の通知 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	解放
*/
void Larvae::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Larvae::Update()
{

	switch (state)
	{
		/*! 次の移動を割り当て */
	case Larvae::ASSIGN:

		break;
		/*! 移動 */
	case Larvae::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			IndexUpdate();
			/*! 成長可能なら */
			if (CheckGrow()) {
				state = STATE::ACTION;
			}
		}
		break;
		/*! 攻撃 */
	case Larvae::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
		state = STATE::MOVE;
		/*! CollisionはUpdateの後で呼び出しているため衝突していたらstateはATKになる */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
		/*! 待機 */
	case Larvae::WAIT:
		if (Wait()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			m_Count = 0;/*!< 待機カウンタをリセット */
			state = STATE::MOVE;
		}
		break;
		/*! アクション */
	case Larvae::ACTION:
		Grow();
		break;
	default:
		break;
	}

	/*! アニメーション更新 */
	AnimUpdate();
}

/*
	@brief	描画
*/
void Larvae::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Larvae::Damage(int damage)
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
void Larvae::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	コケとの衝突
*/
void Larvae::Collision(Moss & moss)
{
	
	/*! 衝突先に向きを変える */
	///*
	INT2 dir = moss.GetIndex() - m_IndexPos;
	if (ConvertDirection(dir) != D3DXVECTOR2{ 0, 0 }) {
		m_Direction = ConvertDirection(dir);
	}
	StackInit();
	isMove = !Game::TheSamePositionToGrid(m_Pos);
	//*/
	Predator(moss);
}

/*
	@brief	トカゲとの衝突
*/
void Larvae::Collision(Lizard & lizard)
{
	isMove = false;
	/*! 衝突先に向きを変える */
	INT2 dir = lizard.GetIndex() - m_IndexPos;
	m_Direction = ConvertDirection(dir);
}

/*
	@brief	戦士との衝突
*/
void Larvae::Collision(Soldier & soldier)
{
	state = STATE::ATK;
}

/*
	@brief	インデックス更新
*/
void Larvae::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = ConvertPositionToIndex(m_Pos);/*!< インデックス更新 */

	/*! 居場所を更新 */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< データの削除 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< データのセット */

	/*! 判定先 */
	D3DXVECTOR2 dirList[] = {
		ConvertLocalDirection(m_Direction,FORWARD),
		ConvertLocalDirection(m_Direction,LEFT),
		ConvertLocalDirection(m_Direction,RIGHT),
	};

	/*! 移動可能方向をスタックに格納 */
	for (auto i : dirList) {
		INT2 pos = m_IndexPos + ConvertIndex(i);
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		if (!TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		StackPushDirection(i);
	}

	/*! 格納方向の取り出し */
	if (StackElementsNum() != 0) {
		int index = rand() % StackElementsNum();	/*!< 乱数でインデックスを決定 */
		D3DXVECTOR2 dir = m_StackDirection[index];
		m_Direction = dir;
		m_NextIndexPos = m_IndexPos + ConvertIndex(dir);
	}
	/*! 進行方向の候補がない */
	else {
		D3DXVECTOR2 dir = ConvertLocalDirection(m_Direction, BACK);
		m_Direction = dir;
		m_NextIndexPos = m_IndexPos + ConvertIndex(dir);
	}
	/*! スタックの初期化 */
	StackInit();

	/*! HPを減らす */
	Damage(1);

}

/*
	@brief	保有養分・魔分の放出
*/
void Larvae::OutComponent()
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
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)  { continue; }
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
		component += 1;/*!< 1ずつ放出していく */

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
	@brief	移動
*/
bool Larvae::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}

	/*! 座標が移動先を超えていたら補正 */
	{
		bool isOver = false;/*!< 座標の超過フラグ */
		D3DXVECTOR2 pos = ConvertIndexToPosition(m_NextIndexPos);
		/*! 座標超過判定 */
		if (m_Direction == UP&&pos.y >= m_Pos.y)			{ isOver = true; }	/*!< 上 */
		else if (m_Direction == DOWN&&pos.y <= m_Pos.y)   { isOver = true; }	/*!< 下 */
		else if (m_Direction == LEFT&&pos.x >= m_Pos.x)   { isOver = true; }	/*!< 左 */
		else if (m_Direction == RIGHT&&pos.x <= m_Pos.x)  { isOver = true; }	/*!< 右 */

		if (isOver) {
			m_Pos = pos;
		}
	}

	return !Game::TheSamePositionToGrid(m_Pos);
}

/*
	@brief	待機
	@return	終了:未終
*/
bool Larvae::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	攻撃
*/
void Larvae::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	成長できるか判定
*/
bool Larvae::CheckGrow()
{
	return GROW_HP < m_Hp&&GROW_COMPONENT < m_Component.x;
}

/*! 成長 */
void Larvae::Grow()
{
	Chrysalis* grow = new Chrysalis;
	grow->Init(m_IndexPos, m_Component, m_Hp);
	EnemyManager::GetInstance().AddEnemy(grow);
	/*!< データの削除 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	捕食
*/
void Larvae::Predator(Moss & moss)
{
	m_Hp += m_Atk < moss.GetHp() ? m_Atk : moss.GetHp();
	m_Component.x += moss.GetComponent().x;
	moss.Damage(m_Atk); 
	if (moss.GetDeleteFlags()) {
		isMove = true;
	}
}

/*
	@brief	スタックの初期化
*/
void Larvae::StackInit()
{
	for (auto &i : m_StackDirection) {
		i = INITIALIZE_STACK_ELEMENTS;
	}
	m_StackIndex = 0;
}

/*
	@brief	スタックへのプッシュ
*/
void Larvae::StackPushDirection(D3DXVECTOR2 direction)
{
	m_StackDirection[m_StackIndex] = direction;
	m_StackIndex++;
}
/*
@brief	アニメーション更新
*/
void Larvae::AnimUpdate()
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

/*
	@brief	スタックに格納された要素の数を返す
	@return	初期値でないスタックの要素数
*/
int Larvae::StackElementsNum()
{
	int ret = 0;
	for (auto i : m_StackDirection) {
		/*! 初期値と一致しているか判定 */
		if (i == INITIALIZE_STACK_ELEMENTS) { return ret; }
		ret++;/*!< カウンタに加算 */
	}
	/*! スタックすべてに値が入っている */
	return STACK_SIZE;
}
