//========================================
// 
// RNライブラリの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNsettings.h"

//****************************************
// プロトタイプ宣言
//****************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//****************************************
// クラス定義
//****************************************
// 3Dオブジェクトクラス
class C3DObject {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {}
	void Uninit(void) {}
	void Update(void) {}

	//========== [[[ 変数宣言 ]]]
	CMotion3D m_motion3D;
	CSetUp3D  m_setUp3D;
};

// 計算クラス
class CCalculation {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {}
	void Uninit(void) {}
	void Update(void) {}

	//========== [[[ 変数宣言 ]]]
	CEase     m_ease;
	CGeometry m_geometry;
	CMatrix   m_matrix;
};

// その他クラス
class COther {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {
		m_defaultData.Init();
	}
	void Uninit(void) {}
	void Update(void) {}

	//========== [[[ 変数宣言 ]]]
	CDefaultData m_defaultData;
};

//****************************************
// 無名空間
//****************************************
namespace {

	//========== [[[ 関数宣言 ]]]
	void Init(HINSTANCE hInstance);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(void);
	void Save(void);

	//========== [[[ 変数宣言 ]]]
	RNLib::SIGNAL  signal = RNLib::SIGNAL::NONE;
	unsigned short m_nCount;
	DWORD          m_dwCurrentTime;
	DWORD          m_dwExecLastTime;
	DWORD          m_dwFrameCount;
	DWORD          m_dwFPSLastTime;
	int            m_nFPSCount;
	bool           m_bBlinkF2;
	bool           m_bBlinkF4;
	bool           m_bSpace3DStop;
	bool           m_bSpace3DStopRsrv;
	bool           m_bSceneSwap;
	// RNオブジェクト
	C3DObject      m_3DObject;
	CCalculation   m_calculation;
	COther         m_other;

	CText2D      m_text2D;
	CPolygon2D   m_polygon2D;
	CModel       m_model;
	CText3D      m_text3D;
	CPolygon3D   m_polygon3D;
	CLight3D     m_light3D;
	CCamera      m_camera;
	CFile        m_file;
	CInput       m_input;
	CMemory      m_memory;
	CSound       m_sound;
	CWindow      m_window;
	CText        m_text;
	CTexture     m_texture;
	CTransition  m_transition;
	CDrawMng     m_drawMng;
	CDrawState   m_drawState;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNLibの関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 取得・設定系処理
// Author:RIKU NISHIMURA
//========================================
RNLib::SIGNAL RNLib::GetSignal     (void)       { return signal;       }
int           RNLib::GetCount      (void)       { return m_nCount;       }
int           RNLib::GetFPSCount   (void)       { return m_nFPSCount;    }
bool          RNLib::GetBlinkF2    (void)       { return m_bBlinkF2;     }
bool          RNLib::GetBlinkF4    (void)       { return m_bBlinkF4;     }
void          RNLib::SetSpace3DStop(bool bStop) { m_bSpace3DStopRsrv = bStop; }
bool          RNLib::GetSpace3DStop(void)       { return m_bSpace3DStop; }
bool          RNLib::GetSceneSwap  (void)       { return m_bSceneSwap;   }
//========== [[[ RNオブジェクト ]]]
// 3DObject    RNLib::              (void) { return                           }
CMotion3D&     RNLib::Motion3D      (void) { return m_3DObject.m_motion3D;    }
CSetUp3D&      RNLib::SetUp3D       (void) { return m_3DObject.m_setUp3D;     }
// Calculation RNLib::              (void) { return                           }
CEase&         RNLib::Ease          (void) { return m_calculation.m_ease;     }
CGeometry&     RNLib::Geometry      (void) { return m_calculation.m_geometry; }
CMatrix&       RNLib::Matrix        (void) { return m_calculation.m_matrix;   }
// Other...    RNLib::              (void) { return                           }
CDefaultData&  RNLib::DefaultData   (void) { return m_other.m_defaultData;    }

CText2D&      RNLib::Text2D        (void)       { return m_text2D;       }
CPolygon2D&   RNLib::Polygon2D     (void)       { return m_polygon2D;    }
CModel&       RNLib::Model         (void)       { return m_model;        }
CText3D&      RNLib::Text3D        (void)       { return m_text3D;       }
CPolygon3D&   RNLib::Polygon3D     (void)       { return m_polygon3D;    }
CLight3D&     RNLib::Light3D       (void)       { return m_light3D;      }
CCamera&      RNLib::Camera3D      (void)       { return m_camera;       }
CFile&        RNLib::File          (void)       { return m_file;         }
CInput&       RNLib::Input         (void)       { return m_input;        }
CMemory&      RNLib::Memory        (void)       { return m_memory;       }
CSound&       RNLib::Sound         (void)       { return m_sound;        }
CWindow&      RNLib::Window        (void)       { return m_window;       }
CText&        RNLib::Text          (void)       { return m_text;         }
CTexture&     RNLib::Texture       (void)       { return m_texture;      }
CTransition&  RNLib::Transition    (void)       { return m_transition;   }
CDrawMng&     RNLib::DrawMng       (void)       { return m_drawMng;      }
CDrawState&   RNLib::DrawStateMng  (void)       { return m_drawState;    }

//========================================
// メインループ
// Author:RIKU NISHIMURA
//========================================
bool RNLib::MainLoop(HINSTANCE instanceHandle, const char* settingsPath) {

	static bool isMessageLoop = false;
	static MSG  msg;

	// シグナルが無し(初回)の時、
	if (signal == RNLib::SIGNAL::NONE) {

		// 設定ファイルを読み込み&書き出し
		if (RNSettings::LoadAndSave(settingsPath))
		{// 成功した時、
			RNSettings::Info settingsInfo = RNSettings::GetInfo();

			m_window.Create(
				instanceHandle,
				{
					WindowProc,						// ウィンドウプロシージャ
					settingsInfo.projectName,		// ウィンドウクラスの名前
					settingsInfo.projectName,		// ウィンドウの名前
					settingsInfo.windowWidth,		// ウィンドウの幅
					settingsInfo.windowHeight,		// ウィンドウの高さ
					settingsInfo.isFullScreen,		// ウィンドウモード
					settingsInfo.resolution,		// 解像度倍率
				}
			);
		}
		else
		{// 失敗した時、シグナルを終了にする
			signal = RNLib::SIGNAL::UNINIT;
		}
	}

	if (isMessageLoop) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT) {
				signal = SIGNAL::SAVE;
				isMessageLoop = false;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return true;
		}
		else
		{// DirectXの処理
			m_dwCurrentTime = timeGetTime(); // 現在時刻を取得

			if ((m_dwCurrentTime - m_dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				m_nFPSCount     = (m_dwFrameCount * 1000) / (m_dwCurrentTime - m_dwFPSLastTime);
				m_dwFPSLastTime = m_dwCurrentTime;	// FPSを測定した時刻を保存
				m_dwFrameCount  = 0;						// フレームカウントをクリア
			}

			// 60/1秒経過していない時は抜ける
			if ((m_dwCurrentTime - m_dwExecLastTime) <= (1000 / 60))
				return true;

			m_dwExecLastTime = m_dwCurrentTime;	// 処理開始時刻(現在時刻)を保存
			m_dwFrameCount++;							// フレームカウントを加算
		}
	}

	switch (signal) {
	case RNLib::SIGNAL::NONE       :signal = RNLib::SIGNAL::LOAD       ; break;
	case RNLib::SIGNAL::INIT       :signal = RNLib::SIGNAL::UPDATE_WAIT; break;
	case RNLib::SIGNAL::UNINIT     :signal = RNLib::SIGNAL::END        ; break;
	case RNLib::SIGNAL::UPDATE     :signal = RNLib::SIGNAL::DRAW       ; break;
	case RNLib::SIGNAL::UPDATE_WAIT:signal = RNLib::SIGNAL::UPDATE     ; break;
	case RNLib::SIGNAL::DRAW       :signal = RNLib::SIGNAL::UPDATE_WAIT; break;
	case RNLib::SIGNAL::LOAD       :signal = RNLib::SIGNAL::INIT       ; break;
	case RNLib::SIGNAL::SAVE       :signal = RNLib::SIGNAL::UNINIT     ; break;
	}

	switch (signal) {
	case RNLib::SIGNAL::INIT:
		Init(instanceHandle);
		break;
	case RNLib::SIGNAL::UNINIT:
		Uninit();
		break;
	case RNLib::SIGNAL::UPDATE:
		Update();
		isMessageLoop = false;
		break;
	case RNLib::SIGNAL::UPDATE_WAIT:
		isMessageLoop = true;
		break;
	case RNLib::SIGNAL::DRAW:
		Draw();
		break;
	case RNLib::SIGNAL::LOAD:
		Load();
		break;
	case RNLib::SIGNAL::SAVE:
		Save();
		break;
	case RNLib::SIGNAL::END:
		return false;
	}

	return true;
}

//========================================
// シーン終了処理
// Author:RIKU NISHIMURA
//========================================
void RNLib::UninitScene(void) {
	m_sound   .Stop();
	m_light3D .SetCol(INITCOLOR);
	m_camera  .SetFixed(false);
	m_bSceneSwap = true;
	m_bSpace3DStopRsrv = false;
}

//========================================
// ウィンドウプロシージャ
// Author:RIKU NISHIMURA
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;

	case WM_KEYDOWN: {

		switch (wParam) {
		case VK_ESCAPE:
			if (MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO) == IDYES)
				DestroyWindow(hWnd);
			break;
		}
	}break;

	case WM_CLOSE: {

		if (MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO) == IDYES)
			DestroyWindow(hWnd);
		else
			return 0;	// (※0を返さないと終了してしまう)
	}break;
	case WM_MOUSEWHEEL: {

		// マウスホイールの前回転/後回転 状態設定
		if (HIWORD(wParam) == WHEEL_DELTA)
			m_input.SetWheelSpin(CInput::WHEELSPIN::FRONT);
		else
			m_input.SetWheelSpin(CInput::WHEELSPIN::BACK);
	}break;
	case WM_LBUTTONDOWN: {

		SetFocus(hWnd); // マウスを左クリックしたウインドウにフォーカスを合わせ、アクティブにする
	}break;
	}

	// 既定の処理を返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 無名名前空間の関数
//----------|---------------------------------------------------------------------
//================================================================================
namespace {
	//========================================
	// 初期化処理
	// Author:RIKU NISHIMURA
	//========================================
	void Init(HINSTANCE hInstance) {
		// 終了後にメモリリークを出力
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		srand((unsigned int)time(0));	// 乱数の種を取得
		timeBeginPeriod(1);				// 分解能を設定
	
		m_dwCurrentTime    = 0;				// 現在時刻を初期化
		m_dwExecLastTime   = timeGetTime();	// 現在時刻を取得(保存)
		m_dwFrameCount     = 0;				// フレームカウントを初期化
		m_dwFPSLastTime    = timeGetTime();	// 現在時刻を取得(保存)
		m_nCount           = 0;
		m_bBlinkF2         = false;
		m_bBlinkF4         = false;
		m_bSpace3DStop     = false;
		m_bSpace3DStopRsrv = false;
		m_bSceneSwap       = false;

		// デバイスを取得
		LPDIRECT3DDEVICE9 device = m_window.GetD3DDevice();

		// RNオブジェクト
		m_3DObject   .Init();
		m_calculation.Init();
		m_other      .Init();

		m_drawMng  .Init();
		m_drawState.Init(device);
		m_light3D  .Init();
		m_camera   .Init();
		m_input    .Init(hInstance);
		m_sound    .Init();
	
		//;;
		InitSetting();
	}
	
	//========================================
	// 終了処理
	// Author:RIKU NISHIMURA
	//========================================
	void Uninit(void) {

		// RNオブジェクト
		m_3DObject   .Uninit();
		m_calculation.Uninit();
		m_other      .Uninit();

		m_drawMng    .Uninit();
		m_drawState  .Uninit();
		m_light3D    .Uninit();
		m_camera     .Uninit();
		m_text       .Uninit();
		m_input      .Uninit();
		m_window     .Uninit();
	
		//;;
		UninitSetting();
		UninitPrint();
	
		m_sound  .Uninit();

		// 設定情報の解放処理
		RNSettings::Release();

		// 分解能を戻す
		timeEndPeriod(1);
	}
	
	//========================================
	// 更新処理
	// Author:RIKU NISHIMURA
	//========================================
	void Update(void) {
		m_bSpace3DStop = m_bSpace3DStopRsrv;

		// カウント加算
		m_nCount   = (m_nCount + 1) % INT_MAX;
		m_bBlinkF2 = (m_nCount % 4 < 2);
		m_bBlinkF4 = (m_nCount % 8 < 4);

		// 全オブジェクトマネージャーの更新処理
		CObjectMgr::UpdateAllMgrs();

		// RNオブジェクト
		m_3DObject   .Update();
		m_calculation.Update();
		m_other      .Update();

		// デバッグログをクリア
		m_text2D.ClearDebugLog();

		m_input   .Update();
		m_light3D .Update();

		if (!m_bSpace3DStop) {
			m_camera.Update();
		}

		//;;
		UpdateSetting();
	
		m_window.Update();
		
		m_input.SetWheelSpin(CInput::WHEELSPIN::NONE);
	}

	//========================================
	// 描画処理
	// Author:RIKU NISHIMURA
	//========================================
	void Draw(void) {

		// どうしても遷移は一番描画優先度を高くしたい。暫定対応
		m_transition.Update();

		// デバイスを取得
		LPDIRECT3DDEVICE9 device = m_window.GetD3DDevice();

		m_polygon3D.Put(CMatrix::ConvPosRotToMtx(D3DXVECTOR3(0.0f, 0.0f, 43.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)), true)
			->SetSize(m_window.GetWidth() * 0.05f, m_window.GetHeight() * 0.05f)
			->SetTex_Camera(&m_camera)
			->SetLighting(false);

		// 描画開始
		if (SUCCEEDED(device->BeginScene())) {

			if (m_drawMng.StartDraw()) {

				{// [[[ スクリーンオブジェクト描画 ]]]
					// スクリーンレンダリング開始
					CCamera::StartRenderingScreen(device);

					// 描画
					m_drawMng.Draw(device, true);
				}// <<< >>>

				{// [[[ カメラオブジェクト描画 ]]]
					// 現在のレンダリングターゲットとZバッファを保存
					LPDIRECT3DSURFACE9 renderDef, ZBuffDef;
					D3DVIEWPORT9 viewPortDef;
					device->GetRenderTarget       (0, &renderDef);
					device->GetDepthStencilSurface(&ZBuffDef);
					device->GetViewport           (&viewPortDef);

					// レンダリング開始
					m_camera.StartRendering(device);

					// 描画
					m_drawMng.Draw(device, false);

					// レンダリング終了
					m_camera.EndRendering(device);

					// レンダリングターゲットとZバッファを元に戻す
					device->SetRenderTarget       (0, renderDef);
					device->SetDepthStencilSurface(ZBuffDef);
					device->SetViewport           (&viewPortDef);
				}// <<< >>>

				// 描画終了
				device->EndScene();

				// バックバッファをフロントバッファと入れ替え
				device->Present(NULL, NULL, NULL, NULL);
			}
			else {
				// 描画終了
				device->EndScene();
			}
		}
	
		// 設定リセット
		if ((!m_bSpace3DStop && !m_bSpace3DStopRsrv) || m_bSceneSwap) {
			m_bSceneSwap = false;
		}
	}

	//========================================
	// 読み込み処理
	// Author:RIKU NISHIMURA
	//========================================
	void Load(void) {
		m_text.LoadFont();
	}

	//========================================
	// 書き出し処理
	// Author:RIKU NISHIMURA
	//========================================
	void Save(void) {

	}
}