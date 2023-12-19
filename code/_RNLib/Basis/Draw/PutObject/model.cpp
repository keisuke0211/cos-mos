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
//==========| [公開]モデルクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Model::_RNC_Model() {

	m_datas = NULL;
}

//========================================
// デストラクタ
//========================================
_RNC_Model::~_RNC_Model() {
	
}

//========================================
// 初期化処理
//========================================
void _RNC_Model::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Model::Uninit(void) {

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void _RNC_Model::Update(void) {

}

//========================================
// 解放処理
//========================================
void _RNC_Model::Release(void) {

	// データの解放
	RNLib::Memory().ReleaseDouble(&m_datas, m_num);
}

//========================================
// 読み込み処理
//========================================
short _RNC_Model::Load(const char* loadPath, short idx) {

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
			RNLib::Window().Message_ERROR(String("モデルの読み込みに失敗しました。\n%s", loadPath));

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
			const DWORD fvf       = m_datas[idx]->m_mesh->GetFVF();
			const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);

			// 頂点数を取得
			m_datas[idx]->m_vtxNum = m_datas[idx]->m_mesh->GetNumVertices();

			// 頂点データを作成
			RNLib::Memory().Alloc(&m_datas[idx]->m_vtxDatas, m_datas[idx]->m_vtxNum);
			for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
				m_datas[idx]->m_vtxDatas[cntVtx] = {};
				RNLib::Memory().Alloc(&m_datas[idx]->m_vtxDatas[cntVtx].isMats, m_datas[idx]->m_matNum);
				for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++)
					m_datas[idx]->m_vtxDatas[cntVtx].isMats[cntMat] = false;
			}

			// マテリアルデータを生成
			RNLib::Memory().Alloc(&m_datas[idx]->m_matDatas, m_datas[idx]->m_matNum);
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
				m_datas[idx]->m_matDatas[cntMat] = {};
			}

			// 面の数を取得
			m_datas[idx]->m_faceNum = m_datas[idx]->m_mesh->GetNumFaces();

			// 頂点の縁取り情報構造体を定義
			struct VertexOutLine {
				Pos3D totalVec = INITPOS3D;
				ULong count = 0;
			};

			// 頂点の縁取り情報を生成
			VertexOutLine* vertexOutLines = NULL;
			RNLib::Memory().Alloc(&vertexOutLines, m_datas[idx]->m_vtxNum);
			for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

			{// 頂点の縁取り情報を算出
				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 法線方向に加算
				for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// 半径の最大を調べる
						const float dist = RNLib::Geometry().FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// 重なっている頂点位置に輪郭の加算距離を加算
					for (ULong cntVtx2 = 0; cntVtx2 < m_datas[idx]->m_vtxNum; cntVtx2++) {

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

			{// インデックス情報を代入
				// インデックスバッファをロック
				WORD* idxBuff = NULL;
				m_datas[idx]->m_mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idxBuff);

				// アトリビュートバッファをロック
				DWORD* attributeBuffer = NULL;
				m_datas[idx]->m_mesh->LockAttributeBuffer(0, &attributeBuffer);

				// インデックス数を取得
				m_datas[idx]->m_idxNum = m_datas[idx]->m_faceNum * 3;

				// インデックス情報を生成
				RNLib::Memory().Alloc(&m_datas[idx]->m_idxes, m_datas[idx]->m_idxNum);

				// 面毎の番号を生成
				RNLib::Memory().Alloc(&m_datas[idx]->m_faceMatIdxes, m_datas[idx]->m_faceNum);

				for (ULong cntFace = 0; cntFace < m_datas[idx]->m_faceNum; cntFace++) {

					{// インデックス情報を代入
						const ULong startIdxIdx = cntFace * 3;
						for (int cnt = 0; cnt < 3; cnt++) {
							const ULong idxIdx = startIdxIdx + cnt;
							m_datas[idx]->m_idxes[idxIdx] = idxBuff[idxIdx];

							// その頂点のマテリアルに属しているかフラグ
							m_datas[idx]->m_vtxDatas[idxBuff[idxIdx]].isMats[attributeBuffer[cntFace]] = true;
						}
					}

					// マテリアルのインデックス数を加算
					m_datas[idx]->m_matDatas[attributeBuffer[cntFace]].idxNum += 3;

					// 面のマテリアル番号
					m_datas[idx]->m_faceMatIdxes[cntFace] = attributeBuffer[cntFace];
				}

				for (UShort cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {

					// マテリアルの頂点番号列
					ULong* matVtxIdxes = NULL;
					RNLib::Memory().Alloc(&matVtxIdxes, m_datas[idx]->m_vtxNum); {
						ULong matVtxCount = 0;
						for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {

							if (m_datas[idx]->m_vtxDatas[cntVtx].isMats[cntMat]) 
							{// マテリアルが該当する頂点である時、
								// マテリアル毎の頂点数を加算
								m_datas[idx]->m_matDatas[cntMat].vtxNum++;

								// マテリアルの頂点番号列に値を代入
								matVtxIdxes[cntVtx] = matVtxCount++;
							}
							else {
								// マテリアルの頂点番号列に値を初期化
								matVtxIdxes[cntVtx] = 0;
							}
						}
					}

					// マテリアル毎のインデックス情報を生成
					RNLib::Memory().Alloc(&m_datas[idx]->m_matDatas[cntMat].idxes, m_datas[idx]->m_matDatas[cntMat].idxNum);

					// マテリアル毎のインデックス情報を代入していく
					ULong matFaceCount = 0;
					for (ULong cntFace = 0; cntFace < m_datas[idx]->m_faceNum; cntFace++) {
						if (cntMat == attributeBuffer[cntFace]) {
							const ULong faceStartIdx    = cntFace      * 3;
							const ULong matFaceStartIdx = matFaceCount * 3;
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx    ] = (ULong)matVtxIdxes[idxBuff[faceStartIdx    ]];
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx + 1] = (ULong)matVtxIdxes[idxBuff[faceStartIdx + 1]];
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx + 2] = (ULong)matVtxIdxes[idxBuff[faceStartIdx + 2]];
							matFaceCount++;
						}
					}

					RNLib::Memory().Release(&matVtxIdxes);
				}

				// インデックスバッファをアンロック
				m_datas[idx]->m_mesh->UnlockIndexBuffer();

				// アトリビュートバッファをアンロック
				m_datas[idx]->m_mesh->UnlockAttributeBuffer();
			}

			// 輪郭メッシュを生成する
			RNLib::Memory().Alloc(&m_datas[idx]->m_outLineMeshs, RNSettings::GetInfo().modelOutLineAddDistanceDelimiter);

			for (UShort cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {

				// メッシュを複製する
				D3DXCreateMeshFVF(m_datas[idx]->m_faceNum, m_datas[idx]->m_vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);
				m_datas[idx]->m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);

				// 頂点バッファをロック
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMeshs[cntOutLine]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// 頂点位置に加算位置を加算
				for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
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
			RNLib::Memory().Alloc(&m_datas[idx]->m_texIdxes, m_datas[idx]->m_matNum);
			RNLib::Memory().Alloc(&m_datas[idx]->m_texes, m_datas[idx]->m_matNum);

			// テクスチャの読み込み
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
				m_datas[idx]->m_matDatas[cntMat].col.Set(mats[cntMat].MatD3D.Diffuse);
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
void _RNC_Model::StoreVtxInfo(const Matrix& modelMtx, const short& modelIdx, Vertex3DInfo** vtxInfos, const short& matIdx) {

	if (modelIdx == NONEDATA) {
		*vtxInfos = NULL;
		return;
	}

	// 頂点フォーマットのサイズを取得
	const DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_datas[modelIdx]->m_mesh->GetFVF());

	// 頂点バッファをロック
	BYTE* vtxBuff = NULL;
	m_datas[modelIdx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

	// 頂点情報を生成
	RNLib::Memory().Alloc(vtxInfos, matIdx == NONEDATA ? m_datas[modelIdx]->m_vtxNum : m_datas[modelIdx]->m_matDatas[matIdx].vtxNum);

	// テクスチャ座標のオフセットを計算
	const DWORD texCoordOffset = dwSizeFVF - sizeof(float) * 2;  // 2は2次元のテクスチャ座標の要素数

	// 頂点情報を代入
	UInt vtxCount = 0;
	if (matIdx == NONEDATA) {
		for (ULong cntVtx = 0; cntVtx < m_datas[modelIdx]->m_vtxNum; cntVtx++) {
			ExecutionStoreVtxInfo(&(*vtxInfos)[cntVtx], modelMtx, cntVtx, vtxBuff, dwSizeFVF, texCoordOffset);
		}
	}
	else {
		ULong matVtxCount = 0;
		for (ULong cntVtx = 0; cntVtx < m_datas[modelIdx]->m_vtxNum; cntVtx++) {
			if (m_datas[modelIdx]->m_vtxDatas[cntVtx].isMats[matIdx]) {
				ExecutionStoreVtxInfo(&(*vtxInfos)[matVtxCount], modelMtx, cntVtx, vtxBuff, dwSizeFVF, texCoordOffset);
				matVtxCount++;
			}
		}
	}

	// 頂点バッファをアンロック
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// 設置処理
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// モデル番号が無しの時、
	if (modelIdx == NONEDATA)
		return NULL;

	return RNLib::DrawMgr().PutModel(priority, mtx, isOnScreen)
		->SetModel(modelIdx);
}

//========================================
// 設置処理(位置と向きで指定)
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {
	
	return Put(priority, modelIdx, RNLib::Matrix().ConvPosRotToMtx(pos, rot), isOnScreen);
}

//========================================
// 設置処理(位置と向きと拡大倍率で指定)
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen) {

	return Put(priority, modelIdx, RNLib::Matrix().ConvPosRotScaleToMtx(pos, rot, scale), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]モデルクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 頂点情報の格納実行処理
//========================================
void _RNC_Model::ExecutionStoreVtxInfo(Vertex3DInfo* vtx, const Matrix& modelMtx, const ULong& cntVtx, BYTE*& vtxBuff, const DWORD& dwSizeFVF, const DWORD& texCoordOffset) {

	vtx->pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
	vtx->nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

	// ワールドマトリックスを算出
	Matrix worldMtx = RNLib::Matrix().MultiplyMtx(RNLib::Matrix().ConvPosNorToMtx(vtx->pos, vtx->nor), modelMtx);

	vtx->worldPos = RNLib::Matrix().ConvMtxToPos(worldMtx);
	vtx->rot      = RNLib::Geometry().FindVecRot(vtx->nor);
	vtx->worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
	vtx->worldRot = RNLib::Geometry().FindVecRot(vtx->worldNor);
	vtx->texPos   = *(Pos2D*)(vtxBuff + (dwSizeFVF * cntVtx) + texCoordOffset);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| データクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Model::CData::CData() {

	m_texIdxes	   = NULL;
	m_texes		   = NULL;
	m_vtxDatas	   = NULL;
	m_vtxNum	   = 0;
	m_idxes		   = NULL;
	m_idxNum	   = 0;
	m_matBuff	   = NULL;
	m_matDatas	   = NULL;
	m_matNum	   = 0;
	m_faceMatIdxes = NULL;
	m_faceNum	   = 0;
	m_mesh		   = 0;
	m_outLineMeshs = NULL;
	m_radiusMax	   = 0.0f;
}

//========================================
// デストラクタ
//========================================
_RNC_Model::CData::~CData() {

	Release();
}

//========================================
// 解放処理
//========================================
void _RNC_Model::CData::Release(void) {

	// テクスチャの破棄
	RNLib::Memory().Release(&m_texIdxes);
	RNLib::Memory().Release(&m_texes);

	// 頂点データの破棄
	for (ULong cnt = 0; cnt < m_vtxNum; cnt++) {
		RNLib::Memory().Release(&m_vtxDatas[cnt].isMats);
	}
	RNLib::Memory().Release(&m_vtxDatas);

	// インデックスの破棄
	RNLib::Memory().Release(&m_idxes);

	// マテリアルデータを生成
	for (ULong cnt = 0; cnt < m_matNum; cnt++) {
		RNLib::Memory().Release(&m_matDatas[cnt].idxes);
	}
	RNLib::Memory().Release(&m_matDatas);

	// 面毎のマテリアル番号の破棄
	RNLib::Memory().Release(&m_faceMatIdxes);

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
	RNLib::Memory().Release(&m_outLineMeshs);

	// マテリアルの破棄
	if (m_matBuff != NULL) {
		m_matBuff->Release();
		m_matBuff = NULL;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 描画情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
Material _RNC_Model::CDrawInfo::ms_outLineMat = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	0.0f
};

//========================================
// コンストラクタ
//========================================
_RNC_Model::CDrawInfo::CDrawInfo() {

	m_mtx                  = INITMATRIX;
	m_mats                 = NULL;
	m_texes                = NULL;
	m_matNum               = 0;
	m_mesh                 = NULL;
	m_outLineMesh          = NULL;
	m_isScaling            = false;
	m_isZTest              = true;
	m_interpolationMode    = _RNC_DrawState::INTERPOLATION_MODE::NONE;
	m_isLighting           = true;
}

//========================================
// デストラクタ
//========================================
_RNC_Model::CDrawInfo::~CDrawInfo() {

	RNLib::Memory().Release(&m_mats);

	// 拡大倍率に変更があった時、解放する
	if (m_isScaling) {
		if (m_mesh != NULL) {
			m_mesh->Release();
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Model::CRegistInfo::CRegistInfo() {

	ClearParameter();
}

//========================================
// デストラクタ
//========================================
_RNC_Model::CRegistInfo::~CRegistInfo() {

}

//========================================
// パラメータークリア処理
//========================================
void _RNC_Model::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_mtx                  = INITMATRIX;
	m_col                  = COLOR_WHITE;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = true;
	m_outLineIdx           = NONEDATA;
	m_brightnessOfEmissive = 1.0f;
	m_interpolationMode    = _RNC_DrawState::INTERPOLATION_MODE::NONE;
}

//========================================
// 描画情報に変換
//========================================
_RNC_Model::CDrawInfo* _RNC_Model::CRegistInfo::ConvToDrawInfo(Device& device) {

	// 描画情報のメモリ確保
	CDrawInfo* drawInfo = NULL;
	RNLib::Memory().Alloc(&drawInfo);

	// 基底情報を代入
	AssignToDrawInfo(*drawInfo);

	const _RNC_Model::CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// 情報を代入
	drawInfo->m_mtx               = m_mtx;
	drawInfo->m_texes             = modelData.m_texes;
	drawInfo->m_matNum            = modelData.m_matNum;
	drawInfo->m_isZTest           = m_isZTest;
	drawInfo->m_isLighting        = m_isLighting;
	drawInfo->m_interpolationMode = m_interpolationMode;
	drawInfo->m_radiusMax         = modelData.m_radiusMax;{

		// 輪郭メッシュ情報を算出
		if (m_outLineIdx == NONEDATA) {
			drawInfo->m_outLineMesh = NULL;
		}
		else {
			if (m_outLineIdx >= RNSettings::GetInfo().modelOutLineAddDistanceDelimiter) {
				m_outLineIdx = RNSettings::GetInfo().modelOutLineAddDistanceDelimiter - 1;
			}
			drawInfo->m_outLineMesh = modelData.m_outLineMeshs[m_outLineIdx];
			drawInfo->m_radiusMax += RNSettings::GetInfo().modelOutLineAddDistanceInterval * m_outLineIdx;
		}

		const Scale3D scale = RNLib::Matrix().ConvMtxToScale(m_mtx);
		float scaleMax = scale.x;
		if (scaleMax < scale.y)
			scaleMax = scale.y;
		if (scaleMax < scale.z)
			scaleMax = scale.z;

		drawInfo->m_radiusMax *= scaleMax;
	}

	//----------------------------------------
	// マテリアル情報を算出
	//----------------------------------------
	{
		D3DXMATERIAL* mats= (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();
		RNLib::Memory().Alloc(&drawInfo->m_mats, drawInfo->m_matNum);
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
	// メッシュ情報を算出
	//----------------------------------------
	// マトリックスの拡大倍率を取得し、
	const Scale3D scale = RNLib::Matrix().ConvMtxToScale(m_mtx);
	drawInfo->m_isScaling = !RNLib::Number().GetIsEq(scale.x, 1.0f, 0.01f) ? true : !RNLib::Number().GetIsEq(scale.y, 1.0f, 0.01f) ? true : !RNLib::Number().GetIsEq(scale.z, 1.0f, 0.01f);
	if (drawInfo->m_isScaling)
	{// もし拡大倍率に変更があった時、
		const DWORD fvf       = modelData.m_mesh->GetFVF();
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);
		const ULong faceNum   = modelData.m_mesh->GetNumFaces();

		// メッシュを複製する
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);

		// メッシュがNULLであれば、
		if (drawInfo->m_mesh == NULL) {
			RNLib::Memory().Release(&drawInfo);
			return NULL;
		}

		// 頂点バッファをロック
		BYTE* vtxBuff = NULL;
		drawInfo->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// 法線方向に加算
		for (ULong cntVtx = 0; cntVtx < modelData.m_vtxNum; cntVtx++) {
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
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetClippingCamera(CCamera& camera) {

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
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetClippingCamera(const short& ID) {

	if (this == NULL)
		return NULL;

	m_clippingID = ID;

	return this;
}

//========================================
// マトリックスを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// 色を設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// モデルを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetModel(const short& modelIdx) {

	if (this == NULL)
		return NULL;

	m_modelIdx = modelIdx;

	return this;
}

//========================================
// テクスチャを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetTex(const short& texIdx) {

	if (this == NULL)
		return NULL;

	m_texIdx = texIdx;

	return this;
}

//========================================
// Zテストを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZTest = isZTest;

	return this;
}

//========================================
// ライティングを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// 輪郭線を設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetOutLineIdx(const UShort& outLineIdx) {

	if (this == NULL)
		return NULL;

	m_outLineIdx = outLineIdx;

	return this;
}

//========================================
// 発光部分の明るさを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetBrightnessOfEmissive(const float& brightnessOfEmissive) {

	if (this == NULL)
		return NULL;

	m_brightnessOfEmissive = brightnessOfEmissive;

	return this;
}

//========================================
// 補間モードを設定
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}