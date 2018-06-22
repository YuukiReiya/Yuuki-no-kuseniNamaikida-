/*
	@file	Player.cpp
	@brief	プレイヤー
	@author	番場 宥輝
*/
#include "Player.h"
#include "../Character/Enemy/Larvae/Larvae.h"
#include "../Manager/InputManager/InputManager.h"
#include "../Manager/UiManager/UiManager.h"
#include "../Manager/SoundManager/SoundManager.h"

/*
	@def	定数
*/
const unsigned int Player::ATK = 30;


/*
	@brief	コンストラクタ
*/
Player::Player()
{
}

/*
	@brief	デストラクタ
*/
Player::~Player()
{
}

/*
	@brief	初期化
*/
void Player::Init()
{
	m_IndexPos = { 28,4 };
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
	m_Pickel.Init();
	if (!m_Debug.Init()) {
		MSG("デバッグ失敗");
	}

}

/*
	@brief	解放
*/
void Player::Destroy()
{
	m_Pickel.Destroy();
}

/*
	@brief	更新
*/
void Player::Update()
{
	/*! 座標の更新 */
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
}

/*
	@brief	レンダリング
*/
void Player::Render()
{
	D3DXVECTOR2 Pos{
		m_Pos.x, 
		m_Pos.y,
	};
	m_Debug.DrawSquare(ConvertView(Pos), (float)GRID_SIZE, (float)GRID_SIZE);
	m_Pickel.Render(ConvertView(Pos));
}

/*
	@brief	移動
*/
void Player::Move()
{
	/*! 左キー */
	if (input.GetKeyDown(input.LEFT)) {
		if (m_IndexPos.x > 0)
			m_IndexPos.x -= 1;
	}
	/*! 右キー */
	else if (input.GetKeyDown(input.RIGHT)) {
		if (m_IndexPos.x < (SOIL_WIDTH - 1))
			m_IndexPos.x += 1;
	}
	/*! 上キー */
	else if (input.GetKeyDown(input.UP)) {
		if (m_IndexPos.y > 0)
			m_IndexPos.y -= 1;
	}
	/*! 下キー */
	else if (input.GetKeyDown(input.DOWN)) {
		if (m_IndexPos.y < (SOIL_HEIGHT - 1))
			m_IndexPos.y += 1;
	}
}

/*
	@brief	プレイヤーが行うアクション
	@detail	ブロックに対して「堀」、モンスターに対して「間引く」
*/
void Player::Action()
{
	/*! キーを押されたら */
	if (input.GetKeyDown(input.DIG)) {
		/*! ブロックが有る */
		if (!TerrainManager::GetInstance().GetBreakFlag(m_IndexPos) && 0 < m_DigPower) {
			Break(m_IndexPos);
		}
		/*! ブロックが無し */
		else {
			Attack(m_IndexPos);
		}
	}
}

/*
	@brief	土ブロックの破壊
*/
void Player::Break(INT2 pos)
{
	/*! 隣接ブロックが壊れていれば進む */
	if (!ContactBleakedBlock(pos)) { 
		SoundManager::GetInstance().Play(SE::DIGMISS, false);
		return; 
	}
	m_DigPower--;
	SoundManager::GetInstance().Play(SE::DIG, false);
	TerrainManager::GetInstance().Break(pos);
	auto component = TerrainManager::GetInstance().GetComponent(pos);
	/* 養分・魔分を持っていればモンスター生成処理 */
	if (component == INT2{ 0,0 }) { return; }
	EnemyManager::GetInstance().Produce(pos, component);
}

/*
	@brief	モンスターを攻撃
	@detail	インデックスの最前面にいるモンスターを攻撃
*/
void Player::Attack(INT2 pos)
{
	Enemy* enemy = LocationManager::GetInstance().GetTopFront(pos);
	if (enemy == NULL) { return; }
	enemy->Damage(ATK);
}

/*
	@brief	魔王の待機位置設定
*/
bool Player::SetTycoon()
{
	if (input.GetKeyDown(input.DECIDE) && TerrainManager::GetInstance().GetBreakFlag(m_IndexPos)) {
		TycoonManager::GetInstance().SetTycoonPos(m_IndexPos);
		return true;
	}
	return false;
}

/*
	@brief	堀パワー設定
*/
void Player::SetDigPower(int power)
{
	m_DigPower = power;
}

/*
	@brief	堀パワー表示
*/
void Player::DrawDigPower()
{
	UiManager::GetInstance().DrawDigPower(m_DigPower);
}

/*
	@brief	隣接マスに壊れたブロックがあるかチェック
*/
bool Player::ContactBleakedBlock(INT2 pos)
{
	/*! 隣接マス */
	INT2 contact[4] = {
		{ 0,-1 },/*!< 上 */
		{ 0, 1 },/*!< 下 */
		{ -1,0 },/*!< 左 */
		{ 1, 0 },/*!< 右 */
	};

	for (auto i : contact) {
		INT2 index = pos + i;

		if (index.x < 0 || SOIL_WIDTH <= index.x) { continue; }
		if (index.y < 0 || SOIL_HEIGHT <= index.y) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(index)) { return true; }
	}
	/*! 隣接するブロックはどこも壊れていない */
	return false;
}
