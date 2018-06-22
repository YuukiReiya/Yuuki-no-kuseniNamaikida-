/*
	@file	SceneRoot.h
	@brief	�V�[���J��
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#pragma once
#include <iostream>
#include "../SceneBase/Scene.h"
#include "SceneRoot.h"
#include "../Scene/SceneMain.h"
#include "../Scene/SceneTitle.h"

/*
	@brief	�V�[���J�ڂ̊Ǘ��N���X
*/
class SceneRoot :public SceneBase
{
private:
	Scene* m_pScene;
public:
	/* �R���X�g���N�^ */
	SceneRoot();
	/*! �f�X�g���N�^ */
	~SceneRoot();

	void Init();
	SceneBase* Update(SceneBase* scene);
	void Render();
	void Destroy();
};
