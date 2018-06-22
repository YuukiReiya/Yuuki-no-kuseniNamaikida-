/*
	@file	Moss.h
	@brief	コケ
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"
#include "../../../Manager/HeroManager/HeroManager.h"
#include "../Buds/Buds.h"
#include "../Larvae/Larvae.h"
#include "../Adult/Adult.h"

#include "../../../../Debug/Debug.h"

/*
	@brief	コケクラス
*/	
class Moss :
	public Enemy
{
public:
	/*! コンストラクタ */
	Moss();
	/*! デストラクタ */
	~Moss();

	void Init(INT2 pos,INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Larvae& larvae);
	void Collision(Adult& adult);
	void Collision(Soldier& solder);
private:
	bool Wait();
	void Attack();
	void IndexUpdate();
	void AnimUpdate();
	bool Move();
	bool CheckAbsorption();
	void Absorption();
	bool CheckDivergence();
	void Divergence();	
	void OutComponent();
	bool CheckGrow();
	void Grow();
	D3DXVECTOR2 CalculateDirection();

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
	static const unsigned int GROW_COMPONENT;			/*!< 成長に必要な養分 */
	static const unsigned int GROW_HP;					/*!< 成長に必要なHP */
	static const unsigned int RECOVERY_POWER;			/*!< 回復量 */
	static const unsigned int MAX_HP;					/*!< 最大HP */
	static const unsigned int WAIT_COUNT;				/*!< 待機カウント */

	/*! 変数 */
	bool isMove;
	bool isAgain;
	bool isAbsorp;/*!< 吸収フラグ */
	DWORD m_Count;/*!< 行動カウンタ */
	int m_AbsorptionPow;/*!< 養分吸収量 */

	Debug m_Debug;
};

