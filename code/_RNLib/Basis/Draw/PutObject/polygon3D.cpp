//========================================
// 
// ポリゴン3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ポリゴン3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CPolygon3D::CPolygon3D() {

}

//========================================
// デストラクタ
//========================================
CPolygon3D::~CPolygon3D() {

}

//========================================
// 初期化処理
//========================================
void CPolygon3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CPolygon3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CPolygon3D::Update(void) {

}

//========================================
// 設置処理
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::Put(const Matrix& mtx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMgr::GetProcessState() != CDrawMgr::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMgr().PutPolygon3D(mtx, isOnScreen);
}

//========================================
// 設置処理(位置と向き指定)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::Put(const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {

	return Put(CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 描画情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
VertexBuffer CPolygon3D::CDrawInfo::m_vtxBuff = NULL;
UShort CPolygon3D::CDrawInfo::m_allocPower = 0;
UShort CPolygon3D::CDrawInfo::m_allocNum   = 0;
UShort CPolygon3D::CDrawInfo::m_idxCount   = 0;

//========================================
// [静的] 頂点バッファ初期生成処理
//========================================
void CPolygon3D::CDrawInfo::InitCreateVertexBuffer(void) {

	m_allocPower = CDrawMgr::POLYGON3D_ALLOC_BASE_POWER;
	m_allocNum   = pow(2, m_allocPower);
	CreateVertexBuffer(m_allocNum);
}

//========================================
// [静的] 頂点バッファ生成処理
//========================================
void CPolygon3D::CDrawInfo::CreateVertexBuffer(const UShort& num) {

	// 頂点バッファの生成
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(
		sizeof(Vertex3D) * 4 * num,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_vtxBuff,
		NULL);
}

//========================================
// [静的] 頂点バッファ解放処理
//========================================
void CPolygon3D::CDrawInfo::ReleaseVertexBuffer(void) {

	// 頂点バッファの破棄
	if (m_vtxBuff != NULL) {
		m_vtxBuff->Release();
		m_vtxBuff = NULL;
	}
}

//========================================
// コンストラクタ
//========================================
CPolygon3D::CDrawInfo::CDrawInfo() {

	m_idx            = 0;
	m_mtx            = INITMATRIX;
	m_texIdx         = NONEDATA;
	m_texCamera      = NULL;
	m_isZTest        = true;
	m_isLighting     = true;
	m_isBillboard    = false;
	m_alphaBlendMode = CDrawState::ALPHA_BLEND_MODE::NORMAL;
	m_distance       = 0.0f;
	for (int cntVtx(0); cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// デストラクタ
//========================================
CPolygon3D::CDrawInfo::~CDrawInfo() {

}

//========================================
// 描画処理
//========================================
void CPolygon3D::CDrawInfo::Draw(Device& device, const Matrix& viewMtx) {

	// 頂点バッファがNULLの時、終了
	if (m_vtxBuff == NULL)
		return;

	//----------------------------------------
	// 事前準備
	//----------------------------------------
	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_3D);

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(Vertex3D));

	//----------------------------------------
	// 一時的な描画モード設定を開始
	//----------------------------------------
	RNLib::DrawStateMgr().StartTemporarySetMode();

	//----------------------------------------
	// パラメーターに応じた設定
	//----------------------------------------
	// [[[ Zテストを有効/無効にする ]]]
	RNLib::DrawStateMgr().SetZTestMode(m_isZTest, device);

	// [[[ ライティングを有効/無効にする ]]]
	RNLib::DrawStateMgr().SetLightingMode(m_isLighting, device);

	// [[[ 加算合成を有効/無効にする ]]]
	RNLib::DrawStateMgr().SetAlphaBlendMode(m_alphaBlendMode, device);

	{
		Matrix mtxTrans(INITMATRIX);	// 計算用マトリックス
		Matrix mtxSelf (INITMATRIX);	// 本体マトリックス
		
		// [[[ ビルボードフラグに応じて向きを設定 ]]]
		if (m_isBillboard) {

			// 位置マトリックスを設定
			const Pos3D setPos(CMatrix::ConvMtxToPos(m_mtx));
			D3DXMatrixTranslation(&mtxTrans, setPos.x, setPos.y, setPos.z);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&mtxSelf, NULL, &viewMtx);
			mtxSelf._41 = 0.0f;
			mtxSelf._42 = 0.0f;
			mtxSelf._43 = 0.0f;

			// 位置マトリックスを設定
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &mtxTrans);
		}
		else {
			// 位置と向きを反映
			D3DXMatrixMultiply(&mtxSelf, &mtxSelf, &m_mtx);
		}

		// [[[ ワールドマトリックスの設定 ]]]
		device->SetTransform(D3DTS_WORLD, &mtxSelf);
	}

	// [[[ テクスチャの設定 ]]]
	if (m_texCamera != NULL) {
		m_texCamera->SetTexture(device);
		RNLib::DrawStateMgr().SetTextureAlphaMode(false, device);	// テクスチャの透過を無効化
	}
	else {
		RNLib::Texture().Set(device, m_texIdx);
	}

	//----------------------------------------
	// 描画
	//----------------------------------------
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_idx, 2);

	//----------------------------------------
	// 一時的な描画モード設定を終了
	//----------------------------------------
	RNLib::DrawStateMgr().EndTemporarySetMode(device);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 登録情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CPolygon3D::CRegistInfo::CRegistInfo() {

	m_vtxPoses = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CPolygon3D::CRegistInfo::~CRegistInfo() {

	ClearParameter();
}

//========================================
// パラメーターのクリア処理
//========================================
void CPolygon3D::CRegistInfo::ClearParameter(void) {

	m_idx            = NONEDATA;
	m_scaleX         = 1.0f;
	m_scaleY         = 1.0f;
	m_isFactScale    = false;
	m_mtx            = INITMATRIX;
	CMemory::Release(&m_vtxPoses);
	m_col            = INITCOLOR;
	m_texIdx         = NONEDATA;
	m_texCamera      = NULL;
	m_ptn            = 0;
	m_ptnX           = 1;
	m_ptnY           = 1;
	m_ptnScaleX      = 1.0f;
	m_ptnScaleY      = 1.0f;
	m_ptnPos         = INITPOS2D;
	m_isZtest        = true;
	m_isLighting     = true;
	m_isBillboard    = false;
	m_alphaBlendMode = CDrawState::ALPHA_BLEND_MODE::NORMAL;
	m_isTexMirrorX   = false;
	m_priority       = 0;
}

//========================================
// 描画情報に変換
//========================================
CPolygon3D::CDrawInfo* CPolygon3D::CRegistInfo::ConvToDrawInfo(void) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// 情報を代入
	// (基底)
	drawInfo->m_type           = CDrawInfoBase::TYPE::POLYGON3D;
	drawInfo->m_priority       = m_priority;
	// (継承)
	drawInfo->m_idx            = m_idx;
	drawInfo->m_mtx            = m_mtx;
	drawInfo->m_texIdx         = m_texIdx;
	drawInfo->m_texCamera      = m_texCamera;
	drawInfo->m_isZTest        = m_isZtest;
	drawInfo->m_isLighting     = m_isLighting;
	drawInfo->m_isBillboard    = m_isBillboard;
	drawInfo->m_alphaBlendMode = m_alphaBlendMode;

	//----------------------------------------
	// 頂点情報の設定
	//----------------------------------------
	// [[[ 位置 ]]]
	if (m_vtxPoses == NULL) {
		float widthHalf;
		float heightHalf;

		if (m_isFactScale) {
			widthHalf  = m_scaleX * 0.5f;
			heightHalf = m_scaleY * 0.5f;
		}
		else {
			float width ((RNLib::Texture().GetWidth (m_texIdx) * PIXEL3D_SIZE) / m_ptnX);
			float height((RNLib::Texture().GetHeight(m_texIdx) * PIXEL3D_SIZE) / m_ptnY);
			widthHalf  = width  * m_scaleX * 0.5f;
			heightHalf = height * m_scaleY * 0.5f;
		}

		drawInfo->m_vtxs[0].pos = D3DXVECTOR3(-widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[1].pos = D3DXVECTOR3( widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[2].pos = D3DXVECTOR3(-widthHalf, -heightHalf, 0.0f);
		drawInfo->m_vtxs[3].pos = D3DXVECTOR3( widthHalf, -heightHalf, 0.0f);
	}
	else {
		drawInfo->m_vtxs[0].pos = m_vtxPoses[0];
		drawInfo->m_vtxs[1].pos = m_vtxPoses[1];
		drawInfo->m_vtxs[2].pos = m_vtxPoses[2];
		drawInfo->m_vtxs[3].pos = m_vtxPoses[3];
	}

	// [[[ 法線 ]]]
	drawInfo->m_vtxs[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	drawInfo->m_vtxs[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	drawInfo->m_vtxs[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	drawInfo->m_vtxs[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// [[[ 色 ]]]
	drawInfo->m_vtxs[0].col =
	drawInfo->m_vtxs[1].col =
	drawInfo->m_vtxs[2].col =
	drawInfo->m_vtxs[3].col = D3DCOLOR_RGBA(m_col.r, m_col.g, m_col.b, m_col.a);

	// [[[ UV座標 ]]]
	if (m_ptn == 0 && m_ptnX == 1 && m_ptnY == 1) {
		if (m_isTexMirrorX) {
			drawInfo->m_vtxs[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			drawInfo->m_vtxs[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			drawInfo->m_vtxs[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			drawInfo->m_vtxs[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}
		else {
			drawInfo->m_vtxs[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			drawInfo->m_vtxs[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			drawInfo->m_vtxs[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			drawInfo->m_vtxs[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	else {
		const float divX((1.0f / m_ptnX) * m_ptnScaleX);
		const float divY((1.0f / m_ptnY) * m_ptnScaleY);
		const float x   (((m_ptn % m_ptnX) * divX) + m_ptnPos.x);
		const float y   ((((m_ptn / m_ptnX) % m_ptnY) * divY) + m_ptnPos.y);

		if (m_isTexMirrorX) {
			const float left  (x + divX);
			const float bottom(y + divY);
			drawInfo->m_vtxs[0].tex = D3DXVECTOR2(left, y     );
			drawInfo->m_vtxs[1].tex = D3DXVECTOR2(x   , y     );
			drawInfo->m_vtxs[2].tex = D3DXVECTOR2(left, bottom);
			drawInfo->m_vtxs[3].tex = D3DXVECTOR2(x   , bottom);
		}
		else {
			const float right (x + divX);
			const float bottom(y + divY);
			drawInfo->m_vtxs[0].tex = D3DXVECTOR2(x    , y     );
			drawInfo->m_vtxs[1].tex = D3DXVECTOR2(right, y     );
			drawInfo->m_vtxs[2].tex = D3DXVECTOR2(x    , bottom);
			drawInfo->m_vtxs[3].tex = D3DXVECTOR2(right, bottom);
		}
	}

	return drawInfo;
}

//========================================
// 番号を設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetIdx(const short& idx) {

	if (this == NULL)
		return NULL;

	m_idx = idx;

	return this;
}

//========================================
// マトリックスを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// 頂点位置を設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetVtxPos(const Pos3D pos0, const Pos3D pos1, const Pos3D pos2, const Pos3D pos3) {

	if (this == NULL)
		return NULL;

	CMemory::Alloc(&m_vtxPoses, 4);
	m_vtxPoses[0] = pos0;
	m_vtxPoses[1] = pos1;
	m_vtxPoses[2] = pos2;
	m_vtxPoses[3] = pos3;

	return this;
}

//========================================
// 色を設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 大きさを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (this == NULL)
		return NULL;

	m_scaleX = width;
	m_scaleY = height;
	m_isFactScale = true;

	return this;
}

//========================================
// 大きさを設定(テクスチャ基準で拡大)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize_TexBaseScale(const float& scaleX, const float& scaleY) {

	if (this == NULL)
		return NULL;

	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_isFactScale = false;

	return this;
}

//========================================
// テクスチャを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_texIdx = texIdx;
	m_ptn = ptn;
	m_ptnX = ptnX;
	m_ptnY = ptnY;
	m_ptnPos = ptnPos;

	return this;
}

//========================================
// テクスチャを設定(カメラ)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex_Camera(CCamera* camera) {

	if (this == NULL)
		return NULL;

	m_texCamera = camera;

	return this;
}

//========================================
// テクスチャを固定しつつX方向に伸ばす
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::ExtendFixedTexX(const float& rateX) {

	if (this == NULL)
		return NULL;

	m_scaleX *= rateX;
	m_ptnScaleX = rateX;

	return this;
}

//========================================
// テクスチャを固定しつつY方向に伸ばす
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::ExtendFixedTexY(const float& rateY) {

	if (this == NULL)
		return NULL;

	m_scaleY *= rateY;
	m_ptnScaleY = rateY;
	
	return this;
}

//========================================
// Zテストを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;
	
	return this;
}

//========================================
// ライティングを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;
	
	return this;
}

//========================================
// ビルボードを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetBillboard(const bool& isBillboard) {

	if (this == NULL)
		return NULL;

	m_isBillboard = isBillboard;
	
	return this;
}

//========================================
// アルファブレンドモードを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetAlphaBlendMode(const CDrawState::ALPHA_BLEND_MODE& alphaBlendMode) {

	if (this == NULL)
		return NULL;

	m_alphaBlendMode = alphaBlendMode;

	return this;
}

//========================================
// テクスチャのX反転設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {
	
	if (this == NULL)
		return NULL;

	m_isTexMirrorX = isMirror;
	
	return this;
}

//========================================
// 優先度設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetPriority(const short& priority) {

	if (this == NULL)
		return NULL;

	m_priority = priority;

	return this;
}