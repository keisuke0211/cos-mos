//========================================
// 
// RNライブラリの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNmode.h"
#include "RNobject.h"
#include "RNsettings.h"
#include "Demo/demo.h"
#include "SetUp3DEditor/setup3D-editor.h"

//****************************************
// プロトタイプ宣言
//****************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//****************************************
// 無名空間
//****************************************
namespace {

	//========== [[[ 関数宣言 ]]]
	void Init(HINSTANCE& instanceHandle, const char* settingsPath, const UShort& priorityMax, const RNSystem::MODE& mode);
	void Uninit(void);
	void Update(void);
	void UpdateBeforeDraw(void);
	void Draw(void);
	void Draw2(void);

	//========== [[[ 変数宣言 ]]]
	RNSystem::SIGNAL signal = RNSystem::SIGNAL::NONE;
	RNSystem::MODE   nowMode;
	DWORD            execLastTime;
	DWORD            lastTime;
	DWORD            FPSCount;
	int              FPS;
	DWORD            drawFPSCount;
	int              drawFPS;
	bool             isSpace3DStop;
	bool             isSpace3DStopReserve;
	bool             isSceneSwap;
	bool             isBeginScene;
	bool             isStartDraw;

	// RNオブジェクト
	C3DObject    _3DObject;
	CCalculation calculation;
	CDraw        draw;
	CMechanical  mechanical;
	COther       other;
	void*        modeObject;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNライブラリ
//----------|---------------------------------------------------------------------
//================================================================================
CDoll3DMgr&   RNLib::Doll3DMgr   (void) { return _3DObject.m_doll3DMgr;  }
CMotion3D&    RNLib::Motion3D    (void) { return _3DObject.m_motion3D;   }
CSetUp3D&     RNLib::SetUp3D     (void) { return _3DObject.m_setUp3D;    }
CEase&        RNLib::Ease        (void) { return calculation.m_ease;     }
CGeometry&    RNLib::Geometry    (void) { return calculation.m_geometry; }
CHitTest&     RNLib::HitTest     (void) { return calculation.m_hitTest;  }
CMatrix&      RNLib::Matrix      (void) { return calculation.m_matrix;   }
CModel&       RNLib::Model       (void) { return draw.m_model;           }
CPolygon2D&   RNLib::Polygon2D   (void) { return draw.m_polygon2D;       }
CPolygon3D&   RNLib::Polygon3D   (void) { return draw.m_polygon3D;       }
CText2D&      RNLib::Text2D      (void) { return draw.m_text2D;          }
CText3D&      RNLib::Text3D      (void) { return draw.m_text3D;          }
CCameraMgr&   RNLib::CameraMgr   (void) { return draw.m_cameraMgr;       }
CDrawMgr&     RNLib::DrawMgr     (void) { return draw.m_drawMgr;         }
CDrawState&   RNLib::DrawStateMgr(void) { return draw.m_drawState;       }
CLight3D&     RNLib::Light3D     (void) { return draw.m_light3D;         }
CText&        RNLib::Text        (void) { return draw.m_text;            }
CTexture&     RNLib::Texture     (void) { return draw.m_texture;         }
CTransition&  RNLib::Transition  (void) { return draw.m_transition;      }
CCount&       RNLib::Count       (void) { return mechanical.m_count;     }
CFile&        RNLib::File        (void) { return mechanical.m_file;      }
CInput&       RNLib::Input       (void) { return mechanical.m_input;     }
CMemory&      RNLib::Memory      (void) { return mechanical.m_memory;    }
CSound&       RNLib::Sound       (void) { return mechanical.m_sound;     }
CWindow&      RNLib::Window      (void) { return mechanical.m_window;    }
CDefaultData& RNLib::DefaultData (void) { return other.m_defaultData;    }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNシステム
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 設定/取得系関数
//========================================
RNSystem::SIGNAL RNSystem::GetSignal      (void)               { return nowMode == RNSystem::MODE::EXECUTION ? signal : SIGNAL::NONE; }
int              RNSystem::GetFPS         (void)               { return FPS; }
int              RNSystem::GetDrawFPS     (void)               { return drawFPS; }
void             RNSystem::AddDrawFPSCount(void)               { drawFPSCount++; }
void             RNSystem::SetSpace3DStop (const bool& isStop) { isSpace3DStopReserve = isStop; }
bool             RNSystem::GetSpace3DStop (void)               { return isSpace3DStop; }
bool             RNSystem::GetSceneSwap   (void)               { return isSceneSwap; }

//========================================
// メインループ
//========================================
bool RNSystem::MainLoop(HINSTANCE& instanceHandle, const char* settingsPath, const UShort& priorityMax, const MODE& mode) {

	static bool isMessageLoop = false;
	static MSG  msg;

	// [[[ メッセージループ ]]]
	if (isMessageLoop) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) 
		{// Windowsの処理
			if (msg.message == WM_QUIT) {
				signal = SIGNAL::UNINIT_WAIT;
				isMessageLoop = false;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return true;
		}
		else {

			// 現在時刻を取得
			DWORD currentTime = timeGetTime();

			if ((currentTime - lastTime) >= 500)
			{// 0.5秒経過
				FPS          = (FPSCount     * 1000) / (currentTime - lastTime);
				FPSCount     = 0;
				drawFPS      = (drawFPSCount * 1000) / (currentTime - lastTime);
				drawFPSCount = 0;
				lastTime     = currentTime;	// FPS測定時刻を保存

				// 描画マネージャーの待ち時間をリセット
				draw.m_drawMgr.ResetWaitMilliseconds();
			}

			// 60/1秒経過していない時は抜ける
			if ((currentTime - execLastTime) <= (1000 / 60))
				return true;

			// 現在時刻を保存
			execLastTime = currentTime;
			FPSCount++;
		}
	}

	// [[[ 現在の信号に応じて、新たに信号を設定 ]]]
	switch (signal) {
	case RNSystem::SIGNAL::NONE       :signal = RNSystem::SIGNAL::INIT       ; break;
	case RNSystem::SIGNAL::INIT       :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	case RNSystem::SIGNAL::UNINIT     :signal = RNSystem::SIGNAL::END        ; break;
	case RNSystem::SIGNAL::UNINIT_WAIT:signal = RNSystem::SIGNAL::UNINIT     ; break;
	case RNSystem::SIGNAL::UPDATE     :signal = RNSystem::SIGNAL::DRAW       ; break;
	case RNSystem::SIGNAL::UPDATE_WAIT:signal = RNSystem::SIGNAL::UPDATE     ; break;
	case RNSystem::SIGNAL::DRAW       :signal = RNSystem::SIGNAL::DRAW2      ; break;
	case RNSystem::SIGNAL::DRAW2      :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	}

	// [[[ 信号に応じた処理 ]]]
	switch (signal) {
	case RNSystem::SIGNAL::INIT: {
		Init(instanceHandle, settingsPath, mode == MODE::EXECUTION ? priorityMax : (UShort)RNMode::PRIORITY::MAX, mode);
	}break;
	case RNSystem::SIGNAL::UNINIT: {
		Uninit();
	}break;
	case RNSystem::SIGNAL::UPDATE: {
		Update();
		isMessageLoop = false;
	}break;
	case RNSystem::SIGNAL::UPDATE_WAIT: {
		isMessageLoop = true;
	}break;
	case RNSystem::SIGNAL::DRAW: {
		UpdateBeforeDraw();
		Draw();
	}break;
	case RNSystem::SIGNAL::DRAW2: {
		Draw2();
	}break;
	case RNSystem::SIGNAL::END: {
		return false;
	}break;
	}

	return true;
}

//========================================
// シーン終了処理
//========================================
void RNSystem::EndScene(void) {

	isSceneSwap          = true;
	isSpace3DStopReserve = false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNデモの関数
//----------|---------------------------------------------------------------------
//================================================================================
CDemo& RNDemo::Get(void) { return nowMode == RNSystem::MODE::DEMO ? *(CDemo*)modeObject : *(CDemo*)nullptr; }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 無名空間の関数
//----------|---------------------------------------------------------------------
//================================================================================
namespace {
	//========================================
	// 初期処理
	//========================================
	void Init(HINSTANCE& instanceHandle, const char* settingsPath, const UShort& priorityMax, const RNSystem::MODE& mode) {

		// モードを保存
		nowMode = mode;

		// 終了後にメモリリークを出力
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		// 乱数の種を取得
		srand((unsigned int)time(0));

		// 分解能を設定
		timeBeginPeriod(1);
		
		// 変数を初期化
		execLastTime         = timeGetTime();
		FPSCount             = 0;
		lastTime             = timeGetTime();
		isSpace3DStop        = false;
		isSpace3DStopReserve = false;
		isSceneSwap          = false;
		isBeginScene         = false;
		isStartDraw          = false;

		// 設定ファイルを読み込み&書き出し
		if (RNSettings::LoadAndSave(settingsPath))
		{// 成功した時、
			// 設定情報を元にウィンドウを生成
			RNSettings::Info settingsInfo = RNSettings::GetInfo();
			mechanical.m_window.Create(
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
			signal = RNSystem::SIGNAL::UNINIT;
		}

		// RNオブジェクトの初期化
		_3DObject  .Init();
		calculation.Init();
		draw	   .Init(mechanical.m_window.GetD3DDevice(), priorityMax);
		mechanical .Init(instanceHandle);
		other	   .Init();

		modeObject = NULL;
		switch (mode) {
		case RNSystem::MODE::DEMO: {
			CMemory::Alloc((CDemo**)&modeObject);
			CDemo* demo = (CDemo*)modeObject;
			demo->Init();
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			CMemory::Alloc((CSetUp3DEditor**)&modeObject);
			CSetUp3DEditor* setup3DEditor = (CSetUp3DEditor*)modeObject;
			setup3DEditor->Init();
		}break;
		}
	}
	
	//========================================
	// 終了処理
	//========================================
	void Uninit(void) {

		// RNオブジェクトの解放
		_3DObject  .Uninit();
		calculation.Uninit();
		draw	   .Uninit();
		mechanical .Uninit();
		other	   .Uninit();

		switch (nowMode) {
		case RNSystem::MODE::DEMO: {
			CDemo* demo = (CDemo*)modeObject;
			demo->Uninit();
			CMemory::Release((CDemo**)&modeObject);
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			CSetUp3DEditor* setup3DEditor = (CSetUp3DEditor*)modeObject;
			setup3DEditor->Uninit();
			CMemory::Release((CSetUp3DEditor**)&modeObject);
		}break;
		}

		// プリントの終了処理
		UninitPrint();
		
		// 設定情報の解放処理
		RNSettings::Release();

		// 分解能を戻す
		timeEndPeriod(1);
	}
	
	//========================================
	// 更新処理
	//========================================
	void Update(void) {

		// 3D空間停止予約を適用する
		isSpace3DStop = isSpace3DStopReserve;

		switch (nowMode) {
		case RNSystem::MODE::DEMO: {
			CDemo* demo = (CDemo*)modeObject;
			demo->Update();
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			CSetUp3DEditor* setup3DEditor = (CSetUp3DEditor*)modeObject;
			setup3DEditor->Update();
		}break;
		}

		// 全オブジェクトマネージャーの更新処理
		CObjectMgr::UpdateAllMgrs();
	}

	//========================================
	// 描画前更新処理
	//========================================
	void UpdateBeforeDraw(void) {

		// 登録情報適用待ち開始
		draw.m_drawMgr.StartRegistInfoApplyWait();

		// RNオブジェクト
		_3DObject  .Update();
		calculation.Update();
		other	   .Update();
		draw	   .Update();
		mechanical .Update();

		// 設定の更新処理
		UpdateSetting();
	}

	//========================================
	// 描画処理
	//========================================
	void Draw(void) {

		// デバイスを取得
		Device device = mechanical.m_window.GetD3DDevice();

		isBeginScene = SUCCEEDED(device->BeginScene());
		if (isBeginScene)
		{// デバイスの描画開始成功

			isStartDraw = draw.m_drawMgr.StartDraw();
			if (isStartDraw) 
			{// 描画開始成功
				{// [[[ スクリーン描画 ]]]
					CCamera::StartRenderingScreen(device);
					draw.m_drawMgr.Draw(device, NONEDATA, false, true);
				}
			}
		}
	}

	//========================================
	// 描画処理2
	//========================================
	void Draw2(void) {

		// デバイスを取得
		Device device = mechanical.m_window.GetD3DDevice();

		if (isBeginScene) {
			if (isStartDraw) {
				{// [[[ カメラ描画 ]]]
					// レンダリングターゲット/Zバッファ/ビューポートを保存
					Surface  renderDef;
					Surface  ZBuffDef;
					Viewport viewPortDef;
					device->GetRenderTarget(0, &renderDef);
					device->GetDepthStencilSurface(&ZBuffDef);
					device->GetViewport(&viewPortDef);

					// カメラ1つ1つに対し描画していく
					CCamera* camera = NULL;
					while (draw.m_cameraMgr.ListLoop((CObject**)&camera)) {
						camera->StartRendering(device);
						draw.m_drawMgr.Draw(device, camera->GetID(), camera->GetClipping(), false);
						camera->EndRendering(device);
					}

					// レンダリングターゲット/Zバッファ/ビューポートを元に戻す
					device->SetRenderTarget(0, renderDef);
					device->SetDepthStencilSurface(ZBuffDef);
					device->SetViewport(&viewPortDef);
				}

				// 描画終了
				device->EndScene();

				// バックバッファをフロントバッファと入れ替え
				device->Present(NULL, NULL, NULL, NULL);
			}
			else
			{// 描画開始失敗
				// 描画終了
				device->EndScene();
			}
		}

		// シーン入れ替えフラグをリセット
		isSceneSwap = false;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ウィンドウプロシージャ
//----------|---------------------------------------------------------------------
//================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
		// [[[ 破棄 ]]]
	case WM_DESTROY: {
		
		PostQuitMessage(0);

	}break;
		// [[[ キー押下 ]]]
	case WM_KEYDOWN: {
		
		// エスケープキーだった時、終了確認
		if (wParam == VK_ESCAPE) {
			if (MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO) == IDYES)
				DestroyWindow(hWnd);
		}

	}break;
		// [[[ 閉じる ]]]
	case WM_CLOSE: {

		// 終了確認
		if (MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO) == IDYES) {
			DestroyWindow(hWnd);
		}
		else {
			// (※0を返さないと終了してしまう)
			return 0;
		}

	}break;
		// [[[ マウスホイールの回転 ]]]
	case WM_MOUSEWHEEL: {

		// マウスホイールの前回転/後回転 状態設定
		mechanical.m_input.SetWheelSpin(HIWORD(wParam) == WHEEL_DELTA ? CInput::WHEELSPIN::FRONT : CInput::WHEELSPIN::BACK);

	}break;
		// [[[ マウス左ボタン押下 ]]]
	case WM_LBUTTONDOWN: {

		// 左クリックしたウインドウにフォーカスを合わせ、アクティブ化
		SetFocus(hWnd);

	}break;
	}

	// 既定の処理を返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}