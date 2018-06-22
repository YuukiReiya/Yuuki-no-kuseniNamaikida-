#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Enemy/Enemy.h"
#include <vector>

/*
	@struct	ENEMY_ARRAY
	@brief	モンスターの動的配列構造体
*/
struct ENEMY_ARRAY
{
	std::vector<Enemy*>enemy;
};

/*
	@brief	モンスターの居場所クラス
*/
class Location
{
public:
	/*! コンストラクタ */
	Location();
	/*! デストラクタ */
	~Location();

	void Init();
	void Destroy();
	void Clear();

	ENEMY_ARRAY m_EnemyLocation[SOIL_WIDTH][SOIL_HEIGHT];
private:
};