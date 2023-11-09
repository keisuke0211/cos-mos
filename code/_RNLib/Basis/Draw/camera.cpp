//========================================
// 
// カメラの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//****************************************
// マクロ定義
//****************************************
#define VIB_DAMPING                    (0.75f)
#define VIB_MIN	                       (0.01f)
#define CONTROLLER_VIB_TIME_DIAMETER   (5)
#define CONTROLLER_VIB_MAX    	       (4.0f)
#define INIT_RANS_RATE                 (0.25f)
#define INIT_RANS_RATE_OPP             (0.75f)
#define SPIN_DAMP                      (0.15f)
#define ROT_X_MAX                      (D3DX_PI * 0.499999f)
#define ROT_X_MIN                      (D3DX_PI * -0.499999f)
#define INIT_RADIAN                    (45.0f)
#define FEEDBACK_POLYGON_COLOR         (Color{255,255,255,127})
#define FEEDBACK_POLYGON_TEX_CUT_SCALE (0.99f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]カメラクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
short CCamera::ms_IDCount = 0;

//========================================
// コンストラクタ
//========================================
CCamera::CCamera(const Scale2D& scale2D) {

	// リストに追加
	RNLib::CameraMgr().AddList(this);

	// IDを設定
	m_ID       = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % SHRT_MAX;

	// 変数を初期化
	m_posV          = INITPOS3D;
	m_posR          = INITPOS3D;
	m_posVib        = INITPOS3D;
	m_rot           = INITROT3D;
	m_spin          = INITVECTOR3D;
	m_dist          = 1.0f;
	m_radian        = INIT_RADIAN;
	m_radianGoal    = INIT_RADIAN;
	m_vibForce      = 0.0f;
	m_isPivotToPosV = true;
	m_isFixed       = false;
	m_isClipping    = false;
	m_state         = STATE::NONE;
	m_stateInfo     = NULL;
	m_BGCol         = Color{0,0,0,0};

	// デバイスを取得
	Device device = RNLib::Window().GetD3DDevice();

	// [[[ 領域サイズを算出 ]]]
	float areaWidth;
	float areaHeight;
	{
		const float resolution = RNLib::Window().GetResolution();
		areaWidth  = scale2D.x * resolution;
		areaHeight = scale2D.y * resolution;
	}

	{// [[[ レンダリング設定 ]]]
		for (int cnt = 0; cnt < 2; cnt++) {

			// レンダリングターゲット用テクスチャの生成
			device->CreateTexture(areaWidth, areaHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_MTInfo.textures[cnt], NULL);

			// テクスチャレンダリング用インターフェースの生成
			m_MTInfo.textures[cnt]->GetSurfaceLevel(0, &m_MTInfo.renders[cnt]);
		}

		// テクスチャレンダリング用Zバッファの生成
		device->CreateDepthStencilSurface(areaWidth, areaHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_MTInfo.ZBuff, NULL);

		// ビューポートの設定
		m_MTInfo.viewport.X      = 0;
		m_MTInfo.viewport.Y      = 0;
		m_MTInfo.viewport.Width  = areaWidth;
		m_MTInfo.viewport.Height = areaHeight;
		m_MTInfo.viewport.MinZ   = 0.0f;
		m_MTInfo.viewport.MaxZ   = 1.0f;
	}

	//----------------------------------------
	// 頂点バッファの設定
	//----------------------------------------
	// 頂点バッファの生成
	device->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_MTInfo.vtxBuff, NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	Vertex2D* vtxs;
	m_MTInfo.vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

	// [[[ 頂点位置の設定 ]]]
	RNLib::Polygon2D().SetVtxPos_TopLeft(vtxs, INITPOS2D, scale2D.x, scale2D.y);

	// [[[ 頂点カラーの設定 ]]]
	RNLib::Polygon2D().SetVtxCol(vtxs, FEEDBACK_POLYGON_COLOR);

	// [[[ テクスチャ座標の設定 ]]]
	RNLib::Polygon2D().SetVtxTex_Cut(vtxs, Pos2D(0.5f, 0.5f), FEEDBACK_POLYGON_TEX_CUT_SCALE);

	// [[[ RHWの設定 ]]]
	RNLib::Polygon2D().SetVtxRHW(vtxs);

	// 頂点座標をアンロックする
	m_MTInfo.vtxBuff->Unlock();
}

//========================================
// デストラクタ
//========================================
CCamera::~CCamera() {

	// リストから削除
	RNLib::CameraMgr().SubList(this);

	// 状態終了処理
	ProcessState(PROCESS::UNINIT);

	// レンダリングターゲット用テクスチャ解放
	for (int cnt = 0; cnt < 2; cnt++) {
		if (m_MTInfo.textures[cnt] != NULL) {
			m_MTInfo.textures[cnt]->Release();
			m_MTInfo.textures[cnt] = NULL;
		}
	}

	// フィードバック用ポリゴンの頂点バッファの破棄
	if (m_MTInfo.vtxBuff != NULL) {
		m_MTInfo.vtxBuff->Release();
		m_MTInfo.vtxBuff = NULL;
	}
}

//========================================
// 更新処理
//========================================
void CCamera::Update(void) {

	// [[[ 振動 ]]]
	if (m_vibForce >= VIB_MIN) {
		if (RNLib::Count().GetBlinkF2()) {
			float fAngle = -D3DX_PI + fRand() * D3DX_PI_DOUBLE;
			m_posVib.x = sinf(fAngle) * m_vibForce;
			m_posVib.y = cosf(fAngle) * m_vibForce;
			m_vibForce *= VIB_DAMPING;
		}
	}
	else {
		m_posVib = INITPOS3D;
	}

	// [[[ 固定なら終了 ]]]
	if (m_isFixed)
		return;

	// [[[ 状態更新処理 ]]]
	ProcessState(PROCESS::UPDATE);

	// [[[ 回転 & 回転量減衰 ]]]
	m_rot  += m_spin;
	m_spin += -m_spin * SPIN_DAMP;

	// [[[ 向きを制御 ]]]
	FloatControl(&m_rot.x, ROT_X_MAX, ROT_X_MIN);
	FloatLoopControl(&m_rot.y, D3DX_PI, -D3DX_PI);

	// [[[ 視点/注視点位置を算出 ]]]
	// 回転軸が視点   > 注視点位置を算出
	// 回転軸が注視点 > 視点位置  を算出
	m_isPivotToPosV ?
		m_posV + CGeometry::FindRotVec(m_rot) * m_dist :
		m_posV = m_posR - CGeometry::FindRotVec(m_rot) * m_dist;

	// [[[ ラジアン推移 ]]]
	m_radian = (m_radian * INIT_RANS_RATE_OPP) + (m_radianGoal * INIT_RANS_RATE);
}

//========================================
// [静的]スクリーンのレンダリング開始処理
//========================================
void CCamera::StartRenderingScreen(Device& device) {

	// [[[ 画面をクリア ]]]
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

	// [[[ ビューマトリックスの作成 ]]]
	Matrix mtxView = INITMatrix; {
		Pos3D posV = Pos3D(0.0f, 0.0f, 0.0f);
		Pos3D posR = Pos3D(0.0f, 0.0f, 1.0f);
		Pos3D posU = Pos3D(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}

	// [[[ ビューマトリックスの設定 ]]]
	device->SetTransform(D3DTS_VIEW, &mtxView);

	// [[[ プロジェクションマトリックスのセットアップ ]]]
	SetUpProjectionMatrix(device, Scale2D(RNLib::Window().GetAreaWidth(), RNLib::Window().GetAreaHeight()), INIT_RADIAN);
}

//========================================
// レンダリング開始処理
//========================================
void CCamera::StartRendering(Device& device) {

	// [[[ レンダリング設定 ]]]
	// レンダリングターゲットを生成したテクスチャに設定
	device->SetRenderTarget(0, m_MTInfo.renders[0]);

	// Zバッファを生成したZバッファに設定
	device->SetDepthStencilSurface(m_MTInfo.ZBuff);

	// ビューポートを設定
	device->SetViewport(&m_MTInfo.viewport);

	// [[[ 画面をクリア ]]]
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DXCOLOR(m_BGCol), 1.0f, 0);

	// [[[ ビューマトリックスの作成 ]]]
	Matrix mtxView = INITMatrix; {
		Pos3D posV = m_posV;
		Pos3D posR = m_posR;

		// 振動位置に変動がある時、
		if (m_posVib != INITD3DXVECTOR3) {
			float angle = CGeometry::FindAngleXZ(m_posV, m_posR) - D3DX_PI_HALF;
			Pos3D addPos;
			addPos.x = sinf(angle) * m_posVib.x;
			addPos.z = cosf(angle) * m_posVib.x;
			addPos.y = m_posVib.y;

			// 視点/注視点に加算位置を適用
			posV += addPos;
			posR += addPos;
		}

		// ビューマトリックスの作成
		Pos3D posU = Pos3D(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}
	
	// [[[ ビューマトリックスの設定 ]]]
	device->SetTransform(D3DTS_VIEW, &mtxView);

	// [[[ プロジェクションマトリックスのセットアップ ]]]
	SetUpProjectionMatrix(device, Scale2D(m_MTInfo.viewport.Width, m_MTInfo.viewport.Height), m_radian);
}

//========================================
// スクリーンのレンダリング終了処理
//========================================
void CCamera::EndRendering(Device& device) {

	if (RNLib::Input().GetKeyPress(DIK_F)) {

		// 頂点フォーマットの設定
		device->SetFVF(FVF_VERTEX_2D);

		// 頂点バッファをデータストリームに設定
		device->SetStreamSource(0, m_MTInfo.vtxBuff, 0, sizeof(Vertex2D));

		// テクスチャの設定
		device->SetTexture(0, m_MTInfo.textures[1]);

		// ポリゴンの描画
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// テクスチャ0と1を入れ替える
	Texture m_texturesWk = m_MTInfo.textures[0];
	m_MTInfo.textures[0] = m_MTInfo.textures[1];
	m_MTInfo.textures[1] = m_texturesWk;

	Surface m_rendersWk = m_MTInfo.renders[0];
	m_MTInfo.renders[0] = m_MTInfo.renders[1];
	m_MTInfo.renders[1] = m_rendersWk;
}

//========================================
// 物理情報を設定
//========================================
void CCamera::SetPosVAndPosR(const Pos3D& posV, const Pos3D& posR) {

	m_posV = posV;
	m_posR = posR;
	m_dist = CGeometry::FindDistance(m_posV, m_posR);
	m_rot  = m_isPivotToPosV ? CGeometry::FindRot(m_posV, m_posR) : CGeometry::FindRot(m_posR, m_posV);
}

//========================================
// 振動を設定
//========================================
void CCamera::SetVib(const float& vibPower) {

	// 現在の振動以下の時、終了
	if (m_vibForce >= vibPower)
		return;

	// 振動の強さを設定
	m_vibForce = vibPower;

	// コントローラーに振動を伝える
	RNLib::Input().SetVibration(
		m_vibForce / CONTROLLER_VIB_MAX,
		m_vibForce * CONTROLLER_VIB_TIME_DIAMETER);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]カメラクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]プロジェクションマトリックスのセットアップ
//========================================
void CCamera::SetUpProjectionMatrix(Device& device, const Scale2D& scale, const float& radian) {

	Matrix mtxProjection = INITMatrix;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(radian), scale.x / scale.y, 10.0f, 4000.0f);

	device->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}