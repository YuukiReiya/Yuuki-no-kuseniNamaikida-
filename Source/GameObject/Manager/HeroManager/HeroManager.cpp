/*
	@file	HeroManager.cpp
	@brief	勇者を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "HeroManager.h"

/*! 名前空間 */
using namespace Game;

/*
	@brief	コンストラクタ
*/
HeroManager::HeroManager()
{
}

/*
	@brief	デストラクタ
*/
HeroManager::~HeroManager()
{
	Destroy();
}

/*
	@brief	初期化
*/
void HeroManager::Init()
{
	m_pHeroTexture = new Texture;
	m_pHeroList = new HeroList;

	m_pHeroList->Init();
	TextureLoad();
	
	isClear = false;
	m_Debug.Init();
}

/*
	@brief	更新
*/
void HeroManager::Update()
{
	
	HERO_NODE* p;
	p = m_pHeroList->m_pHead->pNext;
	
	/*! 勇者の周囲 */
	INT2 contact[4] = {
		{ 0,-1 },
		{ 0,1 },
		{ 1,0 },
		{ -1,0 }
	};

	/*! 一人ずつ更新 */
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		
		/*! インデックスの更新 */
		if (TheSamePositionToGrid(p->data->GetPos())) {
			p->data->IndexUpdate();
			if (p->data->GetCaught())
				TycoonManager::GetInstance().SetHeroPos(p->data->GetIndex());
		}

		/*! HPが0ならリストから外す */
		if (p->data->GetHp() <= 0) {
			/*
				死亡時
			*/
			/*! 捕縛状態解除 */
			isClear = true;
			if (p->data->GetCaught()) {
				p->data->SetCaught(false);
				TycoonManager::GetInstance().SetCaughtFlags(false);
			}

			auto del = p;
			p = p->pNext;
			m_pHeroList->DeleteNode(del);
			continue;
		}

		/*! 魔王捕縛判定 */
		if (!TycoonManager::GetInstance().GetCaughtFlags()) {
			/*! 勇者の周囲のマスを判定 */
			for (auto i : contact) {
				INT2 tycoonPos = TycoonManager::GetInstance().GetTycoonPos() + i;
				/*! 魔王が隣接マスにいるか判定 */
				if (tycoonPos == p->data->GetIndex()) {
					p->data->SetCaught(true);
					TycoonManager::GetInstance().SetCaughtFlags(true);
					TycoonManager::GetInstance().SetHeroData(p->data->GetMovePow());
					TycoonManager::GetInstance().SetHeroPos(p->data->GetIndex());
					p->data->Catch();
					break;
				}
			}
		}

		/*! 更新 */
		p->data->Update();
		p = p->pNext;
	}


}

/*
	@brief	レンダリング
*/
void HeroManager::Render()
{
	HERO_NODE* p;
	p = m_pHeroList->m_pHead->pNext;
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		p->data->Render(m_pHeroTexture->GetTexture());
		p = p->pNext;
	}
	/*
	char str[255];
	sprintf(str, "Tycoon:ix=%d,iy=%d\npx=%f,py=%f",
		TycoonManager::GetInstance().GetTycoonPos().x,
		TycoonManager::GetInstance().GetTycoonPos().y,
		TycoonManager::GetInstance().GetTycoonPosV().x,
		TycoonManager::GetInstance().GetTycoonPosV().y);
	*/
	/*
	*/
	/*
	sprintf(str, "Tycoon:ix=%d,iy=%d\nNx=%d,Ny=%d,座標=%f/%f",
		TycoonManager::GetInstance().GetTycoonPos().x,
		TycoonManager::GetInstance().GetTycoonPos().y,
		TycoonManager::GetInstance().GetTycoonNextPos().x,
		TycoonManager::GetInstance().GetTycoonNextPos().y,
		TycoonManager::GetInstance().GetTycoonPosV().x,
		TycoonManager::GetInstance().GetTycoonPosV().y
	);
	//sprintf(str,"")
	SetWindowTextA(m_Debug.m_hWnd, str);
	*/
}

/*
	@brief	解放
*/
void HeroManager::Destroy()
{
	SAFE_DELETE(m_pHeroTexture);
	SAFE_DELETE(m_pHeroList);
}

/*
	@brief	勇者の追加
*/
void HeroManager::AddHero(Hero * hero)
{
	m_pHeroList->AddNode(hero);
	hero->Init();
}

/*
	@brief	勇者のゲッター
	@param(pos)	座標
	@detail	指定座標の勇者の情報を返す。勇者がいなければNULLを返す
*/
Hero * HeroManager::GetHero(INT2 pos)
{
	auto p = m_pHeroList->m_pHead->pNext;
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		//Hpが1以上かつ指定インデックス座標の勇者
		if (p->data->GetHp() > 0 && p->data->GetIndex() == pos) {
			return p->data;
		}
		p = p->pNext;
	}
	return NULL;
}

/*
	@brief	勇者の取得
*/
Hero * HeroManager::GetHeroInfo()
{
	auto p = m_pHeroList->m_pHead->pNext;
	if (p != m_pHeroList->m_pTail&&p != NULL) { return p->data; }
	return NULL;
}

/*
	@brief	勇者のテクスチャの読み込み
*/
void HeroManager::TextureLoad()
{
	if (!m_pHeroTexture->Load(L"../Resource/Texture/71.png"))MSG("読み込み失敗");

}
