/*
	@file	Scene.cpp
	@brief	各シーンのスーパークラス
	@date	2017/12/04
	@author	番場 宥輝
*/
#include "Scene.h"
#include "../SceneRoot/SceneRoot.h"

/*
	@brief	シーンの更新処理
*/
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}