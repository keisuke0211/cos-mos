//========================================
// 
// カメラの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//****************************************
// マクロ定義
//****************************************
#define VIB_DAMPING                    (0.75f)
#define VIB_MIN	                       (0.01f)
#define CONTROLLER_VIB_TIME_DIAMETER   (5)
#define CONTROLLER_VIB_MAX    	       (4.0f)
#define INITT_RANS_RATE                (0.25f)
#define INITT_RANS_RATE_OPP            (0.75f)
#define SPIN_DAMP                      (0.15f)
#define ROT_X_MAX                      (D3DX_PI * 0.499999f)
#define ROT_X_MIN                      (D3DX_PI * -0.499999f)
#define INIT_RADIAN                    (45.0f)
#define FEEDBACK_POLYGON_COLOR         (Color{255,255,255,127})
#define FEEDBACK_POLYGON_TEX_CUT_SCALE (0.99f)

//****************************************
// 定数定義
//****************************************
namespace {
	// 振動力
	const float VIB_POWERS[] = { 2.0f, 16.0f, 32.0f, };
}
CHECK_ARRAY_SIZE(VIB_POWERS, (int)CCamera::VIB_POWER::MAX);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CCamera3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CCamera::CCamera() {
	m_posV          = INITD3DXVECTOR3;
	m_posR          = INITD3DXVECTOR3;
	m_posVib        = INITD3DXVECTOR3;
	m_rot           = INITD3DXVECTOR3;
	m_spin          = INITD3DXVECTOR3;
	m_dist          = 0.0f;
	m_radian        = INIT_RADIAN;
	m_radianGoal    = INIT_RADIAN;
	m_vibForce      = 0.0f;
	m_isPivotToPosV = true;
	m_isFixed       = false;
	m_state         = STATE::NONE;
	m_stateInfo     = NULL;
	for (int cnt = 0; cnt < 2; cnt++) {
		m_texturesMT[cnt] = NULL;
		m_rendersMT [cnt] = NULL;
	}
	m_ZBuffMT       = NULL;
	m_viewportMT    = INITD3DVIEWPORT9;
	m_vtxBuffMT     = NULL;
}

//========================================
// デストラクタ
//========================================
CCamera::~CCamera() {

}

//========================================
// 初期化処理
//========================================
void CCamera::Init(void) {

	// デバイスを取得
	LPDIRECT3DDEVICE9 device = RNLib::Window().GetD3DDevice();

	 //----------------------------------------
	 // レンダリング設定
	{//----------------------------------------
		float areaWidth  = RNLib::Window().GetAreaWidth();
		float areaHeight = RNLib::Window().GetAreaHeight();

		for (int cnt = 0; cnt < 2; cnt++) {

			// レンダリングターゲット用テクスチャの生成
			device->CreateTexture(
				areaWidth,
				areaHeight,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&m_texturesMT[cnt],
				NULL);

			// テクスチャレンダリング用インターフェースの生成
			m_texturesMT[cnt]->GetSurfaceLevel(0, &m_rendersMT[cnt]);
		}

		// テクスチャレンダリング用Zバッファの生成
		device->CreateDepthStencilSurface(
			areaWidth,
			areaHeight,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_ZBuffMT,
			NULL);

		// ビューポートの設定
		m_viewportMT.X = 0;
		m_viewportMT.Y = 0;
		m_viewportMT.Width  = areaWidth;
		m_viewportMT.Height = areaHeight;
		m_viewportMT.MinZ = 0.0f;
		m_viewportMT.MaxZ = 1.0f;
	}

	//----------------------------------------
	// 頂点バッファの設定
	//----------------------------------------
	// 頂点バッファの生成
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuffMT, NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	Vertex2D* vtxs;
	m_vtxBuffMT->Lock(0, 0, (void**)&vtxs, 0);

	// [[[ 頂点位置の設定 ]]]
	RNLib::Polygon2D().SetVtxPos_TopLeft(vtxs, INITD3DXVECTOR3, RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ 頂点カラーの設定 ]]]
	RNLib::Polygon2D().SetVtxCol(vtxs, FEEDBACK_POLYGON_COLOR);

	// [[[ テクスチャ座標の設定 ]]]
	RNLib::Polygon2D().SetVtxTex_Cut(vtxs, D3DXVECTOR2(0.5f, 0.5f), FEEDBACK_POLYGON_TEX_CUT_SCALE);

	// [[[ RHWの設定 ]]]
	RNLib::Polygon2D().SetVtxRHW(vtxs);

	// 頂点座標をアンロックする
	m_vtxBuffMT->Unlock();
}

//========================================
// 終了処理
//========================================
void CCamera::Uninit(void) {

	// 状態終了処理
	ProcessState(PROCESS::UNINIT);

	// レンダリングターゲット用テクスチャ解放
	for (int cnt = 0; cnt < 2; cnt++) {
		if (m_texturesMT[cnt] != NULL) {
			m_texturesMT[cnt]->Release();
			m_texturesMT[cnt] = NULL;
		}
	}

	// フィードバック用ポリゴンの頂点バッファの破棄
	if (m_vtxBuffMT != NULL) {
		m_vtxBuffMT->Release();
		m_vtxBuffMT = NULL;
	}
}

//========================================
// 更新処理
//========================================
void CCamera::Update(void) {

	//----------------------------------------
	// 振動
	//----------------------------------------
	if (m_vibForce >= VIB_MIN) {
		if (RNLib::GetBlinkF2()) {
			float fAngle = -D3DX_PI + fRand() * D3DX_PI_DOUBLE;
			m_posVib.x = sinf(fAngle) * m_vibForce;
			m_posVib.y = cosf(fAngle) * m_vibForce;
			m_vibForce *= VIB_DAMPING;
		}
	}
	else {
		m_posVib = INITD3DXVECTOR3;
	}

	//----------------------------------------
	// 固定なら終了
	//----------------------------------------
	if (m_isFixed)
		return;

	//----------------------------------------
	// 状態更新処理
	//----------------------------------------
	ProcessState(PROCESS::UPDATE);

	//----------------------------------------
	// 回転 & 回転量減衰
	//----------------------------------------
	m_rot += m_spin;
	m_spin += -m_spin * SPIN_DAMP;

	//----------------------------------------
	// 向きを制御
	//----------------------------------------
	FloatControl(&m_rot.x, ROT_X_MAX, ROT_X_MIN);
	FloatLoopControl(&m_rot.y, D3DX_PI, -D3DX_PI);

	//----------------------------------------
	// 視点/注視点位置を算出
	//----------------------------------------
	// 回転軸が視点   > 注視点位置を算出
	// 回転軸が注視点 > 視点位置  を算出
	if (m_isPivotToPosV) m_posR = m_posV + CGeometry::FindRotVec(m_rot) * m_dist;
	else                 m_posV = m_posR - CGeometry::FindRotVec(m_rot) * m_dist;

	//----------------------------------------
	// ラジアン推移
	//----------------------------------------
	m_radian = (m_radian * INITT_RANS_RATE_OPP) + (m_radianGoal * INITT_RANS_RATE);
}

//========================================
// スクリーンのレンダリング開始処理
//========================================
void CCamera::StartRenderingScreen(LPDIRECT3DDEVICE9 device) {

	//----------------------------------------
	// 画面をクリア
	//----------------------------------------
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

	//----------------------------------------
	// ビューマトリックスの作成
	//----------------------------------------
	D3DXMATRIX mtxView = INITD3DXMATRIX; {
		D3DXVECTOR3 posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVECTOR3 posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}

	//----------------------------------------
	// ビューマトリックスの設定
	//----------------------------------------
	device->SetTransform(D3DTS_VIEW, &mtxView);

	//----------------------------------------
	// プロジェクションマトリックスのセットアップ
	//----------------------------------------
	SetUpProjectionMatrix(device, INIT_RADIAN);
}

//========================================
// レンダリング開始処理
//========================================
void CCamera::StartRendering(LPDIRECT3DDEVICE9 device) {

	//----------------------------------------
	// レンダリング設定
	//----------------------------------------
	// レンダリングターゲットを生成したテクスチャに設定
	device->SetRenderTarget(0, m_rendersMT[0]);

	// Zバッファを生成したZバッファに設定
	device->SetDepthStencilSurface(m_ZBuffMT);

	// ビューポートを設定
	device->SetViewport(&m_viewportMT);

	//----------------------------------------
	// 画面をクリア
	//----------------------------------------
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DXCOLOR(GetColorInfo()->bgCol), 1.0f, 0);

	//----------------------------------------
	// ビューマトリックスの作成
	//----------------------------------------
	D3DXMATRIX mtxView = INITD3DXMATRIX; {
		D3DXVECTOR3 posV = m_posV;
		D3DXVECTOR3 posR = m_posR;

		// 振動位置に変動がある時、
		if (m_posVib != INITD3DXVECTOR3) {
			float fAngle = CGeometry::FindAngleXZ(m_posV, m_posR) - D3DX_PI_HALF;
			D3DXVECTOR3 addPos;
			addPos.x = sinf(fAngle) * m_posVib.x;
			addPos.z = cosf(fAngle) * m_posVib.x;
			addPos.y = m_posVib.y;

			// 視点/注視点に加算位置を適用
			posV += addPos;
			posR += addPos;
		}

		// ビューマトリックスの作成
		D3DXVECTOR3 posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}
	
	//----------------------------------------
	// ビューマトリックスの設定
	//----------------------------------------
	device->SetTransform(D3DTS_VIEW, &mtxView);

	//----------------------------------------
	// プロジェクションマトリックスのセットアップ
	//----------------------------------------
	SetUpProjectionMatrix(device, m_radian);
}

//========================================
// スクリーンのレンダリング終了処理
//========================================
void CCamera::EndRendering(LPDIRECT3DDEVICE9 device) {

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	if (RNLib::Input().GetKeyPress(DIK_F)) {

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_vtxBuffMT, 0, sizeof(Vertex2D));

		// テクスチャの設定
		pDevice->SetTexture(0, m_texturesMT[1]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// テクスチャ0と1を入れ替える
	LPDIRECT3DTEXTURE9 m_texturesWk = m_texturesMT[0];
	m_texturesMT[0] = m_texturesMT[1];
	m_texturesMT[1] = m_texturesWk;

	LPDIRECT3DSURFACE9 m_rendersWk = m_rendersMT[0];
	m_rendersMT[0] = m_rendersMT[1];
	m_rendersMT[1] = m_rendersWk;
}

//========================================
// プロジェクションマトリックスのセットアップ
//========================================
void CCamera::SetUpProjectionMatrix(LPDIRECT3DDEVICE9 device, const float radian) {

	D3DXMATRIX mtxProjection = INITD3DXMATRIX;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(radian), RNLib::Window().GetWidth() / RNLib::Window().GetHeight(), 10.0f, 4000.0f);

	device->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//========================================
// 物理情報を設定
//========================================
void CCamera::SetGeometryInfo(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR) {

	m_posV = posV;
	m_posR = posR;
	m_dist = CGeometry::FindDistance(m_posV, m_posR);

	if (m_isPivotToPosV)
		m_rot = CGeometry::FindRot(m_posV, m_posR);
	else
		m_rot = CGeometry::FindRot(m_posR, m_posV);
}

//========================================
// 振動を設定
//========================================
void CCamera::SetVib(const VIB_POWER vibPower) {

	if (m_vibForce > VIB_POWERS[(int)vibPower])
		return;

	// 振動の強さを設定
	m_vibForce = VIB_POWERS[(int)vibPower];

	// コントローラーに振動を伝える
	RNLib::Input().SetVibration(
		m_vibForce / CONTROLLER_VIB_MAX,
		m_vibForce * CONTROLLER_VIB_TIME_DIAMETER);
}