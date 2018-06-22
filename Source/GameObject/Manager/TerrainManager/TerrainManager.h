/*
	@file	TerrainManager.h
	@brief	地形を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Texture/Texture.h"
#include "../../List/Terrain/Terrain.h"
#include "../../ImmortalObject/ImmortalBlock/ImmortalBlock.h"
#include "../../Ground/Ground.h"
#include "../../../Debug/Debug.h"


/*
	@brief	地形の管理クラス
*/
class TerrainManager
{
public:
	/*! コンストラクタ */
	TerrainManager();
	/*! コンストラクタ */
	~TerrainManager();

	/*! シングルトン */
	static TerrainManager& GetInstance() {
		static TerrainManager instance;
		return instance;
	}
	
	void Init();				/*!< 初期化 */
	void Update();				/*!< 更新 */
	void Render();				/*!< 描画 */
	bool Break(INT2 Pos);		/*!< 破壊 */
	void Destroy();				/*!< 解放 */

	/*! ゲッター */
	bool GetBreakFlag(INT2 Index) { return m_pTerrain->m_pSoils[Index.x][Index.y]->m_isBreaked; }
	INT2 GetComponent(INT2 index) { return m_pTerrain->m_pSoils[index.x][index.y]->m_Component; }

	/*! セッター */
	void SetComponent(INT2 index, INT2 component) { m_pTerrain->m_pSoils[index.x][index.y]->m_Component = component; }
private:
	IDirect3DTexture9*	TextureGetter(INT2 Component);
	bool TextureLoad();

	Terrain*			m_pTerrain;
	ImmortalBlock*		m_pImmortalBlock;
	Ground*				m_pGround;

	/*! テクスチャ */
	Texture*			m_pZeroSoilTexture;			/*!< 養分・魔分がゼロ */
	Texture*			m_pLevel1NatTexture;		/*!< 養分レベル１ */
	Texture*			m_pLevel1MagTexture;		/*!< 魔分レベル１ */
	Texture*			m_pLevel2NatTexture;		/*!< 養分レベル２ */
	Texture*			m_pLevel2MagTexture;		/*!< 魔分レベル２ */
	Texture*			m_pLevel3NatTexture;		/*!< 養分レベル３ */
	Texture*			m_pLevel3MagTexture;		/*!< 魔分レベル３ */
	Texture*			m_pImmortalBlockTexture;	/*!< 破壊不能ブロック */
	Texture*			m_pBreakedSoilTexture;		/*!< ただの土 */

	Debug d;
};

