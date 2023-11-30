//========================================
// 
// モデルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include "../../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]モデルクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CModel::CModel() {

	m_datas = NULL;
}

//========================================
// デストラクタ
//========================================
CModel::~CModel() {
	
}

//========================================
// 初期化処理
//========================================
void CModel::Init(void) {

}

//========================================
// 終了処理
//========================================
void CModel::Uninit(void) {

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void CModel::Update(void) {

}

//========================================
// 解放処理
//========================================
void CModel::Release(void) {

	// データの解放
	CMemory::ReleaseDouble(&m_datas, m_num);
}

//========================================
// 読み込み処理
//========================================
short CModel::Load(const char* loadPath, short idx) {

	const UShort oldNum = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx)) 
	{// 読み込み成功
		// データのメモリ再確保
		CMemory::ReAllocDouble(&m_datas, oldNum, m_num);

		// データの破棄(番号指定の場合)
		if (idxOld != NONEDATA)
			m_datas[idx]->Release();

		// Xファイルの読み込み
		Device device(RNLib::Window().GetD3DDevice());
		if (FAILED(D3DXLoadMeshFromX(loadPath, D3DXMESH_SYSTEMMEM, device, NULL, &m_datas[idx]->m_matBuff, NULL, (DWORD*)&m_datas[idx]->m_matNum, &m_datas[idx]->m_mesh))) 
		{// 読み込み失敗
			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("モデルの読み込みに失敗しました。\n%s", loadPath));

			// データのメモリリセット
			CMemory::ReAllocDouble(&m_datas, m_num, oldNum);

			// 読み込み済パスのメモリリセット
			ReAllocLoadPath(oldNum);

			// 数をリセット
			m_num = oldNum;

			return NONEDATA;
		}
		else
		{// 読み込みに成功した時、
			const DWORD fvf       = m_datas[idx]->m_mesh->GetFVF();
			const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);
			const ULong vtxNum    = m_datas[idx]->m_mesh->GetNumVertices();
			const ULong faceNum   = m_datas[idx]->m_mesh->GetNumFaces();

			// 頂点の縁取り情報構造体を定義
			struct VertexOutLine {
				Pos3D  totalVec = INITPOS3D;
				UShort count = 0;
			};

			// 頂点の縁取り情報を生成
			VertexOutLine* vertexOutLines = NULL;
			CMemory::Alloc(&vertexOutLines, vtxNum);
			for (ULong cntVtx = 0; cntVtx < vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

			{// 頂点の縁取り情報を算出
				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 法線方向に加算
				for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// 半径の最大を調べる
						const float dist = CGeometry::FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// 重なっている頂点位置に輪郭の加算距離を加算
					for (ULong cntVtx2 = 0; cntVtx2 < vtxNum; cntVtx2++) {

						// 今のカウントの頂点と同じなら、折り返す
						if (cntVtx == cntVtx2)
							continue;

						// 頂点が重なっている時、加算位置の値を加算
						Pos3D* pos2 = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx2));
						if (*pos == *pos2) {
							vertexOutLines[cntVtx2].totalVec += *nor;
							vertexOutLines[cntVtx2].count++;
						}
					}

					// 現カウント頂点の加算位置の値を加算
					vertexOutLines[cntVtx].totalVec += *nor;
					vertexOutLines[cntVtx].count++;
				}

				// 頂点バッファをアンロック
				m_datas[idx]->m_mesh->UnlockVertexBuffer();
			}

			// 輪郭メッシュを生成する
			CMemory::Alloc(&m_datas[idx]->m_outLineMeshs, RNSettings::GetInfo().modelOutLineAddDistanceDelimiter);

			for (UShort cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {

				// メッシュを複製する
				D3DXCreateMeshFVF(faceNum, vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);
				m_datas[idx]->m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);

				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMeshs[cntOutLine]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 頂点位置に加算位置を加算
				for (UInt cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Pos3D* pos = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					D3DXVec3Normalize(&vertexOutLines[cntVtx].totalVec, &vertexOutLines[cntVtx].totalVec);
					*pos += vertexOutLines[cntVtx].totalVec * RNSettings::GetInfo().modelOutLineAddDistanceInterval * (cntOutLine + 1);
				}

				// 頂点バッファをアンロック
				m_datas[idx]->m_outLineMeshs[cntOutLine]->UnlockVertexBuffer();
			}

			// 頂点の縁取り情報を破棄
			RNLib::Memory().Release(&vertexOutLines);

			// マテリアル情報に対するポインタを取得
			const D3DXMATERIAL* mats = (D3DXMATERIAL*)m_datas[idx]->m_matBuff->GetBufferPointer();

			// テクスチャのメモリ確保
			CMemory::Alloc(&m_datas[idx]->m_texIdxes, m_datas[idx]->m_matNum);
			CMemory::Alloc(&m_datas[idx]->m_texes, m_datas[idx]->m_matNum);

			// テクスチャの読み込み
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
				m_datas[idx]->m_texIdxes[cntMat] = (mats[cntMat].pTextureFilename != NULL) ? RNLib::Texture().Load(mats[cntMat].pTextureFilename)          : NONEDATA;
				m_datas[idx]->m_texes   [cntMat] = m_datas[idx]->m_texIdxes[cntMat] >= 0   ? RNLib::Texture().GetTexture(m_datas[idx]->m_texIdxes[cntMat]) : NULL;
			}
		}
	}

	return idx;
}

//========================================
// 頂点情報を格納
//========================================
void CModel::StoreVtxInfo(UInt* vtxNum, Vertex3DInfo** vtxInfos, const short& modelIdx, const Matrix& modelMtx) {

	if (modelIdx == NONEDATA) {
		*vtxNum   = 0;
		*vtxInfos = NULL;
		return;
	}

	// 頂点フォーマットのサイズを取得
	const DWORD dwSizeFVF(D3DXGetFVFVertexSize(m_datas[modelIdx]->m_mesh->GetFVF()));

	// 頂点バッファをロック
	BYTE* vtxBuff = NULL;
	m_datas[modelIdx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

	// 頂点数を取得
	*vtxNum = m_datas[modelIdx]->m_mesh->GetNumVertices();

	// 頂点情報を生成
	CMemory::Alloc(vtxInfos, *vtxNum);

	for (UInt cntVtx = 0; cntVtx < *vtxNum; cntVtx++) {
		Vertex3DInfo* vtx = &(*vtxInfos)[cntVtx];
		vtx->pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
		vtx->nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

		// ワールドマトリックスを算出
		Matrix worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosNorToMtx(vtx->pos, vtx->nor), modelMtx);

		vtx->worldPos = CMatrix::ConvMtxToPos(worldMtx);
		vtx->rot      = CGeometry::FindVecRot(vtx->nor);
		vtx->worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
		vtx->worldRot = CGeometry::FindVecRot(vtx->worldNor);
	}

	// 頂点バッファをアンロック
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// 設置処理
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// モデル番号が無しの時、
	if (modelIdx == NONEDATA)
		return NULL;

	return RNLib::DrawMgr().PutModel(priority, mtx, isOnScreen)
		->SetModel(modelIdx);
}

//========================================
// 設置処理(位置と向きで指定)
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {
	
	return Put(priority, modelIdx, CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
}

//========================================
// 設置処理(位置と向きと拡大倍率で指定)
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen) {

	return Put(priority, modelIdx, CMatrix::ConvPosRotScaleToMtx(pos, rot, scale), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| データクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CModel::CData::CData() {

	m_texIdxes     = NULL;
	m_texes        = NULL;
	m_mesh         = NULL;
	m_outLineMeshs = NULL;
	m_matBuff      = NULL;
	m_matNum       = 0;
	m_radiusMax    = 0.0f;
}

//========================================
// デストラクタ
//========================================
CModel::CData::~CData() {

	Release();
}

//========================================
// 解放処理
//========================================
void CModel::CData::Release(void) {

	// テクスチャの破棄
	CMemory::Release(&m_texIdxes);
	CMemory::Release(&m_texes);

	// メッシュの破棄
	if (m_mesh != NULL) {
		m_mesh->Release();
		m_mesh = NULL;
	}

	// 輪郭線メッシュの破棄
	if (m_outLineMeshs != NULL) {
		for (int cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {
			if (m_outLineMeshs[cntOutLine] != NULL) {
				m_outLineMeshs[cntOutLine]->Release();
				m_outLineMeshs[cntOutLine] = NULL;
			}
		}
	}
	CMemory::Release(&m_outLineMeshs);

	// マテリアルの破棄
	if (m_matBuff != NULL) {
		m_matBuff->Release();
		m_matBuff = NULL;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 描画情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
Material CModel::CDrawInfo::ms_outLineMat = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	0.0f
};

//========================================
// コンストラクタ
//========================================
CModel::CDrawInfo::CDrawInfo() {

	m_mtx                  = INITMATRIX;
	m_mats                 = NULL;
	m_texes                = NULL;
	m_matNum               = 0;
	m_mesh                 = NULL;
	m_outLineMesh          = NULL;
	m_isScaling            = false;
	m_isZTest              = true;
	m_isLighting           = false;
}

//========================================
// デストラクタ
//========================================
CModel::CDrawInfo::~CDrawInfo() {

	CMemory::Release(&m_mats);

	// 拡大倍率に変更があった時、解放する
	if (m_isScaling) {
		if (m_mesh != NULL)
			m_mesh->Release();
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CModel::CRegistInfo::CRegistInfo() {

	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CModel::CRegistInfo::~CRegistInfo() {

}

//========================================
// パラメータークリア処理
//========================================
void CModel::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_mtx                  = INITMATRIX;
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_outLineIdx           = NONEDATA;
	m_brightnessOfEmissive = 1.0f;
}

//========================================
// 描画情報に変換
//========================================
CModel::CDrawInfo* CModel::CRegistInfo::ConvToDrawInfo(Device& device) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// 基底情報を代入
	AssignToDrawInfo(*drawInfo);

	const CModel::CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// 情報を代入
	drawInfo->m_mtx                  = m_mtx;
	drawInfo->m_texes                = modelData.m_texes;
	drawInfo->m_matNum               = modelData.m_matNum;
	drawInfo->m_isZTest              = m_isZTest;
	drawInfo->m_isLighting           = m_isLighting;

	//----------------------------------------
	// マテリアル情報を算出
	//----------------------------------------
	{
		D3DXMATERIAL* mats= (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();
		CMemory::Alloc(&drawInfo->m_mats, drawInfo->m_matNum);
		for (int cntMat = 0; cntMat < drawInfo->m_matNum; cntMat++) {
			drawInfo->m_mats[cntMat] = mats[cntMat].MatD3D;
			
			float brightness = 1.0f;
			if (drawInfo->m_mats[cntMat].Emissive.r +
				drawInfo->m_mats[cntMat].Emissive.g +
				drawInfo->m_mats[cntMat].Emissive.b > 0.0f) {
				brightness = m_brightnessOfEmissive;
			}

			// マテリアルの材質パラメータを設定
			const float r = ((float)m_col.r / 255) * brightness;
			const float g = ((float)m_col.g / 255) * brightness;
			const float b = ((float)m_col.b / 255) * brightness;
			const float a = ((float)m_col.a / 255);
			drawInfo->m_mats[cntMat].Diffuse.r *= r;
			drawInfo->m_mats[cntMat].Diffuse.g *= g;
			drawInfo->m_mats[cntMat].Diffuse.b *= b;
			drawInfo->m_mats[cntMat].Diffuse.a *= a;
			drawInfo->m_mats[cntMat].Emissive.r *= r;
			drawInfo->m_mats[cntMat].Emissive.g *= g;
			drawInfo->m_mats[cntMat].Emissive.b *= b;
			drawInfo->m_mats[cntMat].Emissive.a *= a;
		}
	}

	//----------------------------------------
	// 輪郭メッシュ情報を算出
	//----------------------------------------
	if (m_outLineIdx == NONEDATA) {
		drawInfo->m_outLineMesh = NULL;
	}
	else {
		if (m_outLineIdx > RNSettings::GetInfo().modelOutLineAddDistanceDelimiter) {
			m_outLineIdx = RNSettings::GetInfo().modelOutLineAddDistanceDelimiter;
		}
		drawInfo->m_outLineMesh = modelData.m_outLineMeshs[m_outLineIdx];
	}

	//----------------------------------------
	// メッシュ情報を算出
	//----------------------------------------
	// マトリックスの拡大倍率を取得し、
	const Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);
	drawInfo->m_isScaling = !EqualFloat(scale.x, 1.0f, 0.01f) ? true : !EqualFloat(scale.y, 1.0f, 0.01f) ? true : !EqualFloat(scale.z, 1.0f, 0.01f);
	if (drawInfo->m_isScaling)
	{// もし拡大倍率に変更があった時、
		const DWORD fvf       = modelData.m_mesh->GetFVF();
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);
		const ULong vtxNum    = modelData.m_mesh->GetNumVertices();
		const ULong faceNum   = modelData.m_mesh->GetNumFaces();

		// メッシュを複製する
		D3DXCreateMeshFVF(faceNum, vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);

		// メッシュがNULLであれば、
		if (drawInfo->m_mesh == NULL) {
			CMemory::Release(&drawInfo);
			return NULL;
		}

		// 頂点バッファをロック
		BYTE* vtxBuff = NULL;
		drawInfo->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// 法線方向に加算
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
			Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

			nor->x *= scale.x;
			nor->y *= scale.y;
			nor->z *= scale.z;
		}

		// 頂点バッファをアンロック
		drawInfo->m_mesh->UnlockVertexBuffer();
	}
	else
	{// 拡大倍率に変更がなかった時、
		// 描画メッシュはそのまま使用
		drawInfo->m_mesh = modelData.m_mesh;
	}

	return drawInfo;
}

//========================================
// クリッピングカメラ設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetClippingCamera(CCamera& camera) {

	if (this == NULL)
		return NULL;

	if (&camera == NULL)
		return this;

	m_clippingID = camera.GetID();

	return this;
}

//========================================
// クリッピングカメラ設定(ID指定)
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetClippingCamera(const short& ID) {

	if (this == NULL)
		return NULL;

	m_clippingID = ID;

	return this;
}

//========================================
// マトリックスを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// 色を設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// モデルを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetModel(const short& modelIdx) {

	if (this == NULL)
		return NULL;

	m_modelIdx = modelIdx;

	return this;
}

//========================================
// テクスチャを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetTex(const short& texIdx) {

	if (this == NULL)
		return NULL;

	m_texIdx = texIdx;

	return this;
}

//========================================
// Zテストを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZTest = isZTest;

	return this;
}

//========================================
// ライティングを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// 輪郭線を設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetOutLineIdx(const UShort& outLineIdx) {

	if (this == NULL)
		return NULL;

	m_outLineIdx = outLineIdx;

	return this;
}

//========================================
// 発光部分の明るさを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetBrightnessOfEmissive(const float& brightnessOfEmissive) {

	if (this == NULL)
		return NULL;

	m_brightnessOfEmissive = brightnessOfEmissive;

	return this;
}