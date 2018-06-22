/*
	@file	HeroManager.h
	@brief	�E�҂��Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../List/HeroList/HeroList.h"
#include "../../../Texture/Texture.h"
#include "../TycoonManager/TycoonManager.h"
#include "../../../Debug/Debug.h"

/*
	@brief	�E�҂̊Ǘ��N���X
*/
class HeroManager
{
public:
	/*! �R���X�g���N�^ */
	HeroManager();
	/*! �f�X�g���N�^ */
	~HeroManager();

	/*! �V���O���g�� */
	static HeroManager& GetInstance() {
		static HeroManager instance;
		return instance;
	}

	void Init();
	void Update();
	void Render();
	void Destroy();
	void AddHero(Hero* hero);
	Hero* GetHero(INT2 pos);
	Hero* GetHeroInfo();
	bool GetClearFlag() { return isClear; }
private:
	void TextureLoad();
	bool isClear;
	HeroList* m_pHeroList;
	Texture* m_pHeroTexture;
	Debug m_Debug;
};