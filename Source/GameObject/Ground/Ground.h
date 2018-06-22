#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	地上クラス
*/
class Ground
{
public:
	/*! コンストラクタ */
	Ground();
	/*! デストラクタ */
	~Ground();

	void Init();
	void Render();
	void Destroy();
private:
	Sprite* m_pSprite;	/*!< スプライト */
	Texture* m_pTexture;/*!< テクスチャ */
};

