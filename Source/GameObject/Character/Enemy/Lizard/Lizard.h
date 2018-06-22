/*
	@file	Lizard.h
	@brief	トカゲ
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Larvae/Larvae.h"
#include "../Chrysalis/Chrysalis.h"
#include "../Adult/Adult.h"
#include "../Egg/Egg.h"
#include "../../../../Algorithm/AStar/AStar.h"

/*
	@brief	トカゲクラス
*/
class Lizard :
	public Enemy
{
public:
	/*! コンストラクタ */
	Lizard();
	/*! デストラクタ */
	~Lizard();

	void Init(INT2 Pos,INT2 Component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Larvae& larvae);
	void Collision(Chrysalis& chrysalis);
	void Collision(Adult& adult);
	void Collision(Soldier& solder);
private:
	bool CheckLay();
	void Lay();
	void IndexUpdate();
	void AnimUpdate();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckMakeNest();
	void MakeNest();
	void OutComponent();
	D3DXVECTOR2 CalculateDirection();
	void StraightMoveIndexUpdate();
	void NestMoveIndexUpdate();
	void StackPushDirection(D3DXVECTOR2 direction);
	void StackInit();
	int StackElementsNum();

	/*! 定数 */
	static const unsigned int MAX_HP;					/*!< 最大HP */
	static const unsigned int WAIT_COUNT;				/*!< 攻撃までのカウント */
	static const unsigned int MAKE_NEST_COMPONENT;		/*!< 巣を作るための必要養分 */
	static const unsigned int MADE_NEST_HP;				/*!< 巣を作るためのHP */
	static const unsigned int LAY_EGG_COMPONENT;		/*!< 卵を産むための必要養分 */
	static const unsigned int LAY_EGG_HP;				/*!< 卵を産むためのHP */
	static const unsigned int STACK_SIZE;				/*!< スタックのサイズ */
	static const D3DXVECTOR2 INITIALIZE_STACK_ELEMENTS;	/* スタックの要素の初期値 */

	/*
		@enum	STATE
		@brief	状態を表す
	*/
	enum STATE {
		ASSIGN,	/*!< 割り当て */
		MOVE,	/*!< 移動中 */
		ATK,	/*!< 攻撃中 */
		WAIT,	/*!< 待機中 */
		ACTION,	/*!< アクション */
	}state;

	AStar m_aStar;
	bool isMove;
	bool isStraight;//直進フラグ
	bool isMadeNest;//巣を作ったかどうかのフラグ
	INT2 m_NestIndexPos;/*!< 巣の座標 */
	INT2 m_GoalIndexPos;/*!< 移動目標地点 */
	D3DXVECTOR2 m_StackDirection[3];
	int m_StackIndex;
	DWORD m_Count;
};

