/*
	@file	MAIN.h
	@brief	メイン
	@author	番場 宥輝
*/
#pragma once

#include "../Common/Common.h"
#include "../Scene/SceneRoot/SceneRoot.h"
#include <mmsystem.h>

/*
	@brief	メインクラス
*/
class MAIN
{
public:
	/*! コンストラクタ */
	MAIN();
	/*! デストラクタ */
	~MAIN();

	/*! メッセージプロシージャ */
	LRESULT MsgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT Init();	/*!< 初期化 */
	void Destroy();	/*!< 解放 */
	void Loop();	/*!< アプリケーション処理の入り口 */

	/*! ウィンドウの初期化 */
	HRESULT InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName);
	bool HighQualityTimmer();	/*!< 高解像度タイマーの対応有無 */

	/*! 変数 */
	HWND m_hWnd;
private:
	void App();		/*!< アプリケーション処理 */
	void Render();	/*! レンダリング <*/
	void Update();	/*!< 更新 */
	void InitFPS();	/*!< FPS初期化 */
	bool SleepApp();/*!< アプリケーション処理をお休み */


	SceneRoot* m_pRoot;/*!< シーン */

	/*! FPSを揃える */
	const float MIN_FRAME_TIME = 1.0f / FPS;
	float m_FrameTime;
	LARGE_INTEGER m_TimeStart;	
	LARGE_INTEGER m_TimeEnd;
	LARGE_INTEGER m_TimeFreq;
	float m_fps;

};

