#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Enemy/Enemy.h"
#include <vector>

/*
	@struct	ENEMY_ARRAY
	@brief	�����X�^�[�̓��I�z��\����
*/
struct ENEMY_ARRAY
{
	std::vector<Enemy*>enemy;
};

/*
	@brief	�����X�^�[�̋��ꏊ�N���X
*/
class Location
{
public:
	/*! �R���X�g���N�^ */
	Location();
	/*! �f�X�g���N�^ */
	~Location();

	void Init();
	void Destroy();
	void Clear();

	ENEMY_ARRAY m_EnemyLocation[SOIL_WIDTH][SOIL_HEIGHT];
private:
};