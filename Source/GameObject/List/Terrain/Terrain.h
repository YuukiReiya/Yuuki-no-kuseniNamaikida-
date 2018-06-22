/*
	@file	Terrain.h
	@brief	地形
	@author	番場 宥輝
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Texture/Texture.h"
#include "../../../Sprite/Sprite.h"
#include "../../Soil/Soil.h"

//地形情報を持つクラス
/*
	@brief	地形クラス
*/
class Terrain
{
public:
	/*! コンストラクタ */
	Terrain();
	/*! デストラクタ */
	~Terrain();

	void Init();									
	void Destroy();
	Soil* m_pSoils[SOIL_WIDTH][SOIL_HEIGHT];	/*!< 土ブロック */
private:

};
