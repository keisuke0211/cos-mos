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
			{// <<< 輪郭線メッシュの生成 & 半径の最大を調べる >>>
				// 輪郭線用にもう一度読み込む
				D3DXLoadMeshFromX(loadPath, D3DXMESH_SYSTEMMEM, device, NULL, NULL, NULL, NULL, &m_datas[idx]->m_outLineMesh);

				// 頂点フォーマットのサイズを取得
				const DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_datas[idx]->m_outLineMesh->GetFVF());

				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 頂点数を取得
				const int& vtxNum = m_datas[idx]->m_outLineMesh->GetNumVertices();

				// 頂点の縁取り情報構造体を定義
				struct VertexOutLine {
					Pos3D  totalVec = INITPOS3D;
					UShort count    = 0;
				};

				// 頂点の縁取り情報を生成
				VertexOutLine* vertexOutLines = NULL;
				CMemory::Alloc(&vertexOutLines, vtxNum);
				for (int cntVtx = 0; cntVtx < vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

				// 法線方向に加算
				for (int cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// 半径の最大を調べる
						const float dist = CGeometry::FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// 重なっている頂点位置に輪郭の加算距離を加算
					for (int cntVtx2 = 0; cntVtx2 < vtxNum; cntVtx2++) {

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

				// 頂点位置に加算位置を加算
				for (int cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Pos3D* pos = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					D3DXVec3Normalize(&vertexOutLines[cntVtx].totalVec, &vertexOutLines[cntVtx].totalVec);
					*pos += vertexOutLines[cntVtx].totalVec * RNSettings::GetInfo().modelOutLineAddDistance;
				}

				// 頂点の縁取り情報を破棄
				RNLib::Memory().Release(&vertexOutLines);

				// 頂点バッファをアンロック
				m_datas[idx]->m_outLineMesh->UnlockVertexBuffer();
			}// <<< >>>

			// マテリアル情報に対するポインタを取得
			const D3DXMATERIAL* mats((D3DXMATERIAL*)m_datas[idx]->m_matBuff->GetBufferPointer());

			// テクスチャ番号のメモリ確保
			CMemory::Alloc(&m_datas[idx]->m_texIdxs, m_datas[idx]->m_matNum);

			// テクスチャの読み込み
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++)
				m_datas[idx]->m_texIdxs[cntMat] = (mats[cntMat].pTextureFilename != NULL) ? RNLib::Texture().Load(mats[cntMat].pTextureFilename) : NONEDATA;
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
		Vertex3DInfo& vtx = (*vtxInfos)[cntVtx];
		vtx.pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
		vtx.nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

		// ワールドマトリックスを算出
		Matrix worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosNorToMtx(vtx.pos, vtx.nor), modelMtx);

		vtx.worldPos = CMatrix::ConvMtxToPos(worldMtx);
		vtx.rot      = CGeometry::FindVecRot(vtx.nor);
		vtx.worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
		vtx.worldRot = CGeometry::FindVecRot(vtx.worldNor);
	}

	// 頂点バッファをアンロック
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// 設置処理
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMgr::GetProcessState() != CDrawMgr::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

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

	m_texIdxs     = NULL;
	m_mesh        = NULL;
	m_outLineMesh = NULL;
	m_matBuff     = NULL;
	m_matNum      = 0;
	m_radiusMax   = 0.0f;
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

	// テクスチャ番号の破棄
	CMemory::Release(&m_texIdxs);

	// メッシュの破棄
	if (m_mesh != NULL) {
		m_mesh->Release();
		m_mesh = NULL;
	}

	// 輪郭線メッシュの破棄
	if (m_outLineMesh != NULL) {
		m_outLineMesh->Release();
		m_outLineMesh = NULL;
	}

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
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
	m_distance             = 0.0f;
}

//========================================
// デストラクタ
//========================================
CModel::CDrawInfo::~CDrawInfo() {

}

//========================================
// 描画処理
//========================================
void CModel::CDrawInfo::Draw(Device& device, const Matrix& viewMtx) {

	//----------------------------------------
	// 事前準備
	//----------------------------------------
	// モデルデータを取得
	const CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// 本体のワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &m_mtx);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* mats = (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();

	// マトリックスの拡大倍率を取得し、
	const Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);
	const bool isScaling = !EqualFloat(scale.x,1.0f, 0.01f) ? true : !EqualFloat(scale.y, 1.0f, 0.01f) ? true : !EqualFloat(scale.z, 1.0f, 0.01f);
	LPD3DXMESH drawMesh = NULL;
	if (isScaling)
	{// もし拡大倍率に変更があった時、
		const DWORD fvf    = modelData.m_mesh->GetFVF();
		const ULong vtxNum = modelData.m_mesh->GetNumVertices();

		// メッシュを複製する
		D3DXCreateMeshFVF(modelData.m_mesh->GetNumFaces(), vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, modelData.m_mesh->GetFVF(), device, &drawMesh);
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawMesh);

		// 頂点フォーマットのサイズを取得
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);

		// 頂点バッファをロック
		BYTE* vtxBuff = NULL;
		drawMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// 法線方向に加算
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
			Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

			nor->x *= scale.x;
			nor->y *= scale.y;
			nor->z *= scale.z;
		}

		// 頂点バッファをアンロック
		drawMesh->UnlockVertexBuffer();
	}
	else 
	{// 拡大倍率に変更がなかった時、
		// 描画メッシュはそのまま使用
		drawMesh = modelData.m_mesh;
	}

	//----------------------------------------
	// 一時的な描画モード設定を開始
	//----------------------------------------
	RNLib::DrawStateMgr().StartTemporarySetMode();

	// [[[ Zテストの設定 ]]]
	RNLib::DrawStateMgr().SetZTestMode(m_isZTest, device);

	//----------------------------------------
	// 表面の描画
	//----------------------------------------
	for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
		
		{// [[[ マテリアルの設定 ]]]
			Color setCol = m_col;

			// 発光部分の明るさが設定されている時、
			if (m_brightnessOfEmissive < 1.0f) {

				// 発光要素があれば適用
				if (0.0f < mats[cntMat].MatD3D.Emissive.r + mats[cntMat].MatD3D.Emissive.g + mats[cntMat].MatD3D.Emissive.b) {
					setCol = BrightnessToColor(setCol, m_brightnessOfEmissive);
				}
			}
			
			// マテリアルを設定
			SetMaterial(device, &mats[cntMat].MatD3D, setCol);
		}

		// [[[ テクスチャの設定 ]]]
		RNLib::Texture().Set(device, (m_texIdx == NONEDATA) ? modelData.m_texIdxs[cntMat] : m_texIdx);

		// 描画
		drawMesh->DrawSubset(cntMat);
	}

	//----------------------------------------
	// 裏面の描画
	//----------------------------------------
	if (m_isOutLine) {

		// マテリアルの設定
		device->SetMaterial(&ms_outLineMat);
		RNLib::DrawStateMgr().SetCullingMode(CDrawState::CULLING_MODE::BACK_SIDE, device);

		for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
			modelData.m_outLineMesh->DrawSubset(cntMat);
		}
	}

	//----------------------------------------
	// 一時的な描画モード設定を終了
	//----------------------------------------
	RNLib::DrawStateMgr().EndTemporarySetMode(device);

	// 拡大倍率に変更があった時、解放する
	if (isScaling)
		drawMesh->Release();
}

//========================================
// マテリアル設定処理
//========================================
void CModel::CDrawInfo::SetMaterial(Device& device, Material* mat, const Color& col) {

	// マテリアルの材質パラメータを保存
	const D3DXCOLOR diffuseTemp  = mat->Diffuse;
	const D3DXCOLOR emissiveTemp = mat->Emissive;

	// マテリアルの材質パラメータを設定
	const float r = (float)col.r / 255;
	const float g = (float)col.g / 255;
	const float b = (float)col.b / 255;
	const float a = (float)col.a / 255;
	mat->Diffuse.r  = diffuseTemp .r * r;
	mat->Diffuse.g  = diffuseTemp .g * g;
	mat->Diffuse.b  = diffuseTemp .b * b;
	mat->Diffuse.a  = diffuseTemp .a * a;
	mat->Emissive.r = emissiveTemp.r * r;
	mat->Emissive.g = emissiveTemp.g * g;
	mat->Emissive.b = emissiveTemp.b * b;
	mat->Emissive.a = emissiveTemp.a * a;

	// マテリアルの設定
	device->SetMaterial(mat);

	// マテリアルの材質パラメータを元に戻す
	mat->Diffuse  = diffuseTemp;
	mat->Emissive = emissiveTemp;
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
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
}

//========================================
// 描画情報に変換
//========================================
CModel::CDrawInfo* CModel::CRegistInfo::ConvToDrawInfo(void) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// 基底情報を代入
	AssignToDrawInfo(*drawInfo);

	// 情報を代入
	drawInfo->m_mtx                  = m_mtx;
	drawInfo->m_col                  = m_col;
	drawInfo->m_modelIdx             = m_modelIdx;
	drawInfo->m_texIdx               = m_texIdx;
	drawInfo->m_isZTest              = m_isZTest;
	drawInfo->m_isLighting           = m_isLighting;
	drawInfo->m_isOutLine            = m_isOutLine;
	drawInfo->m_brightnessOfEmissive = m_brightnessOfEmissive;
	{// 距離を算出
		// 拡大倍率の最大を算出
		float scaleMax = 0.0f; {
			Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);

			scaleMax = scale.x;
			if (scaleMax < scale.y)
				scaleMax = scale.y;
			if (scaleMax < scale.z)
				scaleMax = scale.z;
		}
		drawInfo->m_distance = 0.0f;//CGeometry::FindDistanceToCameraPlane(CMatrix::ConvMtxToPos(m_mtx), RNLib::Camera3D()) - (RNLib::Model().GetData(m_modelIdx).m_radiusMax * scaleMax);
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
CModel::CRegistInfo* CModel::CRegistInfo::SetOutLine(const bool& isOutLine) {

	if (this == NULL)
		return NULL;

	m_isOutLine = isOutLine;

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