/*
	@file	SceneRoot.h
	@brief	シーン遷移
	@date	2017/12/04
	@author	番場 宥輝
*/
#pragma once
#include <iostream>
#include "../SceneBase/Scene.h"
#include "SceneRoot.h"
#include "../Scene/SceneMain.h"
#include "../Scene/SceneTitle.h"

/*
	@brief	シーン遷移の管理クラス
*/
class SceneRoot :public SceneBase
{
private:
	Scene* m_pScene;
public:
	/* コンストラクタ */
	SceneRoot();
	/*! デストラクタ */
	~SceneRoot();

	void Init();
	SceneBase* Update(SceneBase* scene);
	void Render();
	void Destroy();
};
