/*
	@file	DirectDevice.h
	@brief	DirectX�f�o�C�X
	@author	�ԏ� �G�P
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
	@brief	�}�N����`
*/
#define _CRT_SECURE_NO_WARNINGS
#define MSG(str) MessageBox(NULL,_T(str),NULL,MB_OK)
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[](p);(p)=NULL;}}
#define SAFE_RELEASE(p){if(p) {(p)->Release();(p)=NULL;}}

/*
	@brief	�萔�錾
*/
const int WINDOW_WIDTH = 1920;		/*!< ��ʉ��� */
const int WINDOW_HEIGHT = 1088;		/*!< ��ʏc�� */

/*
	@brief	�f�o�C�X�Ǘ��N���X
*/
class DirectDevice
{
public:
	/*! �f�X�g���N�^ */
	~DirectDevice();
	/*! �V���O���g�� */
	static DirectDevice& GetInstance() {
		static DirectDevice instance;
		return instance;
	}

	/*! 
		@enum	RENDERSTATE
		@brief	�����_�����O�^�C�v�̐؂�ւ�
	*/
	enum RENDERSTATE
	{
		RENDER_DEFAULT,		/*!< �f�t�H���g(�s����) */ 
		RENDER_ALPHATEST	/*!< ���e�X�g�i���߁j*/ 
	};


	bool Init(HWND hWnd);	/*!< �f�o�C�X�̏����� */
	void Release();			/*!< �f�o�C�X�̉��	*/
	void SetRenderState(RENDERSTATE RenderState = RENDER_ALPHATEST);/*!< �����_�[�X�e�[�g�̕ύX */

	/*! �Q�b�^�[ */
	IDirect3DDevice9* GetDevice() { return m_pDevice3D; }		/*!< �f�o�C�X */
	IDirectSound8* GetSoundDevice() { return m_pSoundDevice; }	/*!< �T�E���h�f�o�C�X */
private:
	/*! �R���X�g���N�^ */
	DirectDevice();

	
	
	bool CreateDevice(HWND hWnd);		/*!< �f�o�C�X�̐��� */
	bool CreateSoundDevice(HWND hWnd);	/*!< �T�E���h�f�o�C�X�̐��� */

	/*! �ϐ� */
	IDirect3D9*			m_pD3D9;
	IDirect3DDevice9* m_pDevice3D;
	IDirectSound8*		m_pSoundDevice; 
};

