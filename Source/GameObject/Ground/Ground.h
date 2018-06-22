#pragma once
#include "../../Common/Common.h"
#include "../../Sprite/Sprite.h"
#include "../../Texture/Texture.h"

/*
	@brief	�n��N���X
*/
class Ground
{
public:
	/*! �R���X�g���N�^ */
	Ground();
	/*! �f�X�g���N�^ */
	~Ground();

	void Init();
	void Render();
	void Destroy();
private:
	Sprite* m_pSprite;	/*!< �X�v���C�g */
	Texture* m_pTexture;/*!< �e�N�X�`�� */
};

