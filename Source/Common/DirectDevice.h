/*
	@file	DirectDevice.h
	@brief	DirectXデバイス
	@author	番場 宥輝
*/
#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dsound.h>
#include <tchar.h>


/*
	@brief	マクロ定義
*/
#define _CRT_SECURE_NO_WARNINGS
#define MSG(str) MessageBox(NULL,_T(str),NULL,MB_OK)
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[](p);(p)=NULL;}}
#define SAFE_RELEASE(p){if(p) {(p)->Release();(p)=NULL;}}

/*
	@brief	定数宣言
*/
const int WINDOW_WIDTH = 1920;		/*!< 画面横幅 */
const int WINDOW_HEIGHT = 1088;		/*!< 画面縦幅 */

/*
	@brief	デバイス管理クラス
*/
class DirectDevice
{
public:
	/*! デストラクタ */
	~DirectDevice();
	/*! シングルトン */
	static DirectDevice& GetInstance() {
		static DirectDevice instance;
		return instance;
	}

	/*! 
		@enum	RENDERSTATE
		@brief	レンダリングタイプの切り替え
	*/
	enum RENDERSTATE
	{
		RENDER_DEFAULT,		/*!< デフォルト(不透明) */ 
		RENDER_ALPHATEST	/*!< αテスト（透過）*/ 
	};


	bool Init(HWND hWnd);	/*!< デバイスの初期化 */
	void Release();			/*!< デバイスの解放	*/
	void SetRenderState(RENDERSTATE RenderState = RENDER_ALPHATEST);/*!< レンダーステートの変更 */

	/*! ゲッター */
	IDirect3DDevice9* GetDevice() { return m_pDevice3D; }		/*!< デバイス */
	IDirectSound8* GetSoundDevice() { return m_pSoundDevice; }	/*!< サウンドデバイス */
private:
	/*! コンストラクタ */
	DirectDevice();

	
	
	bool CreateDevice(HWND hWnd);		/*!< デバイスの生成 */
	bool CreateSoundDevice(HWND hWnd);	/*!< サウンドデバイスの生成 */

	/*! 変数 */
	IDirect3D9*			m_pD3D9;
	IDirect3DDevice9* m_pDevice3D;
	IDirectSound8*		m_pSoundDevice; 
};

