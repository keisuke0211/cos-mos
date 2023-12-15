//========================================
// 
// ポリゴン2Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ポリゴン2Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos2D& pos, const Angle& angle, const float& width, const float& height) {

	// 対角線の長さと向き
	const float length          = sqrtf((width * width) + (height * height)) * 0.5f;
	const float vtxAngle        = atan2f(width, height);
	const float inverseVtxAngle = D3DX_PI - vtxAngle;

	// 頂点座標を設定
	{
		float resultAngle = angle - inverseVtxAngle;
		vtxs[0].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[0].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[0].pos.z = 0.0f;
		resultAngle = angle + inverseVtxAngle;
		vtxs[1].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[1].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[1].pos.z = 0.0f;
		resultAngle = angle - vtxAngle;
		vtxs[2].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[2].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[2].pos.z = 0.0f;
		resultAngle = angle + vtxAngle;
		vtxs[3].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[3].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[3].pos.z = 0.0f;
	}

	// 解像度を適用
	ApplyResolution(vtxs);
}

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {
	
	vtxs[0].pos = Pos3D(pos0.x, pos0.y, 0.0f);
	vtxs[1].pos = Pos3D(pos1.x, pos1.y, 0.0f);
	vtxs[2].pos = Pos3D(pos2.x, pos2.y, 0.0f);
	vtxs[3].pos = Pos3D(pos3.x, pos3.y, 0.0f);

	// 解像度を適用
	ApplyResolution(vtxs);
}

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos2D& pos, const float& width, const float& height) {

	vtxs[0].pos = Pos3D(pos.x        , pos.y         , 0.0f);
	vtxs[1].pos = Pos3D(pos.x + width, pos.y         , 0.0f);
	vtxs[2].pos = Pos3D(pos.x        , pos.y + height, 0.0f);
	vtxs[3].pos = Pos3D(pos.x + width, pos.y + height, 0.0f);

	// 解像度を適用
	ApplyResolution(vtxs);
}

//========================================
// [静的]頂点2Dに解像度を適用
//========================================
void CPolygon2D::ApplyResolution(Vertex2D* vtxs) {

	float resolution = RNLib::Window().GetResolution();
	vtxs[0].pos *= resolution;
	vtxs[1].pos *= resolution;
	vtxs[2].pos *= resolution;
	vtxs[3].pos *= resolution;
}

//========================================
// [静的]頂点2Dの座標変換用係数設定
//========================================
void CPolygon2D::SetVtxRHW(Vertex2D *pVtx) {

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// [静的]頂点2Dの色設定
//========================================
void CPolygon2D::SetVtxCol(Vertex2D *pVtx, const Color& col) {

	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// [静的]頂点2Dのテクスチャ座標設定
//========================================
void CPolygon2D::SetVtxTex_Cut(Vertex2D* vtxs, const Pos2D& cutPos, const float& size) {

	SetVtxTex_Cut(vtxs, cutPos, size, size);
}

//========================================
// [静的]頂点2Dのテクスチャ座標設定
//========================================
void CPolygon2D::SetVtxTex(Vertex2D* vtxs) {

	vtxs[0].tex = Pos2D(0.0f, 0.0f);
	vtxs[1].tex = Pos2D(1.0f, 0.0f);
	vtxs[2].tex = Pos2D(0.0f, 1.0f);
	vtxs[3].tex = Pos2D(1.0f, 1.0f);
}

//========================================
// [静的]頂点2Dのテクスチャ座標設定
//========================================
void CPolygon2D::SetVtxTex_Cut(Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height) {

	const float widthHalf  = width  * 0.5f;
	const float heightHalf = height * 0.5f;
	const float left       = cutPos.x - widthHalf;
	const float right      = cutPos.x + widthHalf;
	const float top        = cutPos.y - heightHalf;
	const float bottom     = cutPos.y + heightHalf;

	vtxs[0].tex = Pos2D(left , top   );
	vtxs[1].tex = Pos2D(right, top   );
	vtxs[2].tex = Pos2D(left , bottom);
	vtxs[3].tex = Pos2D(right, bottom);
}

//========================================
// コンストラクタ
//========================================
CPolygon2D::CPolygon2D() {

}

//========================================
// デストラクタ
//========================================
CPolygon2D::~CPolygon2D() {

}

//========================================
// 初期化処理
//========================================
void CPolygon2D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CPolygon2D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CPolygon2D::Update(void) {

}

//========================================
// 設置処理
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const Pos2D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen)
		->SetPos(pos)
		->SetAngle(angle);
}

//========================================
// 設置処理
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const Pos3D& pos, const Angle& angle, const bool& isOnScreen) {

	return Put(priority, Pos2D(pos.x, pos.y), angle, isOnScreen);
}

//========================================
// 設置処理
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
VertexBuffer CPolygon2D::CDrawInfo::ms_vtxBuff = NULL;
UShort CPolygon2D::CDrawInfo::ms_allocPower = 0;
UShort CPolygon2D::CDrawInfo::ms_allocNum   = 0;
UShort CPolygon2D::CDrawInfo::ms_idxCount   = 0;

//========================================
// [静的] 頂点バッファ初期生成処理
//========================================
void CPolygon2D::CDrawInfo::InitCreateVertexBuffer(void) {

	ms_allocPower = CDrawMgr::POLYGON2D_ALLOC_BASE_POWER;
	ms_allocNum   = pow(2, ms_allocPower);
	CreateVertexBuffer(ms_allocNum);
}

//========================================
// [静的]頂点バッファ生成処理
//========================================
void CPolygon2D::CDrawInfo::CreateVertexBuffer(const UShort& num) {

	// 頂点バッファの生成
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(sizeof(Vertex2D) * 4 * num, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &ms_vtxBuff, NULL);
}

//========================================
// [静的]頂点バッファ解放処理
//========================================
void CPolygon2D::CDrawInfo::ReleaseVertexBuffer(void) {

	// 頂点バッファの破棄
	if (ms_vtxBuff != NULL) {
		ms_vtxBuff->Release();
		ms_vtxBuff = NULL;
	}
}

//========================================
// コンストラクタ
//========================================
CPolygon2D::CDrawInfo::CDrawInfo() {

	m_idx               = 0;
	m_tex               = NULL;
	m_texType           = Polygon2DAnd3D::TEX_TYPE::NONE;
	m_interpolationMode = CDrawState::INTERPOLATION_MODE::NONE;
	for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// デストラクタ
//========================================
CPolygon2D::CDrawInfo::~CDrawInfo() {

	CMemory::Release(&m_tex);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CPolygon2D::CRegistInfo::CRegistInfo() {

	m_setVtxPosInfo = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CPolygon2D::CRegistInfo::~CRegistInfo() {

	ClearParameter();
}

//========================================
// パラメーターのクリア処理
//========================================
void CPolygon2D::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_idx               = 0;
	CMemory::Release(&m_setVtxPosInfo);
	m_pos               = INITPOS2D;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NONE;
	m_interpolationMode = CDrawState::INTERPOLATION_MODE::NONE;
	for (int cntVtx = 0; cntVtx < 4; m_vtxCols[cntVtx] = INITCOLOR, cntVtx++);
	m_setTexInfoSum.ClearParameter();
}

//========================================
// 描画情報に変換
//========================================
CPolygon2D::CDrawInfo* CPolygon2D::CRegistInfo::ConvToDrawInfo(void) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// 基底情報を代入
	AssignToDrawInfo(*drawInfo);

	// 情報を代入
	drawInfo->m_idx               = m_idx;
	drawInfo->m_interpolationMode = m_interpolationMode;

	//----------------------------------------
	// 頂点情報の設定
	//----------------------------------------
	// [[[ 位置 ]]]
	switch (m_setVtxPosInfoType) {
	case SET_VTX_POS_INFO_TYPE::NORMAL: {
		SetVtxPosInfo* setVtxPosInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
		CPolygon2D::SetVtxPos(drawInfo->m_vtxs, 
			setVtxPosInfo->vtxPoses[0],
			setVtxPosInfo->vtxPoses[1],
			setVtxPosInfo->vtxPoses[2],
			setVtxPosInfo->vtxPoses[3]);
	}break;
	case SET_VTX_POS_INFO_TYPE::SIZE: {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		CPolygon2D::SetVtxPos(drawInfo->m_vtxs, Pos2D(m_pos.x, m_pos.y), setSizeInfo->angle, setSizeInfo->width, setSizeInfo->height);
	}break;
	}

	// [[[ 座標変換係数 ]]]
	RNLib::Polygon2D().SetVtxRHW(drawInfo->m_vtxs);

	// [[[ 色 ]]]
	drawInfo->m_vtxs[0].col = D3DCOLOR_RGBA(m_vtxCols[0].r, m_vtxCols[0].g, m_vtxCols[0].b, m_vtxCols[0].a);
	drawInfo->m_vtxs[1].col = D3DCOLOR_RGBA(m_vtxCols[1].r, m_vtxCols[1].g, m_vtxCols[1].b, m_vtxCols[1].a);
	drawInfo->m_vtxs[2].col = D3DCOLOR_RGBA(m_vtxCols[2].r, m_vtxCols[2].g, m_vtxCols[2].b, m_vtxCols[2].a);
	drawInfo->m_vtxs[3].col = D3DCOLOR_RGBA(m_vtxCols[3].r, m_vtxCols[3].g, m_vtxCols[3].b, m_vtxCols[3].a);

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetIdx(const short& idx) {

	m_idx = idx;

	return this;
}

//========================================
// 位置を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetPos(const Pos2D& pos) {

	m_pos = pos;

	return this;
}

//========================================
// 角度を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetAngle(const Angle& angle) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::SIZE) {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->angle = angle;
	}
	else if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->width  = 0.0f;
		setSizeInfo->height = 0.0f;
		setSizeInfo->angle  = angle;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;
	}
	else {
		assert(false);	// ※既に別形式で頂点情報が設定されている
	}

	return this;
}

//========================================
// 頂点位置を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetVtxPos(const Pos2D pos0, const Pos2D pos1, const Pos2D pos2, const Pos2D pos3) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetVtxPosInfo**)&m_setVtxPosInfo);
		SetVtxPosInfo* setTexInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
		setTexInfo->vtxPoses[0] = pos0;
		setTexInfo->vtxPoses[1] = pos1;
		setTexInfo->vtxPoses[2] = pos2;
		setTexInfo->vtxPoses[3] = pos3;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NORMAL;
	}
	else {
		assert(false);	// ※既に別形式で頂点情報が設定されている
	}

	return this;
}

//========================================
// 大きさを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::SIZE) {
		SetSizeInfo* setTexInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setTexInfo->width  = width;
		setTexInfo->height = height;
	}
	else if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->width  = width;
		setSizeInfo->height = height;
		setSizeInfo->angle  = 0.0f;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;
	}
	else {
		assert(false);	// ※既に別形式で頂点情報が設定されている
	}
	
	return this;
}

//========================================
// 色を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_vtxCols[0] =
	m_vtxCols[1] =
	m_vtxCols[2] =
	m_vtxCols[3] = col;

	return this;
}
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetVtxCol(const Color col0, const Color col1, const Color col2, const Color col3) {

	if (this == NULL)
		return NULL;

	m_vtxCols[0] = col0;
	m_vtxCols[1] = col1;
	m_vtxCols[2] = col2;
	m_vtxCols[3] = col3;

	return this;
}

//========================================
// テクスチャを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(texIdx, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// テクスチャを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex(CCamera* camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(camera, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// テクスチャを設定(UV)
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(texIdx, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// テクスチャを設定(UV)
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexUV(CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(camera, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// テクスチャのX反転設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexMirrorX(isMirror);

	return this;
}

//========================================
// 補間モードを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetInterpolationMode(const CDrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}