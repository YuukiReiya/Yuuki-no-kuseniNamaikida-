/*
	@file	Terrain.h
	@brief	�n�`
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Texture/Texture.h"
#include "../../../Sprite/Sprite.h"
#include "../../Soil/Soil.h"

//�n�`�������N���X
/*
	@brief	�n�`�N���X
*/
class Terrain
{
public:
	/*! �R���X�g���N�^ */
	Terrain();
	/*! �f�X�g���N�^ */
	~Terrain();

	void Init();									
	void Destroy();
	Soil* m_pSoils[SOIL_WIDTH][SOIL_HEIGHT];	/*!< �y�u���b�N */
private:

};
