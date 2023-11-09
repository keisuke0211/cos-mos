//========================================
// 
// ウィンドウの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CWindowクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CWindow::CWindow(void) {
	m_pos         = INITD3DXVECTOR3;
	m_pClassName  = NULL;
	m_fWidth      = 0.0f;
	m_fHeight     = 0.0f;
	m_fCenterX    = 0.0f;
	m_fCenterY    = 0.0f;
	m_fAreaWidth  = 0.0f;
	m_fAreaHeight = 0.0f;
	m_fResolution = 0.0f;
	m_pD3D        = NULL;
	m_pD3DDevice  = NULL;
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
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
	if (m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL) {
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	// ウインドウクラスの登録を解除
	UnregisterClass(m_pClassName, m_hInstance);

	ShowTaskBar(true);
}

//========================================
// 更新処理
//========================================
void CWindow::Update(void) {
	RECT rec;
	GetWindowRect(m_hWnd, &rec);
	m_pos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CWindow::Destroy(void) {
	DestroyWindow(m_hWnd);
}

//========================================
// CreateWindowRN関数 - ウインドウ作成 -
// Author:RIKU NISHIMURA
//========================================
int CWindow::Create(HINSTANCE hInstance, CreateInfo cwi) {
	// ウインドウクラスの構造体
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		cwi.lpfnWndProc,				// ウインドウプロシージャ
		0,								// 0にする
		0,								// 0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		cwi.pClassName,					// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// ウインドウを生成
	m_hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		cwi.pClassName,					// ウインドウクラスの名前
		cwi.pName,						// ウインドウの名前
		WS_SYSMENU | WS_MINIMIZEBOX,	// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// ウインドウの左上Y座標
		cwi.fWidth,						// ウインドウの幅
		cwi.fHeight,					// ウインドウの高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// Direct3Dデバイスの生成
	if (CreateDirect3DDevice(hInstance, cwi) == -1)
		return -1;

	// ウィンドウのモードを設定する
	SetFullScreen(cwi.bFullScreen);

	return 0;
}

//========================================
// Direct3Dデバイスの生成
// Author:RIKU NISHIMURA
//========================================
int CWindow::CreateDirect3DDevice(HINSTANCE hInstance, CreateInfo ci) {
	// ウィンドウの情報を記憶
	m_hInstance    = hInstance;
	m_pClassName   = ci.pClassName;
	m_fWidth       = ci.fWidth;
	m_fHeight      = ci.fHeight;
	m_fCenterX     = ci.fWidth  * 0.5f;
	m_fCenterY     = ci.fHeight * 0.5f;
	m_fAreaWidth   = ci.fWidth  * ci.fResolution;
	m_fAreaHeight  = ci.fHeight * ci.fResolution;
	m_fResolution  = ci.fResolution;

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
		return -1;

	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return E_FAIL;

	// デバイスのプレゼンテーションパラメータをゼロクリア
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth            = ci.fWidth  * ci.fResolution;
	d3dpp.BackBufferHeight           = ci.fHeight * ci.fResolution;
	d3dpp.BackBufferFormat           = d3ddm.Format;				// バックバッファの形式
	d3dpp.BackBufferCount            = 1;							// バックバッファの数
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil     = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed                   = !ci.bFullScreen;				// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice))) {

		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice))) {

			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice))) {
				return -1;
			}
		}
	}

	return 0;
}

//========================================
// タスクバーの表示/非表示を切り替え
// Author:RIKU NISHIMURA
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void CWindow::ShowTaskBar(const bool bShow) {
	// タスクバーのハンドルを取得
	HWND hTask;
	hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow) {
		ShowWindow(hTask, SW_SHOW);
		SetFocus(m_hWnd);	// 今のウィンドウにフォーカスを当てる
	}
	else {
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// フルスクリーン設定
// Author:RIKU NISHIMURA
//========================================
void CWindow::SetFullScreen(bool bFullScreen) {
	HDC hDC       = GetDC(GetDesktopWindow());		// デスクトップのハンドル
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// デスクトップの画面サイズX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// デスクトップの画面サイズY

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen) {
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// スタイル変更
		SetWindowLong(m_hWnd,
			GWL_STYLE, WS_POPUP);
	}
	else {
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (m_fWidth  * 0.5f),	// 位置を中央に合わせる
			(nScrSizeY * 0.5f) - (m_fHeight * 0.5f),
			m_fWidth,
			m_fHeight,
			SWP_SHOWWINDOW);

		// スタイル変更
		SetWindowLong(m_hWnd, GWL_STYLE,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	ShowWindow(m_hWnd, SW_NORMAL);	// ウインドウの表示状態を設定
	UpdateWindow(m_hWnd);			// クライアント領域を更新
	ShowTaskBar(!bFullScreen);		// タスクバーを表示/非表示
}

//========================================
// メッセージ処理
// Author:RIKU NISHIMURA
//========================================
void CWindow::Message(const char *pText, const char *pCaption) {
	MessageBox(m_hWnd, pText, pCaption, MB_OK);
}

//========================================
// メッセージ処理(エラー)
// Author:RIKU NISHIMURA
//========================================
void CWindow::Message_ERROR(const char* pText) {
	MessageBox(m_hWnd, pText, "ERROR", MB_ICONWARNING);
}

//========================================
// メッセージ処理(OK or CANCEL)
// Author:RIKU NISHIMURA
//========================================
bool CWindow::Message_OKCANCEL(const char* pText, const char* pCaption) {
	switch (MessageBox(m_hWnd, pText, pCaption, MB_OKCANCEL)) {
	case IDOK:
		return true;
	case IDCANCEL:
		return false;
	}

	return false;
}

//========================================
// フォーカスが当たっているか調べる
// Author:RIKU NISHIMURA
//========================================
bool CWindow::FindFocused(HWND hWnd) {
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	// デバイスが失われている場合はフォーカスが当たっていないとみなす
	if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
		return false;

	return (GetForegroundWindow() == hWnd);
}