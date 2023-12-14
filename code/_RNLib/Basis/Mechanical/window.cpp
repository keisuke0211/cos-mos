//========================================
// 
// ウィンドウの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"
#include "../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ウィンドウクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CWindow::CWindow(void) {

	m_pos        = INITPOS2D;
	m_className  = NULL;
	m_width      = 0.0f;
	m_height     = 0.0f;
	m_areaWidth  = 0.0f;
	m_areaHeight = 0.0f;
	m_resolution = 0.0f;
	m_D3D9  = NULL;
	m_D3D9Device     = NULL;
}

//========================================
// デストラクタ
//========================================
CWindow::~CWindow(void) {

}

//========================================
// 初期化処理
//========================================
void CWindow::Init(void) {

}

//========================================
// 終了処理
//========================================
void CWindow::Uninit(void) {

	// Direct3Dデバイスの破棄
	if (m_D3D9Device != NULL) {
		m_D3D9Device->Release();
		m_D3D9Device = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_D3D9 != NULL) {
		m_D3D9->Release();
		m_D3D9 = NULL;
	}

	// ウインドウクラスの登録を解除
	UnregisterClass(m_className, m_instanceHandle);

	// タスクバーを表示
	ShowTaskBar(true);
}

//========================================
// 更新処理
//========================================
void CWindow::Update(void) {

	// ウィンドウ位置を取得
	RECT rec;
	GetWindowRect(m_hWnd, &rec);
	m_pos = Pos2D(rec.left, rec.top);
}

//========================================
// 生成処理
//========================================
int CWindow::Create(HINSTANCE& hInstance, WNDPROC lpfnWndProc) {

	// セッティング情報を取得
	const RNSettings::Info& settingsInfo = RNSettings::GetInfo();

	// ウインドウクラスの構造体
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		lpfnWndProc,					// ウインドウプロシージャ
		0,								// 0にする
		0,								// 0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		settingsInfo.projectName,		// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// ウインドウを生成
	m_hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		settingsInfo.projectName,		// ウインドウクラスの名前
		settingsInfo.projectName,		// ウインドウの名前
		WS_SYSMENU | WS_MINIMIZEBOX,	// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// ウインドウの左上Y座標
		settingsInfo.windowWidth,		// ウインドウの幅
		settingsInfo.windowHeight,		// ウインドウの高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// Direct3Dデバイスの生成
	if (CreateD3DDevice(hInstance) == -1)
		return -1;

	// ウィンドウのモードを設定する
	SetIsFullScreen(settingsInfo.isFullScreen);

	return 0;
}

//========================================
// 削除処理
//========================================
void CWindow::Delete(void) {

	// ウィンドウを破壊する
	DestroyWindow(m_hWnd);
}

//========================================
// タスクバーの表示/非表示を切り替え
//========================================
void CWindow::ShowTaskBar(const bool& isShow) {

	// タスクバーのハンドルを取得
	HWND taskBarHandle;
	taskBarHandle = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (isShow) {
		ShowWindow(taskBarHandle, SW_SHOW);
		SetFocus(m_hWnd);	// 今のウィンドウにフォーカスを当てる
	}
	else {
		ShowWindow(taskBarHandle, SW_HIDE);
	}
}

//========================================
// フルスクリーン設定
//========================================
void CWindow::SetIsFullScreen(const bool& isFullScreen) {

	// デスクトップの情報を取得
	HDC desctopHandle = GetDC(GetDesktopWindow());
	int screenSizeX   = GetDeviceCaps(desctopHandle, HORZRES);
	int screenSizeY   = GetDeviceCaps(desctopHandle, VERTRES);

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), desctopHandle);

	if (isFullScreen) {
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, screenSizeX, screenSizeY, SWP_SHOWWINDOW);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);
	}
	else {
		SetWindowPos(m_hWnd, HWND_TOP, (screenSizeX * 0.5f) - (m_width * 0.5f), (screenSizeY * 0.5f) - (m_height * 0.5f), m_width, m_height, SWP_SHOWWINDOW);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(m_hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(m_hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(!isFullScreen);
}

//========================================
// メッセージ処理
//========================================
void CWindow::Message(const char *pText, const char *pCaption) {

	MessageBox(m_hWnd, pText, pCaption, MB_OK);
	RNLib::Input().ClearInputInfo();
}

//========================================
// メッセージ処理(エラー)
//========================================
void CWindow::Message_ERROR(const char* pText) {

	// 実行モードの時は終了
	if (RNSystem::GetMode() == RNSystem::MODE::EXECUTION)
		return;

	MessageBox(m_hWnd, pText, "ERROR", MB_ICONWARNING);
	RNLib::Input().ClearInputInfo();
}

//========================================
// メッセージ処理(OK or CANCEL)
//========================================
bool CWindow::Message_OKCANCEL(const char* pText, const char* pCaption) {

	switch (MessageBox(m_hWnd, pText, pCaption, MB_OKCANCEL)) {
	case IDOK    :return true;
	case IDCANCEL:return false;
	}

	return false;
}

//========================================
// フォーカスが当たっているか調べる
//========================================
bool CWindow::FindFocused(const HWND& windowHandle) {

	// デバイスが失われている場合はフォーカスが当たっていないとみなす
	HRESULT hr = m_D3D9Device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
		return false;

	return (GetForegroundWindow() == windowHandle);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]ウィンドウクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// Direct3Dデバイスの生成
//========================================
int CWindow::CreateD3DDevice(HINSTANCE hInstance) {

	// セッティング情報を取得
	const RNSettings::Info& settingsInfo = RNSettings::GetInfo();

	// ウィンドウの情報を設定
	m_instanceHandle = hInstance;
	m_className      = settingsInfo.projectName;
	m_width          = settingsInfo.windowWidth;
	m_height         = settingsInfo.windowHeight;
	m_areaWidth      = m_width  * settingsInfo.resolution;
	m_areaHeight     = m_height * settingsInfo.resolution;
	m_resolution     = settingsInfo.resolution;

	// Direct3Dオブジェクトの生成
	m_D3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_D3D9 == NULL)
		return -1;

	// GPUのアダプタ番号を取得
	const int GPUAdapterIdx = GetTargetGPUAdapterIdx();

	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_D3D9->GetAdapterDisplayMode(GPUAdapterIdx, &d3ddm)))
		return E_FAIL;

	// デバイスのプレゼンテーションパラメータをゼロクリア
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = m_areaWidth;
	d3dpp.BackBufferHeight           = m_areaHeight;
	d3dpp.BackBufferFormat           = d3ddm.Format;				// バックバッファの形式
	d3dpp.BackBufferCount            = 1;							// バックバッファの数
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil     = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed                   = !settingsInfo.isFullScreen;	// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_D3D9Device))) {

		if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
			D3DDEVTYPE_HAL,
			m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_D3D9Device))) {

			if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
				D3DDEVTYPE_REF,
				m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_D3D9Device))) {
				return -1;
			}
		}
	}

	return 0;
}

//========================================
// 目的のGPUのアダプター番号を返す
//========================================
int CWindow::GetTargetGPUAdapterIdx(void) {

	// アダプタカウントを取得
	const int adapterCount = m_D3D9->GetAdapterCount();

	for (int cnt = 0; cnt < adapterCount; cnt++) {

		D3DADAPTER_IDENTIFIER9 adapterIdentifier;
		if (SUCCEEDED(m_D3D9->GetAdapterIdentifier(cnt, 0, &adapterIdentifier))) {

			// 目的のGPUと名称が一致した時、番号を返す
			// ※現状、DirectX9ではGeForce RTX 3050を認識できないので使い物にならない
			if (strstr(adapterIdentifier.Description, "GeForce RTX 3050") != NULL)
				return cnt;
		}
	}

	// 目的のGPUが見つからない場合は、デフォルトのアダプターを返す
	return D3DADAPTER_DEFAULT;
}