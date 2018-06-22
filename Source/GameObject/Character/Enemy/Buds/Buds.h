/*
	@file	Buds.h
	@brief	蕾
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Flower/Flower.h"

/*
	@brief	蕾クラス
*/
class Buds :
	public Enemy
{
public:
	/*! コンストラクタ */
	Buds();
	/*! デストラクタ */
	~Buds();

	/*! 関数 */
	void Init(INT2 pos, INT2 component);
	void Init(INT2 pos, INT2 component, int hp);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Adult& adult);
private:
	bool Absorption();
	void Grow();
	void OutComponent();

	/*! 定数 */
	static const unsigned int ACTION_COUNT;				/*!< 行動カウント */
	static const unsigned int ABSORPTION_OVER_LIMIT;	/*!< 養分吸収可能上限 */
	static const unsigned int ABSORPTION_UNDER_LIMIT;	/*!< 養分吸収可能下限 */
	static const unsigned int GROW_COMPONENT;			/*!< 成長に必要な養分 */

	/*! 変数 */
	DWORD m_Count;
	Debug m_Debug;
};

