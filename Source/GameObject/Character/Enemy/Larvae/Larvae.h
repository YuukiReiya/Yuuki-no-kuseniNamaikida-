/*
	@file	Larvae.h
	@brief	幼虫
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Chrysalis/Chrysalis.h"
#include "../Moss/Moss.h"
#include "../Lizard/Lizard.h"
#include "../../../../Debug/Debug.h"

/*
	@brief	幼虫
*/
class Larvae :
	public Enemy
{
public:
	/*! コンストラクタ */
	Larvae();
	/*! デストラクタ */
	~Larvae();

	/*! 関数 */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Moss& moss);
	void Collision(Lizard& lizard);
	void Collision(Soldier& soldier);
private:
	void IndexUpdate();
	void OutComponent();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckGrow();
	void Grow();
	void Predator(Moss&moss);
	void StackInit();
	void StackPushDirection(D3DXVECTOR2 direction);
	void AnimUpdate();
	int StackElementsNum();

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

	/*! 定数 */
	static const unsigned int MAX_HP;					/*!< 最大HP */
	static const unsigned int POWER;					/*!< 攻撃力 */
	static const unsigned int WAIT_COUNT;				/*!< 待機カウント */
	static const unsigned int GROW_COMPONENT;			/*!< 成長に必要な養分 */
	static const unsigned int GROW_HP;					/*!< 成長に必要なHP */
	static const unsigned int STACK_SIZE;				/*!< スタックのサイズ */
	static const D3DXVECTOR2 INITIALIZE_STACK_ELEMENTS;	/* スタックの要素の初期値 */

	/*! 変数 */
	bool isMove;					/*!< 移動フラグ */
	D3DXVECTOR2 m_StackDirection[4];/*!< 進路候補 */
	int m_StackIndex;				/*!< スタックのインデックス */
	DWORD m_Count;					/*!< 攻撃を行うまでのカウンタ */

	Debug m_Debug;
};

