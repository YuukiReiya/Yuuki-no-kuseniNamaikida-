/*
	@file	Character.h
	@brief	�L�����N�^�[
	@author	�ԏ� �G�P
*/
#pragma once
#include"../../Common/Game.h"

/*! �O���錾 */
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
	@brief	�L�����N�^�[�̊��N���X
*/
class Character
{
public:
	/*! �R���X�g���N�^ */
	Character()				= default;
	/*! �f�X�g���N�^ */
	virtual ~Character()	= default;

	virtual void Update()	= 0;	/*!< �X�V */
	virtual void Destroy()	= 0;	/*!< ��� */

	/*! �Փ� */
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
