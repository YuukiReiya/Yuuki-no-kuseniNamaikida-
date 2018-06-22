/*
	@file	Soldier.cpp
	@brief	剣士
	@author	番場 宥輝
*/
#include "Soldier.h"

#include "../../../../Debug/Debug.h"
#include "../../../Manager/TycoonManager/TycoonManager.h"

/*
	@def	定数宣言
*/
const unsigned int Soldier::WAIT_COUNT = 20;

/*
	@brief	コンストラクタ
*/
Soldier::Soldier()
{
}

/*
	@brief	デストラクタ
*/
Soldier::~Soldier()
{
	Destroy();
}

/*
	@brief	初期化
*/
void Soldier::Init()
{
	m_pSprite = new Sprite;
	m_pEnemy = NULL;

	/*! スプライト関連 */
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSplit(3, 4);

	/*! パラメータ */
	m_Pos = Game::ConvertIndexToPosition(INT2{ HERO_POP_X,0 });
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);
	m_isCaught = false;
	m_Count = 0;
	state = STATE::MOVE;
	m_TargetIndexPos = INT2();
	aStar.Init(
		m_IndexPos.x, m_IndexPos.y,
		TycoonManager::GetInstance().GetTycoonPos().x, TycoonManager::GetInstance().GetTycoonPos().y,
		SOIL_WIDTH, SOIL_WIDTH
	);
	if (!aStar.Explore())MSG("魔王までの最短距離を求めることが出来ませんでした");

	/*! 移動目標取り出し */
	{
		aStar.RootHash.pop_back();
		auto next = aStar.GetRoot();
		m_NextIndexPos = { next.x,next.y };
	}

	m_Debug.Init();
}

/*
	@brief	解放
*/
void Soldier::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief　更新
*/
void Soldier::Update()
{
	/*! 地形データの更新 */
	if (!m_isCaught)aStar.UpdateTerrain();

	switch (state)
	{
		/*! 次の移動を割り当て */
	case Soldier::ASSIGN:

		break;
		/*! 移動 */
	case Soldier::MOVE:
		if (!Move()) {
			/*! 移動終了 */
			IndexUpdate();

		}
			
		break;
		/*! 攻撃 */
	case Soldier::ATK:
		/*! CollisionはUpdateの後で呼び出しているため衝突していたらstateはATKになる */
		state = STATE::MOVE;
		if (Wait()) {
			/*! 待機終了 */
			m_pEnemy->Damage(m_Atk);
		}
		break;
		/*! 待機 */
	case Soldier::WAIT:
		
		break;
		/*! 捕縛 */
	case Soldier::CATCH:
		state = STATE::MOVE;
		break;
	default:
		break;
	}



	/*! 移動 */
//	Move();

	/*! アニメーション更新 */
	AnimUpdate();
}

/*
	@brief	描画
*/
void Soldier::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	インデックスの更新
*/
void Soldier::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< インデックス更新 */

	if (before == m_IndexPos) { return; }

	/*! 移動先更新 */
	if (!aStar.RootHash.empty()) {
		auto next = aStar.GetRoot();
		m_NextIndexPos = { next.x,next.y };
	}

}

/*
	@brief	魔王捕縛
*/
void Soldier::Catch()
{
	/*! スタート地点までの最短経路を求める */
	m_TargetIndexPos = { GAME_OVER_X,GAME_OVER_Y };
	aStar.Init(
		m_IndexPos.x, m_IndexPos.y,
		m_TargetIndexPos.x, m_TargetIndexPos.y,
		SOIL_WIDTH, SOIL_HEIGHT
	);
	if (!aStar.Explore())MSG("探索失敗");
	aStar.RootHash.pop_back();
	auto next = aStar.GetRoot();
	m_NextIndexPos = { next.x,next.y };
}

/*
	@brief	ダメージ
*/
void Soldier::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		/*
			死亡処理
			（勇者個々）
		*/
	}
}

/*
	@brief 衝突
*/
void Soldier::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	相手の情報を取得し攻撃状態へ遷移
*/
void Soldier::Battle(Enemy & enemy)
{
	m_pEnemy = &enemy;
	state = STATE::ATK;
}


/*
	@brief	アニメーションの更新
*/
void Soldier::AnimUpdate()
{
	m_AnimCount++;
	/*! 止まっていたら足踏みしない */
	if (m_Move == 0)m_AnimIndexU = 1;
	else {
		if (m_AnimCount > 24) {
			m_AnimCount = 0;
		}
		m_AnimIndexU = m_AnimCount > 12 ? 2 : 0;
	}

	/*! 向きに応じた画像のインデックスを指定 */
	if (m_Direction == Game::UP)
		m_AnimIndexV = 3;
	else if (m_Direction == Game::DOWN)
		m_AnimIndexV = 0;
	else if (m_Direction == Game::LEFT)
		m_AnimIndexV = 1;
	else if (m_Direction == Game::RIGHT)
		m_AnimIndexV = 2;

	m_AnimIndexU %= 3;
	
}

/*
	@brief	移動
*/
bool Soldier::Move()
{
	/*! 向きの更新 */
	m_Direction = {
		(float)(m_NextIndexPos - m_IndexPos).x,
		(float)(m_NextIndexPos - m_IndexPos).y,
	};

	m_Pos += m_Direction*m_Move;

	/*! 座標が移動先を超えていたら補正してあげる */
	{
		bool isOver = false;/*!< 座標の超過フラグ */
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! 座標超過判定 */
		if (m_Direction == Game::UP&&temp.y >= m_Pos.y) { isOver = true; }			/*!< 上 */
		else if (m_Direction == Game::DOWN&&temp.y <= m_Pos.y) { isOver = true; }	/*!< 下 */
		else if (m_Direction == Game::LEFT&&temp.x >= m_Pos.x) { isOver = true; }	/*!< 左 */
		else if (m_Direction == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	/*!< 右 */

		/*! 超過座標の補正 */
		if (isOver)m_Pos = temp;
	}
	return !Game::TheSamePositionToGrid(m_Pos);

}

/*
	@brief	待機
*/
bool Soldier::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	m_Count = 0;
	return true;
}