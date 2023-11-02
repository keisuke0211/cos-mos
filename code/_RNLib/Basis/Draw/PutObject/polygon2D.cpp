//========================================
// 
// ポリゴン2Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNLib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ポリゴン2Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 設置処理
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const Pos3D& pos, const Angle& angle, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMng::GetProcessState() != CDrawMng::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMng().PutPolygon2D(pos, angle, isOnScreen);
}

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos3D& pos, const Angle& angle, const float& width, const float& height) {

	// 対角線の長さと向き
	const float length         (sqrtf((width * width) + (height * height)) * 0.5f);
	const float vtxAngle       (atan2f(width, height));
	const float inverseVtxAngle(D3DX_PI - vtxAngle);

	// 頂点座標を設定
	{
		float angle0(angle - inverseVtxAngle);
		vtxs[0].pos.x = pos.x + sinf(angle0) * length;
		vtxs[0].pos.y = pos.y + cosf(angle0) * length;
		vtxs[0].pos.z = 0.0f;
	} {
		float angle1(angle + inverseVtxAngle);
		vtxs[1].pos.x = pos.x + sinf(angle1) * length;
		vtxs[1].pos.y = pos.y + cosf(angle1) * length;
		vtxs[1].pos.z = 0.0f;
	} {
		float angle2(angle - vtxAngle);
		vtxs[2].pos.x = pos.x + sinf(angle2) * length;
		vtxs[2].pos.y = pos.y + cosf(angle2) * length;
		vtxs[2].pos.z = 0.0f;
	} {
		float angle3(angle + vtxAngle);
		vtxs[3].pos.x = pos.x + sinf(angle3) * length;
		vtxs[3].pos.y = pos.y + cosf(angle3) * length;
		vtxs[3].pos.z = 0.0f;
	}

	// 解像度を適用
	ApplyResolution(vtxs);
}

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos3D& pos0, const Pos3D& pos1, const Pos3D& pos2, const Pos3D& pos3) {
	
	vtxs[0].pos = pos0;
	vtxs[1].pos = pos1;
	vtxs[2].pos = pos2;
	vtxs[3].pos = pos3;

	// 解像度を適用
	ApplyResolution(vtxs);
}

//========================================
// [静的]頂点2Dの座標設定
//========================================
void CPolygon2D::SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos3D& pos, const float& width, const float& height) {

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

	float resolution(RNLib::Window().GetResolution());
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
void CPolygon2D::SetVtxTex_Cut(Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height) {

	const float widthHalf (width  * 0.5f);
	const float heightHalf(height * 0.5f);
	const float left      (cutPos.x - widthHalf);
	const float right     (cutPos.x + widthHalf);
	const float top       (cutPos.y - heightHalf);
	const float bottom    (cutPos.y + heightHalf);

	vtxs[0].tex = Pos2D(left , top   );
	vtxs[1].tex = Pos2D(right, top   );
	vtxs[2].tex = Pos2D(left , bottom);
	vtxs[3].tex = Pos2D(right, bottom);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
LPDIRECT3DVERTEXBUFFER9 CPolygon2D::CDrawInfo::m_vtxBuff = NULL;
unsigned short CPolygon2D::CDrawInfo::m_allocPower = 0;
unsigned short CPolygon2D::CDrawInfo::m_allocNum   = 0;
unsigned short CPolygon2D::CDrawInfo::m_idxCount   = 0;

//========================================
// [静的] 頂点バッファ初期生成処理
//========================================
void CPolygon2D::CDrawInfo::InitCreateVertexBuffer(void) {

	m_allocPower = CDrawMng::POLYGON2D_ALLOC_BASE_POWER;
	m_allocNum   = pow(2, m_allocPower);
	CreateVertexBuffer(m_allocNum);
}

//========================================
// [静的]頂点バッファ生成処理
//========================================
void CPolygon2D::CDrawInfo::CreateVertexBuffer(const unsigned short& num) {

	// 頂点バッファの生成
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(
		sizeof(Vertex2D) * 4 * num,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_vtxBuff,
		NULL);
}

//========================================
// [静的]頂点バッファ解放処理
//========================================
void CPolygon2D::CDrawInfo::ReleaseVertexBuffer(void) {

	// 頂点バッファの破棄
	if (m_vtxBuff != NULL) {
		m_vtxBuff->Release();
		m_vtxBuff = NULL;
	}
}

//========================================
// コンストラクタ
//========================================
CPolygon2D::CDrawInfo::CDrawInfo() {

	m_idx       = 0;
	m_texIdx    = NONEDATA;
	m_texCamera = NULL;
	m_isZTest   = true;
	m_distance  = 0.0f;
	for (int cntVtx(0); cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// デストラクタ
//========================================
CPolygon2D::CDrawInfo::~CDrawInfo() {

}

//========================================
// 描画処理
//========================================
void CPolygon2D::CDrawInfo::Draw(LPDIRECT3DDEVICE9& device, const Matrix& viewMtx) {

	// 頂点バッファがNULLの時、終了
	if (m_vtxBuff == NULL)
		return;

	//----------------------------------------
	// 一時的な描画モード設定を開始
	//----------------------------------------
	RNLib::DrawStateMng().StartTemporarySetMode();

	//----------------------------------------
	// 事前準備
	//----------------------------------------
	// フォグを無効化
	RNLib::DrawStateMng().SetFogMode(CDrawState::FOG_MODE::DISABLED, device);

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(Vertex2D));

	// [[[ テクスチャの設定 ]]]
	if (m_texCamera != NULL) {
		m_texCamera->SetTexture(device);
		RNLib::DrawStateMng().SetTextureAlphaMode(false, device);	// テクスチャの透過を無効化
	}
	else {
		RNLib::Texture().Set(device, m_texIdx);
	}

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_idx, 2);

	//----------------------------------------
	// 一時的な描画モード設定を終了
	//----------------------------------------
	RNLib::DrawStateMng().EndTemporarySetMode(device);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CPolygon2D::CRegistInfo::CRegistInfo() {

	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CPolygon2D::CRegistInfo::~CRegistInfo() {

}

//========================================
// パラメーターのクリア処理
//========================================
void CPolygon2D::CRegistInfo::ClearParameter(void) {

	m_idx          = NONEDATA;
	m_scaleX       = 1.0f;
	m_scaleY       = 1.0f;
	m_isFactScale  = false;
	m_pos          = INITPOS3D;
	m_angle        = 0.0f;
	m_col          = INITCOLOR;
	m_texIdx       = NONEDATA;
	m_texCamera    = NULL;
	m_ptn          = 0;
	m_ptnX         = 1;
	m_ptnY         = 1;
	m_ptnScaleX    = 1.0f;
	m_ptnScaleY    = 1.0f;
	m_ptnPos       = INITPOS2D;
	m_isZtest      = true;
	m_isTexMirrorX = false;
	m_priority     = 0;
}

//========================================
// 描画情報に変換
//========================================
CPolygon2D::CDrawInfo* CPolygon2D::CRegistInfo::ConvToDrawInfo(void) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo(NULL);
	RNLib::Memory().Alloc(&drawInfo);

	// 情報を代入
	// (基底)
	drawInfo->m_type      = CDrawInfoBase::TYPE::POLYGON2D;
	drawInfo->m_priority  = m_priority;
	// (継承)
	drawInfo->m_idx       = m_idx;
	drawInfo->m_texIdx    = m_texIdx;
	drawInfo->m_texCamera = m_texCamera;
	drawInfo->m_isZTest   = m_isZtest;
	drawInfo->m_distance  = -m_pos.z;

	//----------------------------------------
	// 頂点情報の設定
	//----------------------------------------
	// [[[ 位置 ]]]
	if (m_isFactScale) {
		RNLib::Polygon2D().SetVtxPos(drawInfo->m_vtxs, m_pos, m_angle, m_scaleX, m_scaleY);
	}
	else {
		RNLib::Polygon2D().SetVtxPos(drawInfo->m_vtxs, m_pos, m_angle, 
			(RNLib::Texture().GetWidth(m_texIdx) * PIXEL2D_SIZE) / m_ptnX,
			(RNLib::Texture().GetHeight(m_texIdx) * PIXEL2D_SIZE) / m_ptnY);
	}

	// [[[ 座標変換係数 ]]]
	RNLib::Polygon2D().SetVtxRHW(drawInfo->m_vtxs);

	// [[[ 色 ]]]
	RNLib::Polygon2D().SetVtxCol(drawInfo->m_vtxs, m_col);

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
			float left  (x + divX);
			float bottom(y + divY);
			drawInfo->m_vtxs[0].tex = D3DXVECTOR2(left, y     );
			drawInfo->m_vtxs[1].tex = D3DXVECTOR2(x   , y     );
			drawInfo->m_vtxs[2].tex = D3DXVECTOR2(left, bottom);
			drawInfo->m_vtxs[3].tex = D3DXVECTOR2(x   , bottom);
		}
		else {
			float right (x + divX);
			float bottom(y + divY);
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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetIdx(const short& idx) {

	if (this == NULL)
		return NULL;

	m_idx = idx;

	return this;
}

//========================================
// 位置を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetPos(const Pos3D& pos) {

	if (this == NULL)
		return NULL;

	m_pos = pos;

	return this;
}

//========================================
// 角度を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetAngle(const Angle& angle) {

	if (this == NULL)
		return NULL;

	m_angle = angle;

	return this;
}

//========================================
// 色を設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 大きさを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetSize(const float& width, const float& height) {

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetSize_TexBaseScale(const float& scaleX, const float& scaleY) {

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex(const short& texIdx, const unsigned short& ptn, const unsigned short& ptnX, const unsigned short& ptnY, const Pos2D& ptnPos) {

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex_Camera(CCamera* camera) {

	if (this == NULL)
		return NULL;

	m_texCamera = camera;

	return this;
}

//========================================
// テクスチャを固定しつつX方向に伸ばす
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::ExtendFixedTexX(const float& rateX) {

	if (this == NULL)
		return NULL;

	m_scaleX *= rateX;
	m_ptnScaleX = rateX;

	return this;
}

//========================================
// テクスチャを固定しつつY方向に伸ばす
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::ExtendFixedTexY(const float& rateY) {

	if (this == NULL)
		return NULL;

	m_scaleY *= rateY;
	m_ptnScaleY = rateY;

	return this;
}

//========================================
// Zテストを設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;

	return this;
}

//========================================
// テクスチャのX反転設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {

	if (this == NULL)
		return NULL;

	m_isTexMirrorX = isMirror;

	return this;
}

//========================================
// 優先度設定
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetPriority(const short& priority) {

	if (this == NULL)
		return NULL;

	m_priority = priority;

	return this;
}