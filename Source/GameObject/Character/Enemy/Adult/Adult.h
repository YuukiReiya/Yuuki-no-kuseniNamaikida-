/*
	@file	Adult.h
	@brief	成虫
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Moss/Moss.h"
#include "../Buds/Buds.h"
#include "../Flower/Flower.h"

/*
	@brief	成虫
*/
class Adult :
	public Enemy
{
public:
	/*! コンストラクタ */
	Adult();
	/*! デストラクタ */
	~Adult();

	/*! 関数 */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Moss& moss);
	void Collision(Buds& buds);
	void Collision(Flower& flower);
	void Collision(Lizard& lizard);
	void Collision(Soldier& soldier);
private:
	void Predator(Moss& moss);
	void Predator(Buds& buds);
	void Predator(Flower& flower);
	void Predator(Larvae& larvae);
	void IndexUpdate();
	void OutComponent();
	bool Move();
	bool Wait();
	void Attack();
	bool CheckBreeding();
	void Breeding();
	void StackInit();
	void StackPushDirection(D3DXVECTOR2 direction);
	int StackElementsNum();
	void AnimUpdate();


	/*! 定数 */
	static const unsigned int MAX_HP;					/*!< 最大HP */
	static const unsigned int BREEDING_HP;				/*!< 繁殖に必要なHP */
	static const unsigned int WAIT_COUNT;				/*!< 待機カウント */
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

	/*! 変数 */
	bool isMove;					/*!< 移動フラグ */
	D3DXVECTOR2 m_StackDirection[4];/*!< 進路候補 */
	int m_StackIndex;				/*!< スタックのインデックス */
	DWORD m_Count;					/*!< 行動カウンタ */


	Debug m_Debug;
};

