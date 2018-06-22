/*
	@file	EnemyManager.h
	@brief	出現モンスターを管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../List/EnemyList/EnemyList.h"
#include "../CollisionManager/CollisionManager.h"
#include "../LocationManager/LocationManager.h"
#include "../../List/Location/Location.h"
#include "../HeroManager/HeroManager.h"

/*
	@brief	モンスターの管理クラス
*/
class EnemyManager
{
public:
	/*! デストラクタ */
	~EnemyManager();

	/*! シングルトン */
	static EnemyManager& GetInstance() {
		static EnemyManager instance;
		return instance;
	}

	void Init();
	void Update();
	void Render();
	void Destroy();
	void Produce(INT2 pos, INT2 component);
	void AddEnemy(Enemy* enemy);
	void HitListAdd(Enemy* enemy);
	void HitListClear();
private:
	bool Hit(Enemy& A, Enemy& B);
	bool Hit(Enemy& enemy);
	void CollisionEtoE();
	void CollisionEtoH();
	void LoadTexture();
	IDirect3DTexture9* TextureGetter(Enemy::Type type);


	/*! コンストラクタ */
	EnemyManager();

	/*! 変数 */
	Location* m_pLocation;				/*! 位置データ */
	std::vector<Enemy*> m_HitList;		/*!< モンスター衝突判定リスト */
	EnemyList* m_pEnemyList;			/*!< モンスターのリスト */
	Texture* m_MossTexture;				/*!< コケのテクスチャ */
	Texture* m_BudsTexture;				/*!< 蕾のテクスチャ */
	Texture* m_FlowerTexture;			/*!< 花のテクスチャ */
	Texture* m_LarvaeTexture;			/*!< 幼虫のテクスチャ */
	Texture* m_ChrysalisTexture;		/*!< 蛹のテクスチャ */
	Texture* m_AdultTexture;			/*!< 成虫のテクスチャ */
	Texture* m_EggTexture;				/*!< 卵のテクスチャ */
	Texture* m_LizardTexture;			/*!< トカゲのテクスチャ */
};

