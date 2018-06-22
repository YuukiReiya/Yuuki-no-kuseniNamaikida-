/*
	@file	MAIN.cpp
	@brief	メイン
	@author	番場 宥輝
*/
#include "MAIN.h"
#include "../GameObject/Manager/InputManager/InputManager.h"
#include "../GameObject/Manager/SoundManager/SoundManager.h"
#ifdef _DEBUG
#include "../Debug/Debug.h"
#endif // _DEBUG

/*! プロトタイプ宣言 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
	@var	g_pMain
*/
MAIN* g_pMain;

/*
	@brief	エントリポイント
*/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT) {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//メモリリークを特定
#endif
	g_pMain = new MAIN;

	/*! 乱数の初期化 */
	srand((unsigned)time(NULL));

	/*! 高解像度タイマーに対応しているか確認 */
	if (!g_pMain->HighQualityTimmer()) {
		MSG("未対応");
	}

	if (SUCCEEDED(g_pMain)) {
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, -8, 0,
			WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME))) {
			if (SUCCEEDED(g_pMain->Init())) {

#ifdef _DEBUG
				/*! デバッグモードの時のみ */
				Debug::m_hWnd = g_pMain->m_hWnd;
#endif // _DEBUG


				g_pMain->Loop();
			}
		}
	}
	g_pMain->Destroy();
	/*! アプリケーション終了 */
	SAFE_DELETE(g_pMain);
	return 0;
}

/*
	@brief	メッセージプロシージャ
	@detail	コールバック関数
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pMain->MsgProc(hWnd, msg, wParam, lParam);
}

/*
	@brief	コンストラクタ
*/
MAIN::MAIN()
{
}

/*
	@brief	デストラクタ
*/
MAIN::~MAIN()
{
	Destroy();
}

/*
	@brief	メッセージプロシージャ
	@detail	メンバ関数
*/
LRESULT MAIN::MsgProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
			/*! Escキーを押されたら */
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		/*! ウィンドウが破棄されたとき */
	case WM_DESTROY:
		PostQuitMessage(0);	/*!< WM_QUITメッセージをメッセージキューに送る */
		break;
	}
	/*! デフォルトのメッセージ処理を行う */
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
	@brief	初期化
*/
HRESULT MAIN::Init()
{
	if (!DirectDevice::GetInstance().Init(m_hWnd)) {
		return E_FAIL;
	}
	return S_OK;
}

/*
	@brief	解放
*/
void MAIN::Destroy()
{
	SAFE_DELETE(m_pRoot);
	SoundManager::GetInstance().Destroy();
}

/*
	@brief	アプリケーション処理の入り口
*/
void MAIN::Loop()
{
	/*! マウスカーソルの表示 */
	ShowCursor(false);

	/*! サウンドマネージャー初期化 */
	if (!SoundManager::GetInstance().Init()) {
		MSG("サウンドマネージャー初期化に失敗");
	}

	/*! シーンの初期化 */
	m_pRoot = new SceneRoot;
	m_pRoot->Init();

	/*! FPSの初期化 */
	InitFPS();

	/*! メッセージループ */
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
			/*! App処理 */
			App();
		}
	}
}

/*
	@brief	ウィンドウの初期化
*/
HRESULT MAIN::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	/*! ウィンドウの設定 */
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

	/*! ウィンドウの作成 */
	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		iX, iY, iWidth, iHeight, NULL, NULL, hInstance, NULL);

	if (!m_hWnd){
		//ウィンドウの生成に失敗
		MSG("ウィンドウの作成に失敗");
		return E_FAIL;
	}
	/*! ウィンドウ表示 */
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

/*
	@brief	アプリケーション処理
*/
void MAIN::App()
{
//FPSの確認
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


	/*! FPS調整 */
	if (SleepApp())return;
	/*! 更新処理 */
	Update();
	/*! 描画処理 */
	Render();
}

/*
	@brief	レンダリング
*/
void MAIN::Render()
{
	if (SUCCEEDED(DirectDevice::GetInstance().GetDevice()->BeginScene())) {
		/*! 背景クリア色 */
		DWORD ClearColor = 0xff808080;	
		/*! クリアターゲットのフラグ */
		DWORD flag = D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER;
		/*! 背景クリア */
		DirectDevice::GetInstance().GetDevice()->Clear(0, NULL, flag, ClearColor, 1.0f, 0);
		/*! 透過処理 */
		DirectDevice::GetInstance().SetRenderState();
		/*! シーンの描画 */
		m_pRoot->Render();

		/*! 描画終了 */
		DirectDevice::GetInstance().GetDevice()->EndScene();
	}
	/*! 描画反映 */
	DirectDevice::GetInstance().GetDevice()->Present(NULL, NULL, NULL, NULL);
}

/*
	@brief	更新
*/
void MAIN::Update()
{
	/*! 入力の更新 */
	InputManager::GetInstance().Update();
	/*! シーンの更新 */
	m_pRoot->Update(m_pRoot);
}

/*
	@brief	高解像度タイマーが対応しているか確認
*/
bool MAIN::HighQualityTimmer()
{
	/*! QueryPerformanceFrequencyの対応を見る */
	if (QueryPerformanceFrequency(&m_TimeFreq) == FALSE){
		MSG("未対応");
		return false;
	}

	return true;//対応している
}

/*
	@brief	FPSの初期化
*/
void MAIN::InitFPS()
{
	m_FrameTime = 0;
	m_fps = 0;
	/*! 初回の計算 */
	QueryPerformanceCounter(&m_TimeStart);
}

/*
	@brief	アプリケーション処理のお休み
*/
bool MAIN::SleepApp()
{
	/*! 今の時間を取得 */
	QueryPerformanceCounter(&m_TimeEnd);
	/*! 経過時間 */
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);

	/*! FPSのズレを確認 */
	if (m_FrameTime < MIN_FRAME_TIME) {
		/*! フレームを合わせる */
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_FrameTime) * 1000);/*!< ズレ分 */

		timeBeginPeriod(1);	/*!< 分解性能を上げる */
		Sleep(SleepTime);	/*!< お休み */
		timeEndPeriod(1);	/*!< 戻す */

		return true;
	}
	return false;
}
