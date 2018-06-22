/*
	@file	TerrainManager.cpp
	@brief	地形を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "TerrainManager.h"
#include "../../../Camera/Camera2D.h"

/*
	@brief	コンストラクタ
*/
TerrainManager::TerrainManager()
{
}

/*
	@brief	デストラクタ
*/
TerrainManager::~TerrainManager()
{
	Destroy();
}

/*
	@brief	初期化
*/
void TerrainManager::Init()
{
	/*! インスタンス生成 */
	m_pTerrain				=	 new Terrain;
	m_pImmortalBlock		=	 new ImmortalBlock;
	m_pGround				=	 new Ground;
	m_pZeroSoilTexture		=	 new Texture;
	m_pImmortalBlockTexture =	 new Texture;
	m_pBreakedSoilTexture	=	 new Texture;
	m_pLevel1NatTexture		=	 new Texture;
	m_pLevel1MagTexture		=	 new Texture;
	m_pLevel2NatTexture		=	 new Texture;
	m_pLevel2MagTexture		=	 new Texture;
	m_pLevel3NatTexture		=	 new Texture;
	m_pLevel3MagTexture		=	 new Texture;

	if (!TextureLoad()) {
		MSG("地形のテクスチャのロードに失敗");
		return;
	}
	m_pTerrain->Init();
	m_pImmortalBlock->Init();
	m_pGround->Init();

	if (!d.Init()) { MSG("デバッグクラスの初期化に失敗"); }
}

/*
	@brief	更新
*/
void TerrainManager::Update()
{
	for(int x=0;x<SOIL_WIDTH;x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			/*! ※拡大縮小の関係上座標は左上基準 */
			D3DXVECTOR2 Pos{
				(float)GRID_SIZE 					+ (float)GRID_SIZE*x,
				(float)GRID_SIZE*GROUND_HEIGHT		+ (float)GRID_SIZE*y
			};

			m_pTerrain->m_pSoils[x][y]->m_Pos = (ConvertView(Pos));
		}
}

/*
	@brief	レンダリング
*/
void TerrainManager::Render()
{
	/*! 地形 */
	for (int x = 0; x<SOIL_WIDTH; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			if (!m_pTerrain->m_pSoils[x][y]->m_isBreaked) {
				IDirect3DTexture9* texture = TextureGetter(m_pTerrain->m_pSoils[x][y]->m_Component);
				m_pTerrain->m_pSoils[x][y]->Render(texture);
			}
			else {
				m_pTerrain->m_pSoils[x][y]->Render(m_pBreakedSoilTexture->GetTexture());
			}

		}
	/*! 壊せないブロック */
	m_pImmortalBlock->Render(m_pImmortalBlockTexture->GetTexture());

	/*! 地上 */
	m_pGround->Render();
}

/*
	@brief	ブロックの破壊
*/
bool TerrainManager::Break(INT2 Pos)
{
	/*! エラーチェック */
	if (Pos.x < 0 || Pos.y < 0)return false;
	if (SOIL_WIDTH <= Pos.x || SOIL_HEIGHT <= Pos.y)return false;

	/*! フラグが立ってなかったら */
	if (!m_pTerrain->m_pSoils[Pos.x][Pos.y]->m_isBreaked) {
		/*! フラグを立てる */
		m_pTerrain->m_pSoils[Pos.x][Pos.y]->m_isBreaked = true;
		return true;
	}
	return false;
}

/*
	@brief	解放
*/
void TerrainManager::Destroy()
{
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pZeroSoilTexture);
	SAFE_DELETE(m_pGround);
	SAFE_DELETE(m_pImmortalBlockTexture);
	SAFE_DELETE(m_pBreakedSoilTexture);
	SAFE_DELETE(m_pImmortalBlock);
	SAFE_DELETE(m_pLevel1NatTexture);
	SAFE_DELETE(m_pLevel1MagTexture);
	SAFE_DELETE(m_pLevel2NatTexture);
	SAFE_DELETE(m_pLevel2MagTexture);
	SAFE_DELETE(m_pLevel3NatTexture);
	SAFE_DELETE(m_pLevel3MagTexture);
}

/*
	@brief	養分・魔分に応じたブロックのテクスチャを返す
*/
IDirect3DTexture9 * TerrainManager::TextureGetter(INT2 Component)
{
	/* 養分と魔分の比較 */
	int temp = Component.x > Component.y ? Component.x : Component.y;
	if (temp >= COMPONENT_LEVEL3)return Component.x > Component.y ? m_pLevel3NatTexture->GetTexture() : m_pLevel3MagTexture->GetTexture();
	else if (temp >= COMPONENT_LEVEL2)return Component.x > Component.y ? m_pLevel2NatTexture->GetTexture() : m_pLevel2MagTexture->GetTexture();
	else if (temp >= COMPONENT_LEVEL1)return Component.x > Component.y ? m_pLevel1NatTexture->GetTexture() : m_pLevel1MagTexture->GetTexture();
	else if (temp == 0)return m_pZeroSoilTexture->GetTexture();
	MSG("地形管理クラスのテクスチャゲッターで予期しない値が混入 nullptrを返します");
	return nullptr;
}

/*
	@brief	土ブロックに使うテクスチャの読み込み
*/
bool TerrainManager::TextureLoad()
{
	MFALSE(m_pZeroSoilTexture->Load(L"../Resource/Texture/土ブロック.png"),"テクスチャの読み込み失敗")
	MFALSE(m_pLevel1NatTexture->Load(L"../Resource/Texture/養分1-1-3.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pLevel1MagTexture->Load(L"../Resource/Texture/土ブロック.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pLevel2NatTexture->Load(L"../Resource/Texture/養分2-1-9.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pLevel2MagTexture->Load(L"../Resource/Texture/土ブロック.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pLevel3NatTexture->Load(L"../Resource/Texture/養分3-2.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pLevel3MagTexture->Load(L"../Resource/Texture/土ブロック.png"), "テクスチャの読み込み失敗")
	//MFALSE(m_pZeroSoilTexture->Load(L"../Resource/Texture/養分1-1-2.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pImmortalBlockTexture->Load(L"../Resource/Texture/ImmortalObj.png"), "テクスチャの読み込み失敗")
	MFALSE(m_pBreakedSoilTexture->Load(L"../Resource/Texture/土.png"), "テクスチャの読み込み失敗")
	
	return true;
}
