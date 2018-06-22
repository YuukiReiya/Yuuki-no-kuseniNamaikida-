/*
	@file	Scene.h
	@brief	各シーンのスーパークラス
	@date	2017/12/04
	@author	番場 宥輝
*/
#pragma once
#include "SceneBase.h"

/*! 前方宣言 */
class SceneRoot;

/*
	@brief	各シーンの基底クラス
*/
class Scene :public SceneBase
{
public:
	/*! 仮想デストラクタ */
	virtual ~Scene() {};

	SceneBase* Update(SceneBase* scene);
	virtual SceneBase* Update(SceneRoot* root)=0;
};