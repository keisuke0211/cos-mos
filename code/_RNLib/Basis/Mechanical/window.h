//========================================
// 
// ウィンドウの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// ウィンドウクラス
class _RNC_Window {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Window         ();
	~_RNC_Window        ();
	void Init           (void);
	void Uninit         (void);
	void Update         (void);
	int  Create         (HINSTANCE& hInstance, WNDPROC lpfnWndProc);
	void Delete         (void);
	void ShowTaskBar    (const bool& isShow);
	void SetIsFullScreen(const bool& isFullScreen);
	bool FindFocused    (const HWND& windowHandle);
	// メッセージ処理
	void Message         (const char* pText, const char* pCaption);
	void Message_ERROR   (const char* pText);
	bool Message_OKCANCEL(const char* pText, const char* pCaption);
	// 設定取得
	Pos2D&  GetPos       (void) { return m_pos; }
	float&  GetWidth     (void) { return m_width; }
	float&  GetHeight    (void) { return m_height; }
	Size2D  GetSize      (void) { return Pos2D(m_width, m_height); }
	float   GetCenterX   (void) { return m_width * 0.5f; }
	float   GetCenterY   (void) { return m_height * 0.5f; }
	Pos2D   GetCenterPos (void) { return Pos2D(m_width * 0.5f, m_height * 0.5f); }
	float&  GetAreaWidth (void) { return m_areaWidth; }
	float&  GetAreaHeight(void) { return m_areaHeight; }
	float&  GetResolution(void) { return m_resolution; }
	Device& GetD3DDevice (void) { return m_D3D9Device; }
	HWND&   GetHandle    (void) { return m_hWnd; }

private:
	//========== [[[ 関数宣言 ]]]
	int CreateD3DDevice(HINSTANCE& hInstance);
	int GetTargetGPUAdapterIdx(void);

	//========== [[[ 変数宣言 ]]]
	Pos2D       m_pos;
	HINSTANCE   m_instanceHandle;
	const char* m_className;
	float       m_width;
	float       m_height;
	float       m_areaWidth;	// 領域の幅
	float       m_areaHeight;	// 領域の高さ
	float       m_resolution;	// 解像度
	LPDIRECT3D9 m_D3D9;
	Device      m_D3D9Device;
	HWND        m_hWnd;
};
