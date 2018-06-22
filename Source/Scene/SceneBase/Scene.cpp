/*
	@file	Scene.cpp
	@brief	�e�V�[���̃X�[�p�[�N���X
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#include "Scene.h"
#include "../SceneRoot/SceneRoot.h"

/*
	@brief	�V�[���̍X�V����
*/
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}