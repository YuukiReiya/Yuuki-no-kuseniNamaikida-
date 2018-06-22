/*
	@file	Soldier.h
	@brief	剣士
	@author	番場 宥輝
*/
#pragma once
#include "../Hero.h"
#include "../../../../Algorithm/AStar/AStar.h"
#include "../../Enemy/Enemy.h"
#include "../../Enemy/Moss/Moss.h"
#include "../../Enemy/Bugs/Bugs.h"
#include "../../Enemy/Lizard/Lizard.h"

/*
	@brief	剣士クラス
*/
class Soldier :
	public Hero
{
public:
	/*!コンストラクタ */
	Soldier();
	/*!デストラクタ */
	~Soldier();

	void Init();
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void IndexUpdate();
	void Catch();
	void Damage(int damage);

	/*! 衝突 */
	void Collision(Character& character);
	void Collision(Moss& moss)				{ Battle(moss); }
	void Collision(Buds& buds)				{ Battle(buds); }
	void Collision(Flower& flower)			{ Battle(flower); }
	void Collision(Larvae& larvae)			{ Battle(larvae); }
	void Collision(Chrysalis& chrysalis)	{ Battle(chrysalis); }
	void Collision(Adult& adult)			{ Battle(adult); }
	void Collision(Egg& egg)				{ Battle(egg); }
	void Collision(Lizard& lizard)			{ Battle(lizard); }
private: 
	void Battle(Enemy& enemy);
	void AnimUpdate();
	bool Move();
	bool Wait();

	/*! 定数 */
	static const unsigned int WAIT_COUNT;

	/*
		@enum	STATE
		@brief	状態を表す
	*/
	enum STATE {
		ASSIGN,	/*!< 割り当て */
		MOVE,	/*!< 移動中 */
		ATK,	/*!< 攻撃中 */
		WAIT,	/*!< 待機中 */
		CATCH,	/*!< 捕縛 */
	}state;


	/*! 変数 */
	AStar aStar;			/*!< A* */
	Enemy* m_pEnemy;		/*!< 攻撃対象モンスター */
	INT2 m_TargetIndexPos;	/*!< 移動目標地点 */
	DWORD m_Count;			/*!< 行動カウンタ */
	Debug m_Debug;			/*!< デバッグ */
};
