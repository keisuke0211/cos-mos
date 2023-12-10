//========================================
// 
// マテリアルメッシュの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]マテリアルメッシュクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CMatMesh::CMatMesh() {

	m_meshes   = NULL;
	m_meshNums = NULL;
}

//========================================
// デストラクタ
//========================================
CMatMesh::~CMatMesh() {

}

//========================================
// 初期化処理
//========================================
void CMatMesh::Init(const UShort& priorityMax) {

	CMemory::Alloc(&m_meshes, priorityMax);
	CMemory::Alloc(&m_meshNums, priorityMax);
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		m_meshes[cnt] = NULL;
		m_meshNums[cnt] = 0;
	}
}

//========================================
// 終了処理
//========================================
void CMatMesh::Uninit(void) {

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void CMatMesh::Update(void) {

}

//========================================
// 描画処理
//========================================
void CMatMesh::Draw(Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	if (m_meshNums == NULL)
		return;

	// ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &INITMATRIX);

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_3D);

	for (int cntMesh = 0; cntMesh < m_meshNums[priority]; cntMesh++) {

		if (m_meshes[priority][cntMesh] == NULL)
			continue;

		if (m_meshes[priority][cntMesh]->m_isOnScreen != isOnScreen)
			continue;

		// クリッピングIDが対象外であれば折り返す
		if (m_meshes[priority][cntMesh]->m_clippingID != NONEDATA || isCameraClipping)
			if (m_meshes[priority][cntMesh]->m_clippingID != cameraID)
				continue;

		m_meshes[priority][cntMesh]->Draw(device);
	}
}

//========================================
// 解放処理
//========================================
void CMatMesh::Release(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++)
		CMemory::ReleaseDouble(&m_meshes[cnt], m_meshNums[cnt]);
	CMemory::Release(&m_meshes);
	CMemory::Release(&m_meshNums);
}

//========================================
// 削除処理
//========================================
void CMatMesh::Delete(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		for (int cntMesh = 0; cntMesh < m_meshNums[cnt]; cntMesh++) {
			CMemory::Release(&m_meshes[cnt][cntMesh]);
		}
		m_meshNums[cnt] = 0;
	}
}

//========================================
// メッシュ設定処理
//========================================
void CMatMesh::SetMesh(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {

	for (int cntMesh = 0; cntMesh < m_meshNums[priority]; cntMesh++) {
		CMesh& mesh = *m_meshes[priority][cntMesh];

		if (mesh.m_texIdx == texIdx && mesh.m_isOnScreen == isOnScreen) {
			if (mesh.SetMesh(mtx, modelIdx, col)) 
			{// メッシュの設定に成功した
				return;
			}
		}
	}

	// ~~~ 同じメッシュが無かった ~~~
	
	// メッシュを再確保
	const UShort oldNum = m_meshNums[priority];
	CMemory::ReAllocDouble(&m_meshes[priority], oldNum, ++m_meshNums[priority]);

	// メッシュを設定
	m_meshes[priority][oldNum]->m_texIdx = texIdx;
	m_meshes[priority][oldNum]->m_isOnScreen = isOnScreen;
	m_meshes[priority][oldNum]->SetMesh(mtx, modelIdx, col);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]メッシュクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CMatMesh::CMesh::CMesh() {

	m_texIdx     = NONEDATA;
	m_clippingID = NONEDATA;
	m_vtxBuff    = NULL;
	m_vtxNum     = 0;
	m_idxBuff    = NULL;
	m_idxNum     = 0;
}

//========================================
// デストラクタ
//========================================
CMatMesh::CMesh::~CMesh() {

	if (m_vtxBuff != NULL) {
		m_vtxBuff->Release();
		m_vtxBuff = NULL;
	}

	if (m_idxBuff != NULL) {
		m_idxBuff->Release();
		m_idxBuff = NULL;
	}
}

//========================================
// 描画処理
//========================================
void CMatMesh::CMesh::Draw(Device& device) {

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(Vertex3D));

	// インデックスバッファをデータストリームに設定
	device->SetIndices(m_idxBuff);
	
	// [[[ テクスチャの設定 ]]]
	Polygon2DAnd3D::SetTexture(device, (void*)(&m_texIdx), Polygon2DAnd3D::TEX_TYPE::IDX);

	// ポリゴンの描画
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vtxNum, 0, m_idxNum / 3);
}

//========================================
// メッシュ設定処理
//========================================
bool CMatMesh::CMesh::SetMesh(const Matrix& mtx, const short& modelIdx, const Color& col) {

	// デバイスを取得
	Device& device = RNLib::Window().GetD3DDevice();

	// 過去の数として保存
	const UInt vtxNumOld = m_vtxNum;
	const UInt idxNumOld = m_idxNum;

	// モデルデータを取得
	CModel::CData& modelData = RNLib::Model().GetData(modelIdx);

	// 追加する頂点情報とインデックス情報を取得
	CModel::Vertex3DInfo* addVtxes = NULL;
	UInt addVtxNum = 0;
	RNLib::Model().StoreVtxInfo(mtx, modelIdx, &addVtxNum, &addVtxes);
	UShort*& addIdxes  = modelData.m_idxes;
	UInt&    addIdxNum = modelData.m_idxNum;

	// 頂点とインデックス数を加算
	m_vtxNum += addVtxNum;
	m_idxNum += addIdxNum + (m_idxBuff != NULL) * 3;	// ※縮退ポリゴンに使用する分加算

	if (m_vtxNum > USHRT_MAX || m_idxNum > USHRT_MAX) {

		// 追加する頂点情報を解放
		CMemory::Release(&addVtxes);

		return false;
	}

	// 頂点バッファとインデックスバッファを新しく生成
	VertexBuffer newVtxBuff = NULL;
	device->CreateVertexBuffer(sizeof(Vertex3D) * m_vtxNum, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &newVtxBuff, NULL);
	IndexBuffer newIdxBuff = NULL;
	device->CreateIndexBuffer(sizeof(WORD) * m_idxNum, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &newIdxBuff, NULL);
	
	{// [[[ 情報の上書き ]]]

		// 頂点バッファをロック
		Vertex3D* vtxes = NULL;
		Vertex3D* newVtxes = NULL;
		if (m_vtxBuff != NULL)
			m_vtxBuff->Lock(0, 0, (void**)&vtxes, 0);
		newVtxBuff->Lock(0, 0, (void**)&newVtxes, 0);

		// 頂点情報を移し替え
		if (m_vtxBuff != NULL)
			for (UInt cntVtx = 0; cntVtx < vtxNumOld; newVtxes[cntVtx] = vtxes[cntVtx], cntVtx++);

		{// 新しい頂点情報の設定
			UInt vtxCount = 0;
			for (UInt cntVtx = vtxNumOld; cntVtx < m_vtxNum; cntVtx++) {
				newVtxes[cntVtx].pos = addVtxes[vtxCount].worldPos;
				newVtxes[cntVtx].nor = addVtxes[vtxCount].worldNor;
				newVtxes[cntVtx].tex = addVtxes[vtxCount].texPos;
				newVtxes[cntVtx].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
				vtxCount++;
			}
		}

		// 追加する頂点情報を解放
		CMemory::Release(&addVtxes);

		// 頂点バッファをアンロック
		if (m_vtxBuff != NULL)
			m_vtxBuff->Unlock();
		newVtxBuff->Unlock();

		// インデックスバッファをロック
		WORD* idxes = NULL;
		WORD* newIdxes = NULL;
		if (m_idxBuff != NULL)
			m_idxBuff->Lock(0, 0, (void**)&idxes, 0);
		newIdxBuff->Lock(0, 0, (void**)&newIdxes, 0);

		// インデックス情報を移し替え
		if (m_idxBuff != NULL)
			for (UInt cntIdx = 0; cntIdx < idxNumOld; newIdxes[cntIdx] = idxes[cntIdx], cntIdx++);

		{// 新しいインデックス情報の設定
			UInt idxCount = 0;
			for (UInt cntIdx = idxNumOld + (m_idxBuff != NULL) * 3; cntIdx < m_idxNum; cntIdx++) {
				newIdxes[cntIdx] = vtxNumOld + addIdxes[idxCount];
				idxCount++;
			}
		}

		// 縮退インデックスの設定
		if (m_idxBuff != NULL) {
			newIdxes[idxNumOld] = newIdxes[idxNumOld - 1];
			newIdxes[idxNumOld + 1] = newIdxes[idxNumOld + 2];
			newIdxes[idxNumOld + 2] = newIdxes[idxNumOld + 2];
		}

		// インデックスバッファをアンロック
		if (m_idxBuff != NULL)
			m_idxBuff ->Unlock();
		newIdxBuff->Unlock();
	}

	// 古い頂点バッファとインデックスバッファを解放
	if (m_vtxBuff != NULL) {
		m_vtxBuff->Release();
	}
	if (m_idxBuff != NULL) {
		m_idxBuff->Release();
	}

	// 新しい頂点バッファとインデックスバッファを代入
	m_vtxBuff = newVtxBuff;
	m_idxBuff = newIdxBuff;

	return true;
}