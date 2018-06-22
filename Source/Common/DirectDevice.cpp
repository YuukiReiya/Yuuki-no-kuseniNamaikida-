/*
	@file	DirectDevice.cpp
	@brief	DirectXデバイス
	@author	番場 宥輝
*/
#include "DirectDevice.h"

/*
	@brief	コンストラクタ
*/
DirectDevice::DirectDevice()
{
	m_pD3D9 = NULL;
	m_pDevice3D = NULL;
}

/*
	@brief	デストラクタ
*/
DirectDevice::~DirectDevice()
{
}

/*
	@brief	初期化
*/
bool DirectDevice::Init(HWND hWnd)
{
	if (!CreateDevice(hWnd)) {
		MSG("DirectXデバイス作成に失敗");
		return false;
	}
	if (!CreateSoundDevice(hWnd)) {
		MSG("サウンドデバイスの作成に失敗");
		return false;
	}
	return true;
}

/*
	@brief	解放
*/
void DirectDevice::Release()
{
	m_pSoundDevice->Release();
	m_pDevice3D->Release();
	m_pD3D9->Release();
}

/*
	@brief	レンダーステートのセッター
*/
void DirectDevice::SetRenderState(RENDERSTATE RenderState)
{
	switch (RenderState)
	{
	case RENDER_DEFAULT:
	{
		/*! 不透明オブジェクト */
		m_pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			/*!< アルファテストの無効化 */
	}
	break;
	case RENDER_ALPHATEST:
	{
		/*! αテストによる透明領域の切り抜き */
		m_pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			/*!< アルファテストの有効化 */ 
		//m_pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					/*!< アルファ参照値 */ 
		m_pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x60);					/*!< アルファ参照値 */

		m_pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	/*!< アルファテスト合格基準 */

	}
	break;
	}
}

/*
	@brief	デバイスの生成
*/
bool DirectDevice::CreateDevice(HWND hWnd)
{
	/*! Direct3D9オブジェクトの作成 */
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	/*! ディスプレイ情報取得 */
	D3DDISPLAYMODE Display;
	m_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	D3DPRESENT_PARAMETERS	D3DParam = {		
		/*! スワップチェイン設定 */
		WINDOW_WIDTH,WINDOW_HEIGHT,Display.Format,1,D3DMULTISAMPLE_NONE,0,
		D3DSWAPEFFECT_DISCARD,hWnd,TRUE,TRUE,D3DFMT_D24S8,0,0,D3DPRESENT_INTERVAL_DEFAULT
	};
	/*! HALモードで3Dデバイス作成 */
	if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
		if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
			/*! REFモードで3Dデバイス作成 */
			if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
				if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
				{
					/*！ 3Dデバイス作成失敗 */
					m_pD3D9->Release();
					return false;
				}
	return true;
}

/*
	@brief	サウンドデバイスの生成
*/
bool DirectDevice::CreateSoundDevice(HWND hWnd) {
	/*! サウンドデバイス作成 */
	DirectSoundCreate8(NULL, &m_pSoundDevice, NULL);
	/*! 協調レベルのセット */
	if (FAILED(m_pSoundDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))) {
		return false;
	}
	return true;
}

