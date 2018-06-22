/*
	@file	MAIN.h
	@brief	���C��
	@author	�ԏ� �G�P
*/
#pragma once

#include "../Common/Common.h"
#include "../Scene/SceneRoot/SceneRoot.h"
#include <mmsystem.h>

/*
	@brief	���C���N���X
*/
class MAIN
{
public:
	/*! �R���X�g���N�^ */
	MAIN();
	/*! �f�X�g���N�^ */
	~MAIN();

	/*! ���b�Z�[�W�v���V�[�W�� */
	LRESULT MsgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT Init();	/*!< ������ */
	void Destroy();	/*!< ��� */
	void Loop();	/*!< �A�v���P�[�V���������̓���� */

	/*! �E�B���h�E�̏����� */
	HRESULT InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName);
	bool HighQualityTimmer();	/*!< ���𑜓x�^�C�}�[�̑Ή��L�� */

	/*! �ϐ� */
	HWND m_hWnd;
private:
	void App();		/*!< �A�v���P�[�V�������� */
	void Render();	/*! �����_�����O <*/
	void Update();	/*!< �X�V */
	void InitFPS();	/*!< FPS������ */
	bool SleepApp();/*!< �A�v���P�[�V�������������x�� */


	SceneRoot* m_pRoot;/*!< �V�[�� */

	/*! FPS�𑵂��� */
	const float MIN_FRAME_TIME = 1.0f / FPS;
	float m_FrameTime;
	LARGE_INTEGER m_TimeStart;	
	LARGE_INTEGER m_TimeEnd;
	LARGE_INTEGER m_TimeFreq;
	float m_fps;

};

