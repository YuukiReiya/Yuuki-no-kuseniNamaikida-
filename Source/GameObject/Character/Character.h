/*
	@file	Character.h
	@brief	キャラクター
	@author	番場 宥輝
*/
#pragma once
#include"../../Common/Game.h"

/*! 前方宣言 */
class Moss;
class Buds;
class Flower;
class Larvae;
class Chrysalis;
class Adult;
class Egg;
class Lizard;
class Soldier;

/*
	@brief	キャラクターの基底クラス
*/
class Character
{
public:
	/*! コンストラクタ */
	Character()				= default;
	/*! デストラクタ */
	virtual ~Character()	= default;

	virtual void Update()	= 0;	/*!< 更新 */
	virtual void Destroy()	= 0;	/*!< 解放 */

	/*! 衝突 */
	virtual void Collision(Character& character) = 0;	
	virtual void Collision(Moss& moss) {};
	virtual void Collision(Buds& buds) {};
	virtual void Collision(Flower& flower) {};
	virtual void Collision(Larvae& larvae) {};
	virtual void Collision(Chrysalis& chrysalis) {};
	virtual void Collision(Adult& adult) {};
	virtual void Collision(Egg& egg) {};
	virtual void Collision(Lizard& lizard) {};
	virtual void Collision(Soldier& soldier) {};
};
