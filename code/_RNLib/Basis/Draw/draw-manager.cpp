//========================================
// 
// 描画処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画マネージャークラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
CDrawMgr::PROCESS_STATE           CDrawMgr::ms_processState = PROCESS_STATE::REGIST_ACCEPT;
CDrawMgr::CRegistInfoSum*         CDrawMgr::ms_resistInfoSum		= NULL;
CDrawMgr::CRegistInfoSum*         CDrawMgr::ms_resistInfoSumScreen	= NULL;
CDrawMgr::CDrawInfoSum*           CDrawMgr::ms_drawInfoSum			= NULL;
CDrawMgr::CDrawInfoSum*           CDrawMgr::ms_drawInfoSumOvr		= NULL;
CDrawMgr::CDrawInfoSum*           CDrawMgr::ms_drawInfoSumScreen	= NULL;
CDrawMgr::CDrawInfoSum*           CDrawMgr::ms_drawInfoSumScreenOvr = NULL;
std::thread                       CDrawMgr::ms_mainLoopTh(MainLoop);
UShort                            CDrawMgr::ms_priorityMax;

//========================================
// コンストラクタ
//========================================
CDrawMgr::CDrawMgr() {

	m_reAllocCount     = 0;
	m_oldDrawFPS       = 0;
	m_waitMilliseconds = WAIT_MILLISECONDS_MIN;
}

//========================================
// デストラクタ
//========================================
CDrawMgr::~CDrawMgr() {

}

//========================================
// 初期化処理
//========================================
void CDrawMgr::Init(const UShort& priorityMax) {

	// 最大優先度を保存
	if (priorityMax <= 0) {
		assert(false);
		ms_priorityMax = 1;
	}
	else {
		ms_priorityMax = priorityMax;
	}

	// 登録/描画情報のメモリ確保
	CMemory::Alloc(&ms_drawInfoSum		   , ms_priorityMax);
	CMemory::Alloc(&ms_drawInfoSumOvr	   , ms_priorityMax);
	CMemory::Alloc(&ms_drawInfoSumScreen   , ms_priorityMax);
	CMemory::Alloc(&ms_drawInfoSumScreenOvr, ms_priorityMax);
	CMemory::Alloc(&ms_resistInfoSum	   , ms_priorityMax);
	CMemory::Alloc(&ms_resistInfoSumScreen , ms_priorityMax);

	// 登録情報の初期メモリ確保
	for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
		ms_resistInfoSum      [cntPriority].InitAlloc();
		ms_resistInfoSumScreen[cntPriority].InitAlloc();
	}

	// 頂点バッファの初期生成
	CPolygon2D::CDrawInfo::InitCreateVertexBuffer();
	CPolygon3D::CDrawInfo::InitCreateVertexBuffer();
}

//========================================
// 終了処理
//========================================
void CDrawMgr::Uninit(void) {

	// メインループスレッドを斬り離す
	ms_mainLoopTh.detach();

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void CDrawMgr::Update(void) {

}

//========================================
// 解放処理
//========================================
void CDrawMgr::Release(void) {

	for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {

		// 描画情報を解放
		ms_drawInfoSum         [cntPriority].Release();
		ms_drawInfoSumOvr      [cntPriority].Release();
		ms_drawInfoSumScreen   [cntPriority].Release();
		ms_drawInfoSumScreenOvr[cntPriority].Release();
		CMemory::Release(&ms_drawInfoSum         [cntPriority].m_drawInfos);
		CMemory::Release(&ms_drawInfoSumOvr      [cntPriority].m_drawInfos);
		CMemory::Release(&ms_drawInfoSumScreen   [cntPriority].m_drawInfos);
		CMemory::Release(&ms_drawInfoSumScreenOvr[cntPriority].m_drawInfos);

		// 登録情報を解放
		ms_resistInfoSum      [cntPriority].Release();
		ms_resistInfoSumScreen[cntPriority].Release();
	}

	// 登録/描画情報のメモリ解放
	CMemory::Release(&ms_drawInfoSum);
	CMemory::Release(&ms_drawInfoSumOvr);
	CMemory::Release(&ms_drawInfoSumScreen);
	CMemory::Release(&ms_drawInfoSumScreenOvr);
	CMemory::Release(&ms_resistInfoSum);
	CMemory::Release(&ms_resistInfoSumScreen);

	// 頂点バッファを解放する
	CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
	CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
}

//========================================
// 登録情報適用待ち開始
//========================================
void CDrawMgr::StartRegistInfoApplyWait(void) {

	if (ms_processState == PROCESS_STATE::REGIST_ACCEPT)
	{// [ 登録受付 ]の時、
		// 削除済みカメラを解放する
		RNLib::CameraMgr().ReleaseDeletedCamera();

		// [ 登録情報適用待ち ]にする
		ms_processState = PROCESS_STATE::REGIST_INFO_APPLY_WAIT;
	}
}

//========================================
// 描画開始処理
//========================================
bool CDrawMgr::StartDraw(void) {

	// まだ描画情報適用待ちの時、
	if (ms_processState == PROCESS_STATE::REGIST_INFO_APPLY_WAIT) {

		// 更に待っても適用が間に合わなかった時、待ち時間を加算する
		std::this_thread::sleep_for(std::chrono::milliseconds(m_waitMilliseconds));
		if (ms_processState == PROCESS_STATE::REGIST_INFO_APPLY_WAIT && m_waitMilliseconds < WAIT_MILLISECONDS_MAX) {
			m_waitMilliseconds++;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	if ((ms_processState == PROCESS_STATE::DRAW_INFO_SWAP_WAIT && !RNSystem::GetSpace3DStop()) || RNSystem::GetSceneSwap())
	{// [ 描画入れ替え待ち ]の時、

		for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {

			// 描画情報を上書きする
			ms_drawInfoSum      [cntPriority].Overwrite(&ms_drawInfoSumOvr      [cntPriority]);
			ms_drawInfoSumScreen[cntPriority].Overwrite(&ms_drawInfoSumScreenOvr[cntPriority]);

			// 不要メモリ破棄の為、再確保
			ms_resistInfoSum      [cntPriority].ReAlloc();
			ms_resistInfoSumScreen[cntPriority].ReAlloc();
		}

		// ポリゴン2D
		if (CPolygon2D::CDrawInfo::m_allocPower > POLYGON2D_ALLOC_BASE_POWER) {

			for (int cntAlloc = POLYGON2D_ALLOC_BASE_POWER; cntAlloc < CPolygon2D::CDrawInfo::m_allocPower; cntAlloc++) {
				const UShort allocLine = pow(2, cntAlloc);

				if (CPolygon2D::CDrawInfo::m_idxCount < allocLine)
				{// 確保ラインよりも数が少ない時、
					// その確保ライン分確保し直す
					CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
					CPolygon2D::CDrawInfo::CreateVertexBuffer(allocLine);

					// 確保した数/べき乗数を保存
					CPolygon2D::CDrawInfo::m_allocNum = allocLine;
					CPolygon2D::CDrawInfo::m_allocPower = cntAlloc;

					break;
				}
			}
		}

		// ポリゴン3D
		if (CPolygon3D::CDrawInfo::m_allocPower > POLYGON3D_ALLOC_BASE_POWER) {

			for (int cntAlloc = POLYGON3D_ALLOC_BASE_POWER; cntAlloc < CPolygon3D::CDrawInfo::m_allocPower; cntAlloc++) {
				const UShort allocLine = pow(2, cntAlloc);

				if (CPolygon3D::CDrawInfo::m_idxCount < allocLine)
				{// 確保ラインよりも数が少ない時、
					// その確保ライン分確保し直す
					CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
					CPolygon3D::CDrawInfo::CreateVertexBuffer(allocLine);

					// 確保した数/べき乗数を保存
					CPolygon3D::CDrawInfo::m_allocNum = allocLine;
					CPolygon3D::CDrawInfo::m_allocPower = cntAlloc;

					break;
				}
			}
		}

		// 番号カウント初期化
		CPolygon2D::CDrawInfo::m_idxCount = 0;
		CPolygon3D::CDrawInfo::m_idxCount = 0;

		// 頂点情報を代入
		AssignVertexInfo();

		// 描画FPSカウントを加算
		RNSystem::AddDrawFPSCount();

		// [ 登録受付 ]にする
		ms_processState = PROCESS_STATE::REGIST_ACCEPT;

		return true;
	}

	return false;
}

//========================================
// 描画処理
//========================================
void CDrawMgr::Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	// ビューマトリックスを取得
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// 描画していく
	if (isOnScreen) {
		for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
			for (int cntDrawInfo = 0; cntDrawInfo < ms_drawInfoSumScreen[cntPriority].m_drawInfoNum; cntDrawInfo++) {
				if (ms_drawInfoSumScreen[cntPriority].m_drawInfos[cntDrawInfo] != NULL) {

					// クリッピングIDが対象外であれば折り返す
					if (ms_drawInfoSumScreen[cntPriority].m_drawInfos[cntDrawInfo]->m_clippingID != NONEDATA || isCameraClipping)
						if (ms_drawInfoSumScreen[cntPriority].m_drawInfos[cntDrawInfo]->m_clippingID != cameraID)
							continue;

					ms_drawInfoSumScreen[cntPriority].m_drawInfos[cntDrawInfo]->Draw(device, viewMtx);
				}
			}
		}
	}
	else {
		for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
			for (int cntDrawInfo = 0; cntDrawInfo < ms_drawInfoSum[cntPriority].m_drawInfoNum; cntDrawInfo++) {
				if (ms_drawInfoSum[cntPriority].m_drawInfos[cntDrawInfo] != NULL) {

					// クリッピングIDが対象外であれば折り返す
					if (ms_drawInfoSum[cntPriority].m_drawInfos[cntDrawInfo]->m_clippingID != NONEDATA || isCameraClipping)
						if (ms_drawInfoSum[cntPriority].m_drawInfos[cntDrawInfo]->m_clippingID != cameraID)
							continue;

					ms_drawInfoSum[cntPriority].m_drawInfos[cntDrawInfo]->Draw(device, viewMtx);
				}
			}
		}
	}
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::PutPolygon2D(const UShort& priority, const Pos3D& pos, const float& angle, const bool& isOnScreen) {

	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (CPolygon2D::CDrawInfo::m_idxCount == CPolygon2D::CDrawInfo::m_allocNum) {
		CPolygon2D::CDrawInfo::m_allocPower++;
		CPolygon2D::CDrawInfo::m_allocNum = pow(2, CPolygon2D::CDrawInfo::m_allocPower);
		CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon2D::CDrawInfo::CreateVertexBuffer(CPolygon2D::CDrawInfo::m_allocNum);
	}

	// 登録情報
	CPolygon2D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon2D(ms_resistInfoSumScreen[priority]) :
		RegistPolygon2D(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetIdx(CPolygon2D::CDrawInfo::m_idxCount++);
	registInfo->SetPos(pos);
	registInfo->SetAngle(angle);

	return registInfo;
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (CPolygon2D::CDrawInfo::m_idxCount == CPolygon2D::CDrawInfo::m_allocNum) {
		CPolygon2D::CDrawInfo::m_allocPower++;
		CPolygon2D::CDrawInfo::m_allocNum = pow(2, CPolygon2D::CDrawInfo::m_allocPower);
		CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon2D::CDrawInfo::CreateVertexBuffer(CPolygon2D::CDrawInfo::m_allocNum);
	}

	// 登録情報
	CPolygon2D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon2D(ms_resistInfoSumScreen[priority]):
		RegistPolygon2D(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetIdx(CPolygon2D::CDrawInfo::m_idxCount++);

	return registInfo;
}

//========================================
// 設置処理(ポリゴン3D)
//========================================
CPolygon3D::CRegistInfo* CDrawMgr::PutPolygon3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {
	
	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (CPolygon3D::CDrawInfo::m_idxCount == CPolygon3D::CDrawInfo::m_allocNum) {
		CPolygon3D::CDrawInfo::m_allocPower++;
		CPolygon3D::CDrawInfo::m_allocNum = pow(2, CPolygon3D::CDrawInfo::m_allocPower);
		CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon3D::CDrawInfo::CreateVertexBuffer(CPolygon3D::CDrawInfo::m_allocNum);
	}

	// 登録情報
	CPolygon3D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon3D(ms_resistInfoSumScreen[priority]) :
		RegistPolygon3D(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetIdx(CPolygon3D::CDrawInfo::m_idxCount++);
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// 設置処理(テキスト2D)
//========================================
CText2D::CRegistInfo* CDrawMgr::PutText2D(const UShort& priority, const Pos3D& pos, const float& angle, const bool& isOnScreen) {

	// 登録情報
	CText2D::CRegistInfo* registInfo = isOnScreen ?
		RegistText2D(ms_resistInfoSumScreen[priority]) :
		RegistText2D(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetPos(pos);
	registInfo->SetAngle(angle);

	return registInfo;
}

//========================================
// 設置処理(テキスト3D)
//========================================
CText3D::CRegistInfo* CDrawMgr::PutText3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// 登録情報
	CText3D::CRegistInfo* registInfo = isOnScreen ?
		RegistText3D(ms_resistInfoSumScreen[priority]) :
		RegistText3D(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// 設置処理(モデル)
//========================================
CModel::CRegistInfo* CDrawMgr::PutModel(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (ms_processState != PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	// 登録情報
	CModel::CRegistInfo* registInfo = isOnScreen ?
		RegistModel(ms_resistInfoSumScreen[priority]) :
		RegistModel(ms_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetMtx(mtx);

	return registInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]描画マネージャークラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 頂点情報代入処理
//========================================
void CDrawMgr::AssignVertexInfo(void) {

	// 頂点2D情報に変換
	if (CPolygon2D::CDrawInfo::m_vtxBuff != NULL) {

		// 頂点バッファをロック
		Vertex2D* vtxs = NULL;
		CPolygon2D::CDrawInfo::m_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
			ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSum[cntPriority]);
			ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSumScreen[cntPriority]);
		}

		// 頂点バッファをアンロック
		CPolygon2D::CDrawInfo::m_vtxBuff->Unlock();
	}

	// 頂点3D情報に変換
	if (CPolygon3D::CDrawInfo::m_vtxBuff != NULL) {

		// 頂点バッファをロック
		Vertex3D* vtxs = NULL;
		CPolygon3D::CDrawInfo::m_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
			ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSum[cntPriority]);
			ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSumScreen[cntPriority]);
		}

		// 頂点バッファをアンロック
		CPolygon3D::CDrawInfo::m_vtxBuff->Unlock();
	}
}

//========================================
// 描画情報を頂点2D情報に変換
//========================================
void CDrawMgr::ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.m_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.m_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON2D)
		{// カウントの描画情報がポリゴン2Dの時、
			// 頂点情報を代入
			CPolygon2D::CDrawInfo& drawInfo = (CPolygon2D::CDrawInfo&)*drawInfoSum.m_drawInfos[cntDrawInfo];
			int vtxStartIdx = 4 * drawInfo.m_idx;

			for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
				int vtxIdx = vtxStartIdx + cntVtx;
				assert(vtxIdx < CPolygon2D::CDrawInfo::m_allocNum * 4);
				vtxs[vtxIdx] = drawInfo.m_vtxs[cntVtx];
			}
		}
	}
}

//========================================
// 描画情報を頂点3D情報に変換
//========================================
void CDrawMgr::ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.m_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.m_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON3D)
		{// カウントの描画情報がポリゴン2Dの時、
			// 頂点情報を代入
			CPolygon3D::CDrawInfo& drawInfo((CPolygon3D::CDrawInfo&)*drawInfoSum.m_drawInfos[cntDrawInfo]);
			int                    vtxStartIdx(4 * drawInfo.m_idx);

			for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
				int vtxIdx(vtxStartIdx + cntVtx);
				assert(vtxIdx < CPolygon3D::CDrawInfo::m_allocNum * 4);
				vtxs[vtxIdx] = drawInfo.m_vtxs[cntVtx];
			}
		}
	}
}

//========================================
// 登録処理(ポリゴン2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::RegistPolygon2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon2DRegistInfoNum == resistInfo.m_polygon2DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_polygon2DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_polygon2DRegistInfoAllocNum;
		resistInfo.m_polygon2DRegistInfoAllocNum = pow(2, resistInfo.m_polygon2DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		CMemory::ReAllocDouble(&resistInfo.m_polygon2DRegistInfos, oldAllocNum, resistInfo.m_polygon2DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum++];
}

//========================================
// 登録処理(ポリゴン3D)
//========================================
CPolygon3D::CRegistInfo* CDrawMgr::RegistPolygon3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon3DRegistInfoNum == resistInfo.m_polygon3DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_polygon3DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_polygon3DRegistInfoAllocNum;
		resistInfo.m_polygon3DRegistInfoAllocNum = pow(2, resistInfo.m_polygon3DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		CMemory::ReAllocDouble(&resistInfo.m_polygon3DRegistInfos, oldAllocNum, resistInfo.m_polygon3DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum++];
}

//========================================
// 登録処理(テキスト2D)
//========================================
CText2D::CRegistInfo* CDrawMgr::RegistText2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text2DRegistInfoNum == resistInfo.m_text2DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_text2DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_text2DRegistInfoAllocNum;
		resistInfo.m_text2DRegistInfoAllocNum = pow(2, resistInfo.m_text2DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		CMemory::ReAllocDouble(&resistInfo.m_text2DRegistInfos, oldAllocNum, resistInfo.m_text2DRegistInfoAllocNum, true);
	}

	resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum++];
}

//========================================
// 登録処理(テキスト3D)
//========================================
CText3D::CRegistInfo* CDrawMgr::RegistText3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text3DRegistInfoNum == resistInfo.m_text3DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_text3DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_text3DRegistInfoAllocNum;
		resistInfo.m_text3DRegistInfoAllocNum = pow(2, resistInfo.m_text3DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		CMemory::ReAllocDouble(&resistInfo.m_text3DRegistInfos, oldAllocNum, resistInfo.m_text3DRegistInfoAllocNum, true);
	}

	resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum++];
}

//========================================
// 登録処理(モデル)
//========================================
CModel::CRegistInfo* CDrawMgr::RegistModel(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_modelRegistInfoNum == resistInfo.m_modelRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_modelRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_modelRegistInfoAllocNum;
		resistInfo.m_modelRegistInfoAllocNum = pow(2, resistInfo.m_modelRegistInfoAllocPower);

		// その確保ライン分確保し直す
		CMemory::ReAllocDouble(&resistInfo.m_modelRegistInfos, oldAllocNum, resistInfo.m_modelRegistInfoAllocNum, true);
	}

	resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum]->ClearParameter();
	return resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum++];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]描画マネージャークラスのスレッド用メンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]メインループ
//========================================
void CDrawMgr::MainLoop(void) {
	while (true) {
		if (ms_processState == PROCESS_STATE::REGIST_INFO_APPLY_WAIT)
		{// [ 登録情報適用待ち ]の時、
			for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
				// 登録情報を元に設置する
				PutBasedRegistInfo(ms_resistInfoSum      [cntPriority], cntPriority, false);
				PutBasedRegistInfo(ms_resistInfoSumScreen[cntPriority], cntPriority, true);

				// 登録情報を描画情報に変換する
				ConvRegistInfoToDrawInfo(ms_resistInfoSum      [cntPriority], ms_drawInfoSumOvr      [cntPriority]);
				ConvRegistInfoToDrawInfo(ms_resistInfoSumScreen[cntPriority], ms_drawInfoSumScreenOvr[cntPriority]);
			}

			// [ 描画情報入れ替え待ち ]にする
			ms_processState = PROCESS_STATE::DRAW_INFO_SWAP_WAIT;
		}

		// ※全力で回さないように0秒スリープ
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

//========================================
// [静的]登録情報を元に設置する
//========================================
void CDrawMgr::PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen) {

	//----------------------------------------
	// テキスト3Dからポリゴン3Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text3DRegistInfoNum; cnt++) {
		resistInfoSum.m_text3DRegistInfos[cnt]->PutPolygon3D(priority, isOnScreen);
	}

	resistInfoSum.m_text3DDrawNum = resistInfoSum.m_text3DRegistInfoNum;
	resistInfoSum.m_text3DRegistInfoNum = 0;

	//----------------------------------------
	// テキスト2Dからポリゴン2Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text2DRegistInfoNum; cnt++) {
		resistInfoSum.m_text2DRegistInfos[cnt]->PutPolygon2D(priority, isOnScreen);
	}

	resistInfoSum.m_text2DDrawNum = resistInfoSum.m_text2DRegistInfoNum;
	resistInfoSum.m_text2DRegistInfoNum = 0;
}

//========================================
// [静的]登録情報を描画情報に変換する
//========================================
void CDrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum) {

	int cntDrawInfo = 0;

	{// 描画情報のメモリ確保
		drawInfoSum.m_drawInfoNum =
			resistInfoSum.m_polygon2DRegistInfoNum +	// ポリゴン2D
			resistInfoSum.m_polygon3DRegistInfoNum +	// ポリゴン3D
			resistInfoSum.m_modelRegistInfoNum;			// モデル
		CMemory::Alloc<CDrawInfoBase*>(&drawInfoSum.m_drawInfos, drawInfoSum.m_drawInfoNum, true);
	}

	//----------------------------------------
	// ポリゴン3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.m_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon3DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon3DDrawNum = resistInfoSum.m_polygon3DRegistInfoNum;
	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// モデル
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.m_drawInfos[cntDrawInfo] = resistInfoSum.m_modelRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_modelDrawNum = resistInfoSum.m_modelRegistInfoNum;
	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.m_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon2DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon2DDrawNum = resistInfoSum.m_polygon2DRegistInfoNum;
	resistInfoSum.m_polygon2DRegistInfoNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画情報総括クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDrawMgr::CDrawInfoSum::CDrawInfoSum() {

	m_drawInfos   = NULL;
	m_drawInfoNum = 0;
}

//========================================
// デストラクタ
//========================================
CDrawMgr::CDrawInfoSum::~CDrawInfoSum() {

}

//========================================
// 解放処理
//========================================
void CDrawMgr::CDrawInfoSum::Release(void) {

	// 描画情報を解放
	if (m_drawInfos != NULL) {
		for (int cnt = 0; cnt < m_drawInfoNum; cnt++) {
			CMemory::Release(&m_drawInfos[cnt]);
		}
		CMemory::Release(&m_drawInfos);
	}
}

//========================================
// 上書き処理
//========================================
void CDrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

	// 解放処理
	Release();

	// 上書き
	m_drawInfos         = pOvr->m_drawInfos;
	m_drawInfoNum       = pOvr->m_drawInfoNum;
	pOvr->m_drawInfos   = NULL;
	pOvr->m_drawInfoNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録情報情報総括クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDrawMgr::CRegistInfoSum::CRegistInfoSum() {

	m_polygon2DRegistInfos          = NULL;
	m_polygon2DRegistInfoNum        = 0;
	m_polygon2DRegistInfoAllocPower = 0;
	m_polygon2DRegistInfoAllocNum   = 0;
	m_polygon2DDrawNum              = 0;
	m_polygon3DRegistInfos          = NULL;
	m_polygon3DRegistInfoNum        = 0;
	m_polygon3DRegistInfoAllocPower = 0;
	m_polygon3DRegistInfoAllocNum   = 0;
	m_polygon3DDrawNum              = 0;
	m_text2DRegistInfos             = NULL;
	m_text2DRegistInfoNum           = 0;
	m_text2DRegistInfoAllocPower    = 0;
	m_text2DRegistInfoAllocNum      = 0;
	m_text2DDrawNum                 = 0;
	m_text3DRegistInfos             = NULL;
	m_text3DRegistInfoNum           = 0;
	m_text3DRegistInfoAllocPower    = 0;
	m_text3DRegistInfoAllocNum      = 0;
	m_text3DDrawNum                 = 0;
	m_modelRegistInfos              = NULL;
	m_modelRegistInfoNum            = 0;
	m_modelRegistInfoAllocPower     = 0;
	m_modelRegistInfoAllocNum       = 0;
	m_modelDrawNum                  = 0;
}

//========================================
// デストラクタ
//========================================
CDrawMgr::CRegistInfoSum::~CRegistInfoSum() {

}

//========================================
// 初期確保処理
//========================================
void CDrawMgr::CRegistInfoSum::InitAlloc(void) {

	const UShort allocNum = pow(2, REGIST_ALLOC_BASE_POWER);

	CMemory::AllocDouble(&m_polygon2DRegistInfos, allocNum);
	CMemory::AllocDouble(&m_polygon3DRegistInfos, allocNum);
	CMemory::AllocDouble(&m_text2DRegistInfos   , allocNum);
	CMemory::AllocDouble(&m_text3DRegistInfos   , allocNum);
	CMemory::AllocDouble(&m_modelRegistInfos    , allocNum);

	m_polygon2DRegistInfoAllocNum   = 
	m_polygon3DRegistInfoAllocNum   = 
	m_text2DRegistInfoAllocNum      = 
	m_text3DRegistInfoAllocNum      = 
	m_modelRegistInfoAllocNum       = allocNum;

	m_polygon2DRegistInfoAllocPower = 
	m_polygon3DRegistInfoAllocPower = 
	m_text2DRegistInfoAllocPower    = 
	m_text3DRegistInfoAllocPower    = 
	m_modelRegistInfoAllocPower     = REGIST_ALLOC_BASE_POWER;
}

//========================================
// 再確保処理
//========================================
void CDrawMgr::CRegistInfoSum::ReAlloc(void) {

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	if (m_polygon2DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_polygon2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon2DDrawNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAllocDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum, allocLine);

				// 確保した数/べき乗数を保存
				m_polygon2DRegistInfoAllocNum   = allocLine;
				m_polygon2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// ポリゴン3D
	//----------------------------------------
	if (m_polygon3DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_polygon3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon3DDrawNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAllocDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum, allocLine);

				// 確保した数/べき乗数を保存
				m_polygon3DRegistInfoAllocNum   = allocLine;
				m_polygon3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// テキスト2D
	//----------------------------------------
	if (m_text2DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_text2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text2DDrawNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAllocDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum, allocLine);

				// 確保した数/べき乗数を保存
				m_text2DRegistInfoAllocNum   = allocLine;
				m_text2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// テキスト3D
	//----------------------------------------
	if (m_text3DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_text3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text3DDrawNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAllocDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum, allocLine);

				// 確保した数/べき乗数を保存
				m_text3DRegistInfoAllocNum   = allocLine;
				m_text3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// モデル
	//----------------------------------------
	if (m_modelRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_modelRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_modelDrawNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAllocDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum, allocLine);

				// 確保した数/べき乗数を保存
				m_modelRegistInfoAllocNum   = allocLine;
				m_modelRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}
}

//========================================
// 解放処理
//========================================
void CDrawMgr::CRegistInfoSum::Release() {

	CMemory::ReleaseDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum);
	CMemory::ReleaseDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum);
	CMemory::ReleaseDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum);
	CMemory::ReleaseDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum);
	CMemory::ReleaseDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum);
}