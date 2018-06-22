/*
	@file	MAIN.cpp
	@brief	���C��
	@author	�ԏ� �G�P
*/
#include "MAIN.h"
#include "../GameObject/Manager/InputManager/InputManager.h"
#include "../GameObject/Manager/SoundManager/SoundManager.h"
#ifdef _DEBUG
#include "../Debug/Debug.h"
#endif // _DEBUG

/*! �v���g�^�C�v�錾 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
	@var	g_pMain
*/
MAIN* g_pMain;

/*
	@brief	�G���g���|�C���g
*/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT) {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//���������[�N�����
#endif
	g_pMain = new MAIN;

	/*! �����̏����� */
	srand((unsigned)time(NULL));

	/*! ���𑜓x�^�C�}�[�ɑΉ����Ă��邩�m�F */
	if (!g_pMain->HighQualityTimmer()) {
		MSG("���Ή�");
	}

	if (SUCCEEDED(g_pMain)) {
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, -8, 0,
			WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME))) {
			if (SUCCEEDED(g_pMain->Init())) {

#ifdef _DEBUG
				/*! �f�o�b�O���[�h�̎��̂� */
				Debug::m_hWnd = g_pMain->m_hWnd;
#endif // _DEBUG


				g_pMain->Loop();
			}
		}
	}
	g_pMain->Destroy();
	/*! �A�v���P�[�V�����I�� */
	SAFE_DELETE(g_pMain);
	return 0;
}

/*
	@brief	���b�Z�[�W�v���V�[�W��
	@detail	�R�[���o�b�N�֐�
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pMain->MsgProc(hWnd, msg, wParam, lParam);
}

/*
	@brief	�R���X�g���N�^
*/
MAIN::MAIN()
{
}

/*
	@brief	�f�X�g���N�^
*/
MAIN::~MAIN()
{
	Destroy();
}

/*
	@brief	���b�Z�[�W�v���V�[�W��
	@detail	�����o�֐�
*/
LRESULT MAIN::MsgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
			/*! Esc�L�[�������ꂽ�� */
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		/*! �E�B���h�E���j�����ꂽ�Ƃ� */
	case WM_DESTROY:
		PostQuitMessage(0);	/*!< WM_QUIT���b�Z�[�W�����b�Z�[�W�L���[�ɑ��� */
		break;
	}
	/*! �f�t�H���g�̃��b�Z�[�W�������s�� */
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
	@brief	������
*/
HRESULT MAIN::Init()
{
	if (!DirectDevice::GetInstance().Init(m_hWnd)) {
		return E_FAIL;
	}
	return S_OK;
}

/*
	@brief	���
*/
void MAIN::Destroy()
{
	SAFE_DELETE(m_pRoot);
	SoundManager::GetInstance().Destroy();
}

/*
	@brief	�A�v���P�[�V���������̓����
*/
void MAIN::Loop()
{
	/*! �}�E�X�J�[�\���̕\�� */
	ShowCursor(false);

	/*! �T�E���h�}�l�[�W���[������ */
	if (!SoundManager::GetInstance().Init()) {
		MSG("�T�E���h�}�l�[�W���[�������Ɏ��s");
	}

	/*! �V�[���̏����� */
	m_pRoot = new SceneRoot;
	m_pRoot->Init();

	/*! FPS�̏����� */
	InitFPS();

	/*! ���b�Z�[�W���[�v */
	MSG msg{ 0 };
	ZeroMemory(&msg, sizeof(&msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*! App���� */
			App();
		}
	}
}

/*
	@brief	�E�B���h�E�̏�����
*/
HRESULT MAIN::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	/*! �E�B���h�E�̐ݒ� */
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	/*! �E�B���h�E�̍쐬 */
	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		iX, iY, iWidth, iHeight, NULL, NULL, hInstance, NULL);

	if (!m_hWnd){
		//�E�B���h�E�̐����Ɏ��s
		MSG("�E�B���h�E�̍쐬�Ɏ��s");
		return E_FAIL;
	}
	/*! �E�B���h�E�\�� */
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

/*
	@brief	�A�v���P�[�V��������
*/
void MAIN::App()
{
//FPS�̊m�F
#ifdef _DEBUG
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf(str, "fps=%d", frame);
	if (timeGetTime() - time > 1000) {
		time = timeGetTime();
		frame = 0;
		SetWindowTextA(Debug::m_hWnd, str);
	}
#endif // _DEBUG


	/*! FPS���� */
	if (SleepApp())return;
	/*! �X�V���� */
	Update();
	/*! �`�揈�� */
	Render();
}

/*
	@brief	�����_�����O
*/
void MAIN::Render()
{
	if (SUCCEEDED(DirectDevice::GetInstance().GetDevice()->BeginScene())) {
		/*! �w�i�N���A�F */
		DWORD ClearColor = 0xff808080;	
		/*! �N���A�^�[�Q�b�g�̃t���O */
		DWORD flag = D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER;
		/*! �w�i�N���A */
		DirectDevice::GetInstance().GetDevice()->Clear(0, NULL, flag, ClearColor, 1.0f, 0);
		/*! ���ߏ��� */
		DirectDevice::GetInstance().SetRenderState();
		/*! �V�[���̕`�� */
		m_pRoot->Render();

		/*! �`��I�� */
		DirectDevice::GetInstance().GetDevice()->EndScene();
	}
	/*! �`�攽�f */
	DirectDevice::GetInstance().GetDevice()->Present(NULL, NULL, NULL, NULL);
}

/*
	@brief	�X�V
*/
void MAIN::Update()
{
	/*! ���͂̍X�V */
	InputManager::GetInstance().Update();
	/*! �V�[���̍X�V */
	m_pRoot->Update(m_pRoot);
}

/*
	@brief	���𑜓x�^�C�}�[���Ή����Ă��邩�m�F
*/
bool MAIN::HighQualityTimmer()
{
	/*! QueryPerformanceFrequency�̑Ή������� */
	if (QueryPerformanceFrequency(&m_TimeFreq) == FALSE){
		MSG("���Ή�");
		return false;
	}

	return true;//�Ή����Ă���
}

/*
	@brief	FPS�̏�����
*/
void MAIN::InitFPS()
{
	m_FrameTime = 0;
	m_fps = 0;
	/*! ����̌v�Z */
	QueryPerformanceCounter(&m_TimeStart);
}

/*
	@brief	�A�v���P�[�V���������̂��x��
*/
bool MAIN::SleepApp()
{
	/*! ���̎��Ԃ��擾 */
	QueryPerformanceCounter(&m_TimeEnd);
	/*! �o�ߎ��� */
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);

	/*! FPS�̃Y�����m�F */
	if (m_FrameTime < MIN_FRAME_TIME) {
		/*! �t���[�������킹�� */
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_FrameTime) * 1000);/*!< �Y���� */

		timeBeginPeriod(1);	/*!< ���𐫔\���グ�� */
		Sleep(SleepTime);	/*!< ���x�� */
		timeEndPeriod(1);	/*!< �߂� */

		return true;
	}
	return false;
}
