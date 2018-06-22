/*
	@file	SceneBase.h
	@brief	シーン管理の基底クラス
	@date	2017/12/04
	@author	番場 宥輝
*/
#pragma once

/*
	@brief	シーンの基底クラス
*/
class SceneBase
{
public:
	/*! 仮想デストラクタ */
	virtual ~SceneBase() {};

	/*! 純粋仮想関数 */
	virtual void Init() = 0;						/*!< 初期化 */
	virtual SceneBase* Update(SceneBase* scene) = 0;/*!< 更新 */
	virtual void Render() = 0;						/*!< レンダリング */
	virtual void Destroy() = 0;						/*!< 解放 */
};
