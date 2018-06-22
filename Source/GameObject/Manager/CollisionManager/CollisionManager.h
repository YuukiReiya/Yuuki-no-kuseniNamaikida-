/*
	@file	CollisionManager.h
	@brief	オブジェクトの衝突を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Character.h"
#include <vector>

/*
	@struct	CollisionInfo
	@brief	衝突情報を格納する構造体
	@detail	衝突する側と衝突先の情報を保持する
*/
struct CollisionInfo
{
	Character* A;	/*!< 衝突側 */
	Character* B;	/*!< 衝突先 */
};

/*
	@brief	衝突管理クラス
*/
class CollisionManager
{
public:
	/*! コンストラクタ */
	CollisionManager();
	/*! デストラクタ */
	~CollisionManager();

	/*! シングルトン */
	static CollisionManager& GetInstance() {
		static CollisionManager instance;
		return instance;
	}

	void Update();
	void Init();
	void Add(Character& A, Character& B);
	void Clear();
private:
	std::vector<CollisionInfo> m_List;
};

