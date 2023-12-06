//========================================
// 
// ウィンドウのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ window.h ]]]
//========================================
#ifndef _WINDOW_H_
#define _WINDOW_H_

//****************************************
// マクロ定義
//****************************************
#define IMGUI_CORRECT_X (0.98775f)	// ImGui用のウィンドウ領域補正値X
#define IMGUI_CORRECT_Y (0.94735f)	// ImGui用のウィンドウ領域補正値Y
// (※ImGuiが当たり判定の際にDirect3Dデバイスの画面サイズを参照しており、誤差を埋める為の数値)

//****************************************
// クラス定義
//****************************************
// ウィンドウクラス
class CWindow {
public:
	//========== [[[ 構造体定義 ]]]
	// 作成情報構造体
	typedef struct {
		WNDPROC     lpfnWndProc;	// ウィンドウプロシージャ
		const char* pClassName;
		const char* pName;
		float       fWidth;
		float       fHeight;
		bool        bFullScreen;
		float       fResolution;	// 解像度倍率
	}CreateInfo;

	//========== [[[ 関数宣言 ]]]
	CWindow();
	~CWindow();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Destroy(void);
	int  Create       (HINSTANCE hInstance, CreateInfo ci);
	void ShowTaskBar  (const bool bShow);
	void SetFullScreen(const bool bFullScreen);
	bool FindFocused  (HWND hWnd);
	// メッセージ処理
	void Message         (const char* pText, const char* pCaption);
	void Message_ERROR   (const char* pText);
	bool Message_OKCANCEL(const char* pText, const char* pCaption);
	// 設定取得
	D3DXVECTOR3       GetPos       (void) { return m_pos;         }
	float             GetWidth     (void) { return m_fWidth;      }
	float             GetHeight    (void) { return m_fHeight;     }
	Scale2D           GetScale     (void) { return Pos2D(m_fWidth, m_fHeight); }
	float             GetCenterX   (void) { return m_fCenterX;    }
	float             GetCenterY   (void) { return m_fCenterY;    }
	Pos2D             GetCenterPos (void) { return Pos2D(m_fCenterX, m_fCenterY); }
	float             GetAreaWidth (void) { return m_fAreaWidth;  }
	float             GetAreaHeight(void) { return m_fAreaHeight; }
	float             GetResolution(void) { return m_fResolution; }
	Device&           GetD3DDevice (void) { return m_pD3DDevice;  }
	HWND              GetHandle    (void) { return m_hWnd;        }

private:
	//========== [[[ 関数宣言 ]]]
	int CreateDirect3DDevice(HINSTANCE hInstance, CreateInfo ci);

	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3       m_pos;
	HINSTANCE         m_hInstance;
	const char*       m_pClassName;
	float             m_fWidth;
	float             m_fHeight;
	float             m_fCenterX;
	float             m_fCenterY;
	float             m_fAreaWidth;		// 領域の幅
	float             m_fAreaHeight;	// 領域の高さ
	float             m_fResolution;	// 解像度
	LPDIRECT3D9       m_pD3D;
	Device m_pD3DDevice;
	HWND              m_hWnd;
};

#endif