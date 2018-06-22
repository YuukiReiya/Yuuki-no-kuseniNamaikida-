/*
	@file	Lizard.h
	@brief	トカゲ
	@author	番場 宥輝
*/
#include "Lizard.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	定数宣言
*/
const unsigned int Lizard::MAX_HP = 160;
const unsigned int Lizard::WAIT_COUNT = 30;
const unsigned int Lizard::MAKE_NEST_COMPONENT = 14;
const unsigned int Lizard::MADE_NEST_HP = 2;
const unsigned int Lizard::LAY_EGG_COMPONENT = 12;
const unsigned int Lizard::LAY_EGG_HP = 40;
const unsigned int Lizard::STACK_SIZE = sizeof(Lizard::m_Direction) / sizeof(D3DXVECTOR2);
const D3DXVECTOR2 Lizard::INITIALIZE_STACK_ELEMENTS = { 0,0 };

/*
	@brief	コンストラクタ
*/
Lizard::Lizard()
{
}

/*
	@brief	デストラクタ
*/
Lizard::~Lizard()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Lizard::Init(INT2 pos,INT2 component)
{
	m_pSprite = new Sprite;


	m_Type = Type::LIZARD;
	m_isDelete = false;
	m_Hp = MAX_HP;
	m_Atk = 35;
	m_IndexPos = pos;
	state = STATE::WAIT;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.6f;
	m_Direction = Game::RIGHT;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSplit(3, 4);
	m_pSprite->SetSizeScaling(1.3f);
	isMove = true;
	isStraight = true;
	isMadeNest = false;
	m_Count = 0;
	m_NestIndexPos = pos;
	m_AtkCount = 0;
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	StackInit();

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
void Lizard::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	更新
*/
void Lizard::Update()
{
	switch (state)
	{
	/*! 次の移動を割り当て */
	case Lizard::ASSIGN:

		break;
	/*! 移動 */
	case Lizard::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			state = STATE::MOVE;
			IndexUpdate();
		}
		break;
	/*! 攻撃 */
	case Lizard::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
		state = STATE::MOVE;
		/*! CollisionはUpdateの後で呼び出しているため衝突していたらstateはATKになる */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
	/*! 待機 */
	case Lizard::WAIT:
		if (Wait()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< 衝突判定リストへ追加 */
			m_Count = 0;/*!< 待機カウンタをリセット */
			state = STATE::MOVE;
		}
		break;
	/*! アクション */
	case Lizard::ACTION:
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
void Lizard::Render(IDirect3DTexture9* texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	ダメージ
*/
void Lizard::Damage(int damage)
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
void Lizard::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	幼虫と衝突
*/
void Lizard::Collision(Larvae & larvae)
{
	
	/*! 衝突先に向きを変える */
	INT2 dir = larvae.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += larvae.GetComponent().x;
	larvae.Damage(m_Atk);
}

/*
	@brief	蛹と衝突
*/
void Lizard::Collision(Chrysalis & chrysalis)
{
	/*! 衝突先に向きを変える */
	INT2 dir = chrysalis.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += chrysalis.GetComponent().x;
	chrysalis.Damage(m_Atk);
}

/*
	@brief	成虫と衝突
*/
void Lizard::Collision(Adult & adult)
{
	/*! 衝突先に向きを変える */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += adult.GetComponent().x;
	adult.Damage(m_Atk);
}

/*
	@brief	戦士との衝突
*/
void Lizard::Collision(Soldier & solder)
{
	state = STATE::ATK;
}

/*
	@brief	卵を産めるか判定
*/
bool Lizard::CheckLay()
{
	return LAY_EGG_HP < m_Hp &&  LAY_EGG_COMPONENT < m_Component.x&&m_Hp < 130;
}

/*
	@brief	卵を産む
*/
void Lizard::Lay()
{
	m_Hp -= LAY_EGG_HP;
	m_Component.x -= LAY_EGG_COMPONENT;

	Egg*  egg = new Egg;
	egg->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(egg);
}

/*
	@brief	インデックス更新
*/
void Lizard::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< インデックス更新 */

	/*! インデックスに変更がなかったら処理を抜ける */
	if (m_IndexPos == before) { return; }

	/*! 居場所を更新 */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< データの削除 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< データのセット */

	if (!isMadeNest&&CheckMakeNest()) {
		MakeNest();
	}

	if (isStraight)
		StraightMoveIndexUpdate();
	else
		NestMoveIndexUpdate();
}

/*
	@brief	アニメーション更新
*/
void Lizard::AnimUpdate()
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
	@brief	移動
*/
bool Lizard::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}
	/*! 座標が移動先を超えていたら補正 */
	{
		bool isOver = false;/*!< 座標の超過フラグ */
		D3DXVECTOR2 pos = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! 座標超過判定 */
		if (m_Direction == Game::UP&&pos.y >= m_Pos.y) { isOver = true; }			/*!< 上 */
		else if (m_Direction == Game::DOWN&&pos.y <= m_Pos.y) { isOver = true; }	/*!< 下 */
		else if (m_Direction == Game::LEFT&&pos.x >= m_Pos.x) { isOver = true; }	/*!< 左 */
		else if (m_Direction == Game::RIGHT&&pos.x <= m_Pos.x) { isOver = true; }	/*!< 右 */

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
bool Lizard::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	攻撃
*/
void Lizard::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	巣が作れるか判定
*/bool Lizard::CheckMakeNest()
{
	/*! HPと養分が一定以上なければ処理しない */
	if (m_Hp <= MADE_NEST_HP || m_Component.x <= MAKE_NEST_COMPONENT) {
		return false;
	}
	/*! 周囲のマスを判定 */
	INT2 around[8]{
		{ -1, -1},{  0,-1 },{  1,-1 },
		{ -1, 0 },			{  1, 0 },
		{ -1, 1 },{  0, 1 },{  1, 1 },
	};
	int count = 0;/*!< 移動可能なマスの個数 */
	for (int i = 0; i < sizeof(around) / sizeof(INT2); i++) {
		INT2 index = m_IndexPos + around[i];
		/*! 外部参照 */
		if (index.x < 0 || index.x >= SOIL_WIDTH)	{ continue; }
		if (index.y < 0 || index.y >= SOIL_HEIGHT)	{ continue; }
		/*! ブロック有無 */
		if (TerrainManager::GetInstance().GetBreakFlag(index)) { count++; }
	}
	/*! 周囲5マスが移動可能 */
	if (5 <= count) { return true; }
	return false;
}

/*
	@brief	巣を作る
*/
void Lizard::MakeNest()
{
	/*
		巣のインスタンス生成処理
	*/
	m_NestIndexPos = m_IndexPos;
	isMadeNest = true;
	isStraight = false;
}

/*
	@brief	保有養分・魔分の周囲への放出
*/
void Lizard::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 }	  ,{ 1, 0 },
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

	/*! 放出できるブロックがない */
	if (!output) { return; }

	for (int i = 0;; i++) {
		/*! 放出する養分・魔分が無い */
		if (m_Component == INT2(0, 0)) { break; }/*!< 処理を抜ける */
												 /*! 放出位置 */
		INT2 pos = m_IndexPos + around[i %( sizeof(around) / sizeof(INT2))];
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
	@brief	進行方向の算出
	@detail	現在向いている方向から移動可能な方向を算出
*/
D3DXVECTOR2 Lizard::CalculateDirection()
{
	D3DXVECTOR2 dir;
	INT2 temp;

	/*! 周囲 */
	D3DXVECTOR2 way[3] = {
		Game::FORWARD,
		Game::RIGHT,
		Game::LEFT,
	};

	for (int i = 0; i < sizeof(way) / sizeof(D3DXVECTOR2); i++) {
		dir = Game::ConvertLocalDirection(m_Direction, way[i]);
		temp = Game::ConvertIndex(dir);
		temp = m_IndexPos + temp;
		if (temp.x < 0 || temp.x >= SOIL_WIDTH)		{ continue; }
		if (temp.y < 0 || temp.y >= SOIL_HEIGHT)	{ continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(temp))return dir;
	}
	//左右が進めないので来た方向へ向きを変える
	dir = Game::ConvertLocalDirection(m_Direction, Game::BACK);
	return dir;
}

/*
	@brief	直進移動するためのインデックス更新
*/
void Lizard::StraightMoveIndexUpdate()
{
	m_Direction = CalculateDirection();
	m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
}

/*
	@brief	巣の周囲を徘徊する移動
*/void Lizard::NestMoveIndexUpdate()
{
	/*! 巣のある場所に居たら */
	if (m_IndexPos == m_NestIndexPos) {
		/*! 4隅 */
		INT2 corner[]{
			{-1,-1},{1,-1},
			{-1, 1},{1, 1},
		};
		/*! 移動目標候補策定 */
		std::vector<INT2>stack;
		for (auto i : corner) {
			INT2 target = m_IndexPos + i;
			/*! 外部参照 */
			if (target.x < 0 || SOIL_WIDTH <= target.x)  { continue; }
			if (target.y < 0 || SOIL_HEIGHT <= target.y) { continue; }
			/*! ブロックが壊れてない */
			if (!TerrainManager::GetInstance().GetBreakFlag(target)) { continue; }
			/*! 
				□■□
				■★□	←左上のような状態は移動目標に設定しない
				□□□
			*/
			INT2 v = m_IndexPos + INT2(0, i.y);/*!< 縦 */
			INT2 h = m_IndexPos + INT2(i.x, 0);/*!< 横 */
			if (TerrainManager::GetInstance().GetBreakFlag(v) ||
				TerrainManager::GetInstance().GetBreakFlag(h)) {
				stack.push_back(target);/*!< 移動目標候補をスタックに積む */
			}
		}
		
		/*! データの取り出し */
		m_GoalIndexPos = stack[rand() % stack.size()];

		/*! 目標地点までの最短経路を探索 */
		m_aStar.Init(
			m_IndexPos.x, m_IndexPos.y,
			m_GoalIndexPos.x, m_GoalIndexPos.y,
			SOIL_WIDTH, SOIL_HEIGHT);
		m_aStar.Explore();

		/*! 最短経路の取り出し */
		m_aStar.RootHash.pop_back();
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };

		/*! ローカル動的配列の解放 */
		stack.clear();
		stack.shrink_to_fit();
		
		/*! 卵を産めるか判定 */
		if (CheckLay()) {
			Lay();
		}
	}
	/*! 移動目標に到達 */
	else if (m_IndexPos == m_GoalIndexPos) {
		/*! 巣の位置を移動目標に */
		m_GoalIndexPos = m_NestIndexPos;
		/*! 最短経路を探索 */
		m_aStar.Init(
			m_IndexPos.x, m_IndexPos.y,
			m_GoalIndexPos.x, m_GoalIndexPos.y,
			SOIL_WIDTH, SOIL_HEIGHT);
		m_aStar.Explore();	
		/*! 最短経路を取り出し */
		m_aStar.RootHash.pop_back();
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };
	}
	/*! 中継地点 */
	else {
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };
	}
}

/*
	@brief	スタックの初期化
*/
void Lizard::StackInit()
{
	for (auto &i : m_StackDirection) {
		i = INITIALIZE_STACK_ELEMENTS;
	}
	m_StackIndex = 0;
}

/*
	@brief	スタックへのプッシュ
*/
void Lizard::StackPushDirection(D3DXVECTOR2 direction)
{
	m_StackDirection[m_StackIndex] = direction;
	m_StackIndex++;
}

/*
	@brief	スタックに格納された要素の数を返す
	@return	初期値でないスタックの要素数
*/
int Lizard::StackElementsNum()
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
