/*
	@file	Scene.h
	@brief	�e�V�[���̃X�[�p�[�N���X
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#pragma once
#include "SceneBase.h"

/*! �O���錾 */
class SceneRoot;

/*
	@brief	�e�V�[���̊��N���X
*/
class Scene :public SceneBase
{
public:
	/*! ���z�f�X�g���N�^ */
	virtual ~Scene() {};

	SceneBase* Update(SceneBase* scene);
	virtual SceneBase* Update(SceneRoot* root)=0;
};