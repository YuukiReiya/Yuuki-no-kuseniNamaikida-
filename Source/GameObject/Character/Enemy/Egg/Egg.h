/*
	@file	Egg.h
	@brief	卵
	@author	番場 宥輝
*/
#pragma once
#include "../Enemy.h"
#include "../Lizard/Lizard.h"

/*
	@brief	卵クラス
*/
class Egg :
	public Enemy
{
public:
	/*! コンストラクタ */
	Egg();
	/*! デストラクタ */
	~Egg();

	void Init(INT2 pos, INT2 component);
	void Destroy();
	void Update();
	void Render(IDirect3DTexture9* texture);
	void Damage(int damage);
	void Collision(Character& character);

private:
	void Hatch();

	/*! 定数 */
	static const unsigned int HATCH_COUNT;				/*!< 孵化に必要な時間 */
	
	/*! 変数 */
	DWORD m_Count;
	Debug m_Debug;
};

