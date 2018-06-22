/*
	@file	Chrysalis.h
	@brief	蛹
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Adult/Adult.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"

/*
	@brief	蛹
*/
class Chrysalis :
	public Enemy
{
public:
	/*! コンストラクタ */
	Chrysalis();
	/*! デストラクタ */
	~Chrysalis();

	/*! 関数 */
	void Init(INT2 pos, INT2 component);
	void Init(INT2 pos, INT2 component, int hp);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);
	void Collision(Lizard& lizard);
private:
	void Grow();
	void OutComponent();

	/*! 定数 */
	static const unsigned int ACTION_COUNT;				/*!< 行動カウント */
	static const unsigned int GROW_COUNT;				/*!< 成長に必要な時間 */


	/*! 変数 */
	DWORD m_Count;
	Debug m_Debug;

};

