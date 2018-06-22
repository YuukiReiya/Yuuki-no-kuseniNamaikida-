/*
	@file	InputManager.h
	@brief	入力系統を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once

#define input InputManager::GetInstance()

/*
	@brief	入力管理クラス
*/
class InputManager
{
public:
	/*! デストラクタ */
	~InputManager();

	/*! シングルトン */
	static InputManager& GetInstance(){
		static InputManager instance;
		return instance;
	}


	/*
		@enum	Input
	*/
	enum Key{
		DECIDE,	/*!< 決定 */
		CANCEL,	/*!< キャンセル */
		DIG,	/*!< 掘る */
		SEARCH,	/*!< 調べる */
		LEFT,	/*!< 左 */
		RIGHT,	/*!< 右 */
		UP,		/*!< 上 */
		DOWN,	/*!< 下 */
	};

	/*! 関数 */
	void Update();

	/*! 入力 */
	bool GetKey(Key key);
	bool GetKeyDown(Key key);
	bool GetKeyUp(Key key);
private:
	/*! コンストラクタ */
	InputManager();
};

