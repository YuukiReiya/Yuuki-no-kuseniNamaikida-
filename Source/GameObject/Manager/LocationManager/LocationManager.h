/*
	@file	LocationManager.h
	@brief	キャラの居場所を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#pragma once
#include "../../List/Location/Location.h"

/*
	@brief	キャラクターの居場所の管理クラス
*/
class LocationManager
{
public:
	/*! デストラクタ */
	~LocationManager();
	/*! シングルトン */
	static LocationManager&GetInstance() {
		static LocationManager instance;
		return instance;
	}

	void Init();
	void Destroy();
	bool DeleteData(INT2 pos, Enemy* enemy);

	/*! ゲッター */
	Enemy* GetTopFront(INT2 pos);
	Enemy* GetTopBack(INT2 pos);

	/*! セッター */
	void SetData(INT2 location, Enemy* enemy);
private:

	/*! コンストラクタ */
	LocationManager();


	/*! 変数 */
	Location* m_pLocation;


};

