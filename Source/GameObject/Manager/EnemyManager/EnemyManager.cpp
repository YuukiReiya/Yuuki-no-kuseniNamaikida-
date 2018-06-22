/*
	@file	EnemyManager.cpp
	@brief	出現モンスターを管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "EnemyManager.h"
#include "../../Character/Enemy/Moss/Moss.h"
#include "../../Character/Enemy/Larvae/Larvae.h"
#include "../../Character/Enemy/Lizard/Lizard.h"

/*
	@brief	コンストラクタ
*/
EnemyManager::EnemyManager()
{
}

/*
	@brief	デストラクタ
*/
EnemyManager::~EnemyManager()
{
	Destroy();
}

/*
	@brief	初期化
*/
void EnemyManager::Init()
{
	m_pEnemyList = new EnemyList;
	
	m_MossTexture = new Texture;
	m_BudsTexture = new Texture;
	m_FlowerTexture = new Texture;

	m_LarvaeTexture = new Texture;
	m_ChrysalisTexture = new Texture;
	m_AdultTexture = new Texture;

	m_EggTexture = new Texture;
	m_LizardTexture = new Texture;
	
	m_pEnemyList->Init();
	LoadTexture();
}

/*
	@brief	更新
*/
void EnemyManager::Update()
{
	ENEMY_NODE* p;
	p = m_pEnemyList->m_pHead->pNext;
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		p->data->Update();
		/*! 削除フラグが立ってればノードをつなぎなおす */
		if (p->data->GetDeleteFlags()) {
			ENEMY_NODE* del;
			del = p;
			p->pPrev->pNext = p->pNext;
			p->pNext->pPrev = p->pPrev;
			p = p->pNext;
			SAFE_DELETE(del);
		}
		/*! 立ってなければpには次ポンタを入れる */
		else {
			p = p->pNext;
		}
	}

	/*! 衝突判定 */
	CollisionEtoE();
	CollisionEtoH();
}

/*
	@brief	モンスター同士の当たり判定
*/
bool EnemyManager::Hit(Enemy & A, Enemy & B)
{
	///*
	auto w = abs(A.GetPos().x - B.GetPos().x);
	auto h = abs(A.GetPos().y - B.GetPos().y);
	if ((w <= 10 && h <= (GRID_SIZE)) || (h == 10 && w <= (GRID_SIZE))) {
		return true;
	}
	return false;
	//*/
	/*
	D3DXVECTOR2 dir[4] = {
		Game::UP,
		Game::DOWN,
		Game::LEFT,
		Game::RIGHT,
	};
	for (auto i : dir) {
		INT2 index = A.GetIndex() + Game::ConvertIndex(i);
		if (index.x < 0 || index.x >= SOIL_WIDTH) { continue; }
		if (index.y < 0 || index.y >= SOIL_HEIGHT) { continue; }
		if (index == B.GetIndex()) { return true; }
	}
	return false;
	*/
}
/*
	@brief	モンスターと勇者の当たり判定
*/
bool EnemyManager::Hit(Enemy & enemy)
{

	INT2 way[4] = {
		{ -1,0 },{ 1,0 },
		{ 0,-1 },{ 0,1 }
	};
	for (auto i : way) {
		INT2 pos = enemy.GetIndex() + i;
		auto hero = HeroManager::GetInstance().GetHero(pos);

		/*
			勇者が死んだときにモンスターのm_pHeroがNULLになるように注意
		*/
		enemy.SetHero(hero);
		if (hero) {
			CollisionManager::GetInstance().Add(enemy, *hero);/*!< モンスターの勇者に対するアクション */
			CollisionManager::GetInstance().Add(*hero, enemy);/*!< 勇者のモンスターに対するアクション */
			return true;
		}
	}
	return false;
}

/*
	@brief	モンスター同士の衝突情報登録
*/
void EnemyManager::CollisionEtoE()
{
	/*! リストが空なら処理しない */
	if (m_HitList.empty()) { return; }

	/*! リスト登録数分回す */
	for (unsigned int i = 0; i < m_HitList.size(); i++) {
		/*! 正面判定 */
		auto dir = Game::ConvertIndex(m_HitList[i]->GetDirection());
		INT2 pos = m_HitList[i]->GetIndex() + dir;
		/*! 外部参照 */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }
		/*! 隣接キャラの抽出 */
		Enemy* opponent = LocationManager::GetInstance().GetTopFront(pos);
		/*! 隣接するキャラが存在すればCollisionListへ登録 */
		if (opponent) {
			CollisionManager::GetInstance().Add(*m_HitList[i], *opponent);
			CollisionManager::GetInstance().Add(*opponent, *m_HitList[i]);
			break;
		}
	}
}

/*
	@brief	勇者との衝突情報登録
*/
void EnemyManager::CollisionEtoH()
{
	auto p = m_pEnemyList->m_pHead->pNext;
	
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		Hit(*p->data);
		p = p->pNext;
	}
}

/*
	@brief	レンダリング
*/
void EnemyManager::Render()
{
	ENEMY_NODE* p;
	p = m_pEnemyList->m_pHead->pNext;
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		IDirect3DTexture9* texture = TextureGetter(p->data->GetType());
		p->data->Render(texture);
		p = p->pNext;
	}
}

/*
	@brief	解放
*/
void EnemyManager::Destroy()
{
	SAFE_DELETE(m_pEnemyList);
	SAFE_DELETE(m_MossTexture);
	SAFE_DELETE(m_BudsTexture);
	SAFE_DELETE(m_FlowerTexture);
	SAFE_DELETE(m_LarvaeTexture);
	SAFE_DELETE(m_ChrysalisTexture);
	SAFE_DELETE(m_AdultTexture);
	SAFE_DELETE(m_EggTexture);
	SAFE_DELETE(m_LizardTexture);
}

/*
	@brief	養分・魔分に応じたモンスターを生成 ※現状魔分は未実装のため養分モンスターを生成
*/
void EnemyManager::Produce(INT2 pos, INT2 component)
{
	/******************/
	/* モンスター出現 */
	/******************/

	/*! 養分・魔分の比較 */
	auto val = component.x > component.y ? component.x : component.y;
	Enemy* enemy;
	/*! レベル３ */
	if (val > 16) {
		enemy = component.x > component.y ? new Lizard : new Lizard;
	}
	/*! レベル２ */
	else if (val > 9) {
		enemy = component.x > component.y ? new Larvae : new Larvae;
	}
	/*! レベル１ */
	else {
		enemy = component.x > component.y ? new Moss : new Moss;
	}
	/*! モンスターの初期化 */
	enemy->Init(pos, component);
	EnemyManager::GetInstance().AddEnemy(enemy);
}

/*
	@brief	モンスターの追加
*/
void EnemyManager::AddEnemy(Enemy * enemy)
{
	m_pEnemyList->AddNode(enemy);
}

/*
	@brief	衝突判定リストへの追加
*/
void EnemyManager::HitListAdd(Enemy * enemy)
{
	m_HitList.push_back(enemy);
}

/*
	@brief	衝突判定リストクリア
*/
void EnemyManager::HitListClear()
{
	m_HitList.clear();
	m_HitList.shrink_to_fit();
}

/*
	@brief	モンスターのテクスチャの読み込み
*/
void EnemyManager::LoadTexture()
{
	if (!m_MossTexture->Load(L"../Resource/Texture/moss.png"))	 MSG("読み込み失敗");
	if (!m_BudsTexture->Load(L"../Resource/Texture/蕾.png"))	 MSG("読み込み失敗");
	if (!m_FlowerTexture->Load(L"../Resource/Texture/花.png"))MSG("読み込み失敗");
	if (!m_LarvaeTexture->Load(L"../Resource/Texture/196.png"))MSG("読み込み失敗");
	if (!m_ChrysalisTexture->Load(L"../Resource/Texture/蛹.png"))MSG("読み込み失敗");
	if (!m_AdultTexture->Load(L"../Resource/Texture/196-2.png"))MSG("読み込み失敗");
	if (!m_EggTexture->Load(L"../Resource/Texture/egg.png"))	 MSG("読み込み失敗");
	if (!m_LizardTexture->Load(L"../Resource/Texture/153.png"))MSG("読み込み失敗");
}

/*
	@brief	モンスターに対応したテクスチャを返す
*/
IDirect3DTexture9 * EnemyManager::TextureGetter(Enemy::Type type)
{
	if (type == Enemy::Type::MOSS)return m_MossTexture->GetTexture();
	if (type == Enemy::Type::BUDS)return m_BudsTexture->GetTexture();
	if (type == Enemy::Type::FLOWER)return m_FlowerTexture->GetTexture();
	if (type == Enemy::Type::LARVAE)return m_LarvaeTexture->GetTexture();
	if (type == Enemy::Type::CHRYSALIS)return m_ChrysalisTexture->GetTexture();
	if (type == Enemy::Type::ADULT)return m_AdultTexture->GetTexture();
	if (type == Enemy::Type::EGG)return m_EggTexture->GetTexture();
	if (type == Enemy::Type::LIZARD)return m_LizardTexture->GetTexture();
	return nullptr;
}
