/*
	@file	Player.h
	@brief	�v���C���[
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Common/Common.h"
#include "../../GameObject/Manager/TerrainManager/TerrainManager.h"
#include "../../GameObject/Manager/EnemyManager/EnemyManager.h"
#include "../Manager/TycoonManager/TycoonManager.h"
#include "../../GameObject/Character/Enemy/Moss/Moss.h"
#include "../../GameObject/Character/Enemy/Bugs/Bugs.h"
#include "../../GameObject/Character/Enemy/Lizard/Lizard.h"
#include "../Pickel/Pickel.h"
#include "../../Debug/Debug.h"

/*
	@brief	�v���C���[�N���X
*/
class Player
{
public:
	/*! �R���X�g���N�^ */
	Player();
	/*! �f�X�g���N�^ */
	~Player();

	void Init();
	void Destroy();
	void Update();
	void Render();
	void Move();
	void Action();
	bool SetTycoon();
	void SetDigPower(int power);
	void DrawDigPower();
private:
	void Break(INT2 pos);
	void Attack(INT2 pos);
	bool ContactBleakedBlock(INT2 pos);
	
	/*! �萔 */
	static const unsigned int ATK;

	/*! �ϐ� */
	D3DXVECTOR2 m_Pos;
	INT2		m_IndexPos;
	Pickel		m_Pickel;
	Debug		m_Debug;
	UINT		m_DigPower;/*!< �x�p���[ */
};

