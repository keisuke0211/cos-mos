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

	// 解放処理
	Release();
}

//========================================
// 解放処理
//========================================
void CModel::Release(void) {

	// データの解放
	RNLib::Memory().ReleaseDouble(&m_datas, m_num);
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
		RNLib::Memory().ReAllocDouble(&m_datas, oldNum, m_num);

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
			RNLib::Memory().ReAllocDouble(&m_datas, m_num, oldNum);

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
				const DWORD dwSizeFVF(D3DXGetFVFVertexSize(m_datas[idx]->m_outLineMesh->GetFVF()));

				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 法線方向に加算
				const int&   vtxNum                  = m_datas[idx]->m_outLineMesh->GetNumVertices();
				const float& modelOutLineAddDistance = RNSettings::GetInfo().modelOutLineAddDistance;
				for (int cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					D3DXVECTOR3* pos =  (D3DXVECTOR3*)(vtxBuff + (dwSizeFVF * cntVtx));
					D3DXVECTOR3  nor = *(D3DXVECTOR3*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// 半径の最大を調べる
						const float dist = CGeometry::FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// 位置に輪郭の加算距離を加算
					*pos += nor * modelOutLineAddDistance;
				}

				// 頂点バッファをアンロック
				m_datas[idx]->m_outLineMesh->UnlockVertexBuffer();
			}// <<< >>>

			// マテリアル情報に対するポインタを取得
			const D3DXMATERIAL* mats((D3DXMATERIAL*)m_datas[idx]->m_matBuff->GetBufferPointer());

			// テクスチャ番号のメモリ確保
			RNLib::Memory().Alloc(&m_datas[idx]->m_texIdxs, m_datas[idx]->m_matNum);

			// テクスチャの読み込み
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++)
				m_datas[idx]->m_texIdxs[cntMat] = (mats[cntMat].pTextureFilename != NULL) ? RNLib::Texture().Load(mats[cntMat].pTextureFilename) : NONEDATA;
		}
	}

	return idx;
}

//========================================
// 設置処理
//========================================
CModel::CRegistInfo* CModel::Put(const Matrix& mtx, const short& modelIdx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMng::GetProcessState() != CDrawMng::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMng().PutModel(mtx, isOnScreen)
		->SetModel(modelIdx);
}

//========================================
// 設置処理(位置と向きで指定)
//========================================
CModel::CRegistInfo* CModel::Put(const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const bool& isOnScreen) {
	
	return Put(CMatrix::ConvPosRotToMtx(pos, rot), modelIdx, isOnScreen);
}

//========================================
// 設置処理(位置と向きと拡大倍率で指定)
//========================================
CModel::CRegistInfo* CModel::Put(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const bool& isOnScreen) {

	return Put(CMatrix::ConvPosRotScaleToMtx(pos, rot, scale), modelIdx, isOnScreen);
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
	RNLib::Memory().Release(&m_texIdxs);

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

//========================================
// コンストラクタ
//========================================
CModel::CDrawInfo::CDrawInfo() {

	m_mtx                  = INITD3DXMATRIX;
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

	//----------------------------------------
	// 一時的な描画モード設定を開始
	//----------------------------------------
	RNLib::DrawStateMng().StartTemporarySetMode();

	// [[[ Zテストの設定 ]]]
	RNLib::DrawStateMng().SetZTestMode(m_isZTest, device);

	for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
		
		//----------------------------------------
		// パラメーターに応じた設定
		//----------------------------------------
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

		//----------------------------------------
		// 描画
		//----------------------------------------
		// 表面を描画
		RNLib::DrawStateMng().SetCullingMode(CDrawState::CULLING_MODE::FRONT_SIDE, device);
		modelData.m_mesh->DrawSubset(cntMat);

		//----------------------------------------
		// 輪郭線の描画
		//----------------------------------------
		if (m_isOutLine) {

			// マテリアルの設定
			SetMaterial(device, &mats[cntMat].MatD3D, COLOR_BLACK);

			// 裏面を描画
			RNLib::DrawStateMng().SetCullingMode(CDrawState::CULLING_MODE::BACK_SIDE, device);
			modelData.m_outLineMesh->DrawSubset(cntMat);
		}
	}

	//----------------------------------------
	// 一時的な描画モード設定を終了
	//----------------------------------------
	RNLib::DrawStateMng().EndTemporarySetMode(device);
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

	m_mtx                  = INITD3DXMATRIX;
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
	m_priority             = 0;
}

//========================================
// デストラクタ
//========================================
CModel::CRegistInfo::~CRegistInfo() {

}

//========================================
// パラメーターのクリア処理
//========================================
void CModel::CRegistInfo::ClearParameter(void) {

	m_mtx                  = INITD3DXMATRIX;
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
	m_priority             = 0;
}

//========================================
// 描画情報に変換
//========================================
CModel::CDrawInfo* CModel::CRegistInfo::ConvToDrawInfo(void) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	RNLib::Memory().Alloc(&drawInfo);

	// 情報を代入
	// (基底)
	drawInfo->m_type                 = CDrawInfoBase::TYPE::MODEL;
	drawInfo->m_priority             = m_priority;
	// (継承)
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
		drawInfo->m_distance = CGeometry::FindDistanceToCameraPlane(CMatrix::ConvMtxToPos(m_mtx), RNLib::Camera3D()) - (RNLib::Model().GetData(m_modelIdx).m_radiusMax * scaleMax);
	}

	return drawInfo;
}


//========================================
// マトリックスを設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetMtx(const D3DXMATRIX& mtx) {

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

//========================================
// 優先度設定
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetPriority(const short& priority) {

	if (this == NULL)
		return NULL;

	m_priority = priority;

	return this;
}
