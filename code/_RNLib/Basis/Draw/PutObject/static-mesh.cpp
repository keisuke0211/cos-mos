//========================================
// 
// スタティックメッシュの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]スタティックメッシュクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_StaticMesh::_RNC_StaticMesh() {

	m_meshes         = NULL;
	m_meshNums       = 0;
	m_meshesScreen   = NULL;
	m_meshNumsScreen = 0;
}

//========================================
// デストラクタ
//========================================
_RNC_StaticMesh::~_RNC_StaticMesh() {

}

//========================================
// 初期化処理
//========================================
void _RNC_StaticMesh::Init(const UShort& priorityMax) {

	RNLib::Memory().Alloc(&m_meshes, priorityMax);
	RNLib::Memory().Alloc(&m_meshNums, priorityMax);
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		m_meshes  [cnt] = NULL;
		m_meshNums[cnt] = 0;
	}
	RNLib::Memory().Alloc(&m_meshesScreen, _RNC_DrawMgr::SCREEN_PRIORITY_MAX);
	RNLib::Memory().Alloc(&m_meshNumsScreen, _RNC_DrawMgr::SCREEN_PRIORITY_MAX);
	for (int cnt = 0; cnt < _RNC_DrawMgr::SCREEN_PRIORITY_MAX; cnt++) {
		m_meshesScreen  [cnt] = NULL;
		m_meshNumsScreen[cnt] = 0;
	}
}

//========================================
// 終了処理
//========================================
void _RNC_StaticMesh::Uninit(void) {

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void _RNC_StaticMesh::Update(void) {

}

//========================================
// 描画処理
//========================================
void _RNC_StaticMesh::Draw(Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	// スクリーンかに応じて変数を参照
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (meshes[priority] == NULL)
		return;

	if (meshNums[priority] == NULL)
		return;

	for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {

		if (meshes[priority][cntMesh] == NULL)
			continue;

		// クリッピングIDが対象外であれば折り返す
		if (meshes[priority][cntMesh]->m_clippingID != NONEDATA || isCameraClipping)
			if (meshes[priority][cntMesh]->m_clippingID != cameraID)
				continue;

		meshes[priority][cntMesh]->Draw(device);
	}
}

//========================================
// 解放処理
//========================================
void _RNC_StaticMesh::Release(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++)
		RNLib::Memory().ReleaseDouble(&m_meshes[cnt], m_meshNums[cnt]);
	RNLib::Memory().Release(&m_meshes);
	RNLib::Memory().Release(&m_meshNums);
	for (int cnt = 0; cnt < _RNC_DrawMgr::SCREEN_PRIORITY_MAX; cnt++)
		RNLib::Memory().ReleaseDouble(&m_meshesScreen[cnt], m_meshNumsScreen[cnt]);
	RNLib::Memory().Release(&m_meshesScreen);
	RNLib::Memory().Release(&m_meshNumsScreen);
}

//========================================
// 削除処理
//========================================
void _RNC_StaticMesh::Delete(const bool& isOnScreen) {

	// スクリーンかに応じて変数を参照
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (meshNums == NULL)
		return;

	const UShort& priorityMax = isOnScreen ? _RNC_DrawMgr::SCREEN_PRIORITY_MAX : RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		for (int cntMesh = 0; cntMesh < meshNums[cnt]; cntMesh++) {
			RNLib::Memory().Release(&meshes[cnt][cntMesh]);
		}
		meshNums[cnt] = 0;
	}
}

//========================================
// モデル設定処理
//========================================
void _RNC_StaticMesh::SetModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	SetModel(priority, RNLib::Matrix().ConvPosRotScaleToMtx(pos, rot, scale), modelIdx, col, isOnScreen);
}
void _RNC_StaticMesh::SetModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	SetModel(priority, RNLib::Matrix().ConvPosRotToMtx(pos, rot), modelIdx, col, isOnScreen);
}
void _RNC_StaticMesh::SetModel(const UShort& priority, const Matrix& mtx, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	
	// スクリーンかに応じて変数を参照
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (modelIdx == NONEDATA)
		return;

	// モデルデータを取得
	const _RNC_Model::CData& data = RNLib::Model().GetData(modelIdx);

	for (int cntMat = 0; cntMat < data.m_matNum; cntMat++) {

		bool isSet = false;
		for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {
			CMesh& mesh = *meshes[priority][cntMesh];

			if (mesh.m_texIdx == data.m_texIdxes[cntMat]) {
				if (mesh.SetModel(mtx, modelIdx, col, data.m_texIdxes[cntMat], cntMat))
				{// メッシュの設定に成功した
					isSet = true;
					break;
				}
			}
		}

		if(!isSet){
			// 同じメッシュが無かった時、メッシュを新規作成
			NewCreateMesh(priority, mtx, modelIdx, data.m_texIdxes[cntMat], cntMat, col, isOnScreen);
		}
	}
}

//========================================
// マテリアルモデル設定処理
//========================================
void _RNC_StaticMesh::SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	SetMaterialModel(priority, RNLib::Matrix().ConvPosRotScaleToMtx(pos, rot, scale), modelIdx, texIdx, col, isOnScreen);
}
void _RNC_StaticMesh::SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	SetMaterialModel(priority, RNLib::Matrix().ConvPosRotToMtx(pos, rot), modelIdx, texIdx, col, isOnScreen);
}
void _RNC_StaticMesh::SetMaterialModel(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	
	// スクリーンかに応じて変数を参照
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {
		CMesh& mesh = *meshes[priority][cntMesh];

		if (mesh.m_texIdx == texIdx) {
			if (mesh.SetModel(mtx, modelIdx, col, texIdx, NONEDATA)) 
			{// メッシュの設定に成功した
				return;
			}
		}
	}

	// 同じメッシュが無かった時、メッシュを新規作成
	NewCreateMesh(priority, mtx, modelIdx, texIdx, NONEDATA, col, isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]スタティックメッシュクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// メッシュを新規作成
//========================================
void _RNC_StaticMesh::NewCreateMesh(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const short& matIdx, const Color& col, const bool& isOnScreen) {

	// スクリーンかに応じて変数を参照
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	// メッシュを再確保
	const UShort oldNum = meshNums[priority];
	RNLib::Memory().ReAllocDouble(&meshes[priority], oldNum, ++meshNums[priority]);

	// メッシュを設定
	meshes[priority][oldNum]->m_texIdx = texIdx;
	meshes[priority][oldNum]->SetModel(mtx, modelIdx, col, texIdx, matIdx);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]メッシュクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_StaticMesh::CMesh::CMesh() {

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
_RNC_StaticMesh::CMesh::~CMesh() {

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
void _RNC_StaticMesh::CMesh::Draw(Device& device) {

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
bool _RNC_StaticMesh::CMesh::SetModel(const Matrix& mtx, const short& modelIdx, const Color& col, const short& texIdx, const short& matIdx) {

	// デバイスを取得
	Device& device = RNLib::Window().GetD3DDevice();

	// 過去の数として保存
	const ULong vtxNumOld = m_vtxNum;
	const ULong idxNumOld = m_idxNum;

	// モデルデータを取得
	_RNC_Model::CData& modelData = RNLib::Model().GetData(modelIdx);
	_RNC_Model::CData::MatData matData = {};
	if (matIdx != NONEDATA) {
		matData = modelData.m_matDatas[matIdx];
	}

	// 追加する頂点情報とインデックス情報を取得
	_RNC_Model::Vertex3DInfo* addVtxes = NULL;
	ULong   addVtxNum = (matIdx == NONEDATA) ? modelData.m_vtxNum : modelData.m_matDatas[matIdx].vtxNum;
	RNLib::Model().StoreVtxInfo(mtx, modelIdx, &addVtxes, matIdx);
	ULong*& addIdxes  = (matIdx == NONEDATA) ? modelData.m_idxes : modelData.m_matDatas[matIdx].idxes;
	ULong&  addIdxNum = (matIdx == NONEDATA) ? modelData.m_idxNum : modelData.m_matDatas[matIdx].idxNum;

	// 頂点とインデックス数を加算
	m_vtxNum += addVtxNum;
	m_idxNum += addIdxNum + (m_idxBuff != NULL) * 3;	// ※縮退ポリゴンに使用する分加算

	if (m_vtxNum > USHRT_MAX || m_idxNum > USHRT_MAX || m_vtxNum <= 0 || m_idxNum <= 0) {

		// 追加する頂点情報を解放
		RNLib::Memory().Release(&addVtxes);

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

		// 元の頂点情報を移し替え
		if (m_vtxBuff != NULL) {
			for (UInt cntVtx = 0; cntVtx < vtxNumOld; newVtxes[cntVtx] = vtxes[cntVtx], cntVtx++);
		}

		{// 追加する頂点情報を移し替え
			UInt vtxCount = 0;
			for (UInt cntVtx = vtxNumOld; cntVtx < m_vtxNum; cntVtx++) {
				newVtxes[cntVtx].pos = addVtxes[vtxCount].worldPos;
				newVtxes[cntVtx].nor = addVtxes[vtxCount].worldNor;
				newVtxes[cntVtx].tex = addVtxes[vtxCount].texPos;
				newVtxes[cntVtx].col = (matIdx == NONEDATA) ? D3DCOLOR_RGBA(col.r, col.g, col.b, col.a) : matData.col.GetMixed(col).ConvD3DCOLOR();
				vtxCount++;
			}
		}

		// 追加する頂点情報を解放
		RNLib::Memory().Release(&addVtxes);

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

//========================================
// メッシュ取得処理
//========================================
UShort _RNC_StaticMesh::GetMeshNum(void) {

	const UShort priorityMax = RNLib::DrawMgr().GetPriorityMax();

	UShort totalMeshNum = 0;
	for (int cnt = 0; cnt < priorityMax; cnt++)
		totalMeshNum += m_meshNums[cnt];
	for (int cnt = 0; cnt < _RNC_DrawMgr::SCREEN_PRIORITY_MAX; cnt++)
		totalMeshNum += m_meshNumsScreen[cnt];

	return totalMeshNum;
}