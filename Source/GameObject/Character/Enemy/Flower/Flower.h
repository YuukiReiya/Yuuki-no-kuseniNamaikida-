/*
	@file	Flower.h	
	@brief	花
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Moss/Moss.h"

/*
	花クラス
*/
class Flower :
	public Enemy
{
public:
	/*! コンストラクタ */
	Flower();
	/*! デストラクタ */
	~Flower();

	/*! 関数 */
	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Adult& adult);
private:
	bool Absorption();
	void Breeding();
	void OutComponent();

	/*! 定数 */
	static const unsigned int ACTION_COUNT;				/*!< 行動カウント */
	static const unsigned int ABSORPTION_POWER;			/*!< 養分吸収量 */
	static const unsigned int ABSORPTION_OVER_LIMIT;	/*!< 養分吸収可能上限 */
	static const unsigned int ABSORPTION_UNDER_LIMIT;	/*!< 養分吸収可能下限 */
	static const unsigned int BREEDING_COMPONENT;		/*!< 繁殖に必要な養分 */
	static const unsigned int BREEDING_COUNT;			/*!< 繁殖に必要な吸収回数 */
	static const unsigned int MAX_HP;					/*!< HP最大値 */


	/*! 変数 */
	DWORD m_Count;						/*!< 行動カウンタ */
	int m_AbsorptionCount;				/*!< 吸収回数 */
	Debug m_Debug;
};

