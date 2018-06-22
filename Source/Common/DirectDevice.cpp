/*
	@file	DirectDevice.cpp
	@brief	DirectX�f�o�C�X
	@author	�ԏ� �G�P
*/
#include "DirectDevice.h"

/*
	@brief	�R���X�g���N�^
*/
DirectDevice::DirectDevice()
{
	m_pD3D9 = NULL;
	m_pDevice3D = NULL;
}

/*
	@brief	�f�X�g���N�^
*/
DirectDevice::~DirectDevice()
{
}

/*
	@brief	������
*/
bool DirectDevice::Init(HWND hWnd)
{
	if (!CreateDevice(hWnd)) {
		MSG("DirectX�f�o�C�X�쐬�Ɏ��s");
		return false;
	}
	if (!CreateSoundDevice(hWnd)) {
		MSG("�T�E���h�f�o�C�X�̍쐬�Ɏ��s");
		return false;
	}
	return true;
}

/*
	@brief	���
*/
void DirectDevice::Release()
{
	m_pSoundDevice->Release();
	m_pDevice3D->Release();
	m_pD3D9->Release();
}

/*
	@brief	�����_�[�X�e�[�g�̃Z�b�^�[
*/
void DirectDevice::SetRenderState(RENDERSTATE RenderState)
{
	switch (RenderState)
	{
	case RENDER_DEFAULT:
	{
		/*! �s�����I�u�W�F�N�g */
		m_pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			/*!< �A���t�@�e�X�g�̖����� */
	}
	break;
	case RENDER_ALPHATEST:
	{
		/*! ���e�X�g�ɂ�铧���̈�̐؂蔲�� */
		m_pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			/*!< �A���t�@�e�X�g�̗L���� */ 
		//m_pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					/*!< �A���t�@�Q�ƒl */ 
		m_pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x60);					/*!< �A���t�@�Q�ƒl */

		m_pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	/*!< �A���t�@�e�X�g���i� */

	}
	break;
	}
}

/*
	@brief	�f�o�C�X�̐���
*/
bool DirectDevice::CreateDevice(HWND hWnd)
{
	/*! Direct3D9�I�u�W�F�N�g�̍쐬 */
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	/*! �f�B�X�v���C���擾 */
	D3DDISPLAYMODE Display;
	m_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	D3DPRESENT_PARAMETERS	D3DParam = {		
		/*! �X���b�v�`�F�C���ݒ� */
		WINDOW_WIDTH,WINDOW_HEIGHT,Display.Format,1,D3DMULTISAMPLE_NONE,0,
		D3DSWAPEFFECT_DISCARD,hWnd,TRUE,TRUE,D3DFMT_D24S8,0,0,D3DPRESENT_INTERVAL_DEFAULT
	};
	/*! HAL���[�h��3D�f�o�C�X�쐬 */
	if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
		if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
			/*! REF���[�h��3D�f�o�C�X�쐬 */
			if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
				if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &m_pDevice3D)))
				{
					/*�I 3D�f�o�C�X�쐬���s */
					m_pD3D9->Release();
					return false;
				}
	return true;
}

/*
	@brief	�T�E���h�f�o�C�X�̐���
*/
bool DirectDevice::CreateSoundDevice(HWND hWnd) {
	/*! �T�E���h�f�o�C�X�쐬 */
	DirectSoundCreate8(NULL, &m_pSoundDevice, NULL);
	/*! �������x���̃Z�b�g */
	if (FAILED(m_pSoundDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))) {
		return false;
	}
	return true;
}

