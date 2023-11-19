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
CPolygon3D::CRegistInfo* CPolygon3D::Put(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMgr::GetProcessState() != CDrawMgr::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMgr().PutPolygon3D(priority, mtx, isOnScreen);
}

//========================================
// 設置処理(位置と向き指定)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::Put(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {

	return Put(priority, CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
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
	m_tex            = NULL;
	m_texType        = Polygon2DAnd3D::TEX_TYPE::NONE;
	m_isZTest        = true;
	m_isLighting     = true;
	m_isBillboard    = false;
	m_alphaBlendMode = CDrawState::ALPHA_BLEND_MODE::NORMAL;
	m_distance       = 0.0f;
	for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// デストラクタ
//========================================
CPolygon3D::CDrawInfo::~CDrawInfo() {

	CMemory::Release<void>(&m_tex);
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
	Polygon2DAnd3D::SetTexture(device, m_tex, m_texType);

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

	m_setVtxPosInfo = NULL;
	m_setVtxNorInfo = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CPolygon3D::CRegistInfo::~CRegistInfo() {

	ClearParameter();
}

//========================================
// パラメータークリア処理
//========================================
void CPolygon3D::CRegistInfo::ClearParameter(void) {

	m_idx				= 0;
	CMemory::Release(&m_setVtxPosInfo);
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NONE;
	CMemory::Release(&m_setVtxNorInfo);
	m_col				= INITCOLOR;
	m_setTexInfoSum.ClearParameter();
	m_isZtest			= true;
	m_isLighting		= true;
	m_isBillboard		= false;
	m_alphaBlendMode	= CDrawState::ALPHA_BLEND_MODE::NORMAL;
}

//========================================
// 描画情報に変換
//========================================
CPolygon3D::CDrawInfo* CPolygon3D::CRegistInfo::ConvToDrawInfo(void) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE)
		return NULL;

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// 基底情報を代入
	AssignToDrawInfo(*drawInfo, CDrawInfoBase::TYPE::POLYGON3D);

	// 情報を代入
	drawInfo->m_idx            = m_idx;
	drawInfo->m_mtx            = m_mtx;
	drawInfo->m_isZTest        = m_isZtest;
	drawInfo->m_isLighting     = m_isLighting;
	drawInfo->m_isBillboard    = m_isBillboard;
	drawInfo->m_alphaBlendMode = m_alphaBlendMode;

	//----------------------------------------
	// 頂点情報の設定
	//----------------------------------------
	// [[[ 位置 ]]]
	switch (m_setVtxPosInfoType) {
	case SET_VTX_POS_INFO_TYPE::NORMAL: {
		SetVtxPosInfo* setVtxPosInfo = (SetVtxPosInfo*)m_setVtxPosInfo;

		drawInfo->m_vtxs[0].pos = setVtxPosInfo->vtxPoses[0];
		drawInfo->m_vtxs[1].pos = setVtxPosInfo->vtxPoses[1];
		drawInfo->m_vtxs[2].pos = setVtxPosInfo->vtxPoses[2];
		drawInfo->m_vtxs[3].pos = setVtxPosInfo->vtxPoses[3];
	}break;
	case SET_VTX_POS_INFO_TYPE::SIZE: {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		const float widthHalf  = setSizeInfo->width  * 0.5f;
		const float heightHalf = setSizeInfo->height * 0.5f;

		drawInfo->m_vtxs[0].pos = D3DXVECTOR3(-widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[1].pos = D3DXVECTOR3( widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[2].pos = D3DXVECTOR3(-widthHalf, -heightHalf, 0.0f);
		drawInfo->m_vtxs[3].pos = D3DXVECTOR3( widthHalf, -heightHalf, 0.0f);
	}break;
	}

	// [[[ 法線 ]]]
	if (m_setVtxNorInfo == NULL) {
		drawInfo->m_vtxs[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	else {
		drawInfo->m_vtxs[0].nor = m_setVtxNorInfo->vtxNors[0];
		drawInfo->m_vtxs[1].nor = m_setVtxNorInfo->vtxNors[1];
		drawInfo->m_vtxs[2].nor = m_setVtxNorInfo->vtxNors[2];
		drawInfo->m_vtxs[3].nor = m_setVtxNorInfo->vtxNors[3];
	}

	// [[[ 色 ]]]
	drawInfo->m_vtxs[0].col =
	drawInfo->m_vtxs[1].col =
	drawInfo->m_vtxs[2].col =
	drawInfo->m_vtxs[3].col = D3DCOLOR_RGBA(m_col.r, m_col.g, m_col.b, m_col.a);

	// [[[ UV座標 ]]]
	m_setTexInfoSum.AssignTexInfo(
		drawInfo->m_tex,
		drawInfo->m_texType,
		drawInfo->m_vtxs[0].tex,
		drawInfo->m_vtxs[1].tex,
		drawInfo->m_vtxs[2].tex,
		drawInfo->m_vtxs[3].tex);

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

	CMemory::Alloc((SetVtxPosInfo**)&m_setVtxPosInfo);
	SetVtxPosInfo* setTexInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
	setTexInfo->vtxPoses[0] = pos0;
	setTexInfo->vtxPoses[1] = pos1;
	setTexInfo->vtxPoses[2] = pos2;
	setTexInfo->vtxPoses[3] = pos3;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NORMAL;

	return this;
}

//========================================
// 大きさを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (this == NULL)
		return NULL;
	
	CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
	SetSizeInfo* setTexInfo = (SetSizeInfo*)m_setVtxPosInfo;
	setTexInfo->width  = width;
	setTexInfo->height = height;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;

	return this;
}

//========================================
// 大きさを設定(サイズ)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize(const Size2D& size) {

	return SetSize(size.x, size.y);
}

//========================================
// 法線を設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetVtxNor(const Vector3D nor0, const Vector3D nor1, const Vector3D nor2, const Vector3D nor3) {

	if (this == NULL)
		return NULL;

	CMemory::Alloc(&m_setVtxNorInfo);
	m_setVtxNorInfo->vtxNors[0] = nor0;
	m_setVtxNorInfo->vtxNors[1] = nor1;
	m_setVtxNorInfo->vtxNors[2] = nor2;
	m_setVtxNorInfo->vtxNors[3] = nor3;

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
// テクスチャを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL; 

	m_setTexInfoSum.SetTex(texIdx, ptn, ptnX, ptnY, ptnPos);
	
	return this;
}

//========================================
// テクスチャを設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex(CCamera* camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(camera, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// テクスチャを設定(UV)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(texIdx, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// テクスチャを設定(UV)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexUV(CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(camera, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// テクスチャのX反転設定
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexMirrorX(isMirror);

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