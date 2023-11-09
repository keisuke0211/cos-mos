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
CDrawMgr::PROCESS_STATE          CDrawMgr::ms_processState = PROCESS_STATE::REGIST_ACCEPT;
CDrawMgr::CRegistInfoSum         CDrawMgr::ms_resistInfoSum;
CDrawMgr::CRegistInfoSum         CDrawMgr::ms_resistInfoSumScreen;
CDrawMgr::CDrawInfoSum           CDrawMgr::ms_drawInfoSum;
CDrawMgr::CDrawInfoSum           CDrawMgr::ms_drawInfoSumOvr;
CDrawMgr::CDrawInfoSum           CDrawMgr::ms_drawInfoSumScreen;
CDrawMgr::CDrawInfoSum           CDrawMgr::ms_drawInfoSumScreenOvr;
CDrawMgr::CameraDrawOrderInfoSum CDrawMgr::ms_cameraDrawOrderInfoSum = {};
CDrawMgr::CameraDrawOrderInfoSum CDrawMgr::ms_cameraDrawOrderInfoSumOvr = {};
std::thread                      CDrawMgr::ms_mainLoopTh(MainLoop);

//========================================
// コンストラクタ
//========================================
CDrawMgr::CDrawMgr() {

	m_reAllocCount = 0;
}

//========================================
// デストラクタ
//========================================
CDrawMgr::~CDrawMgr() {

}

//========================================
// 初期化処理
//========================================
void CDrawMgr::Init(void) {

	// 登録情報の初期メモリ確保
	ms_resistInfoSum.InitAlloc();
	ms_resistInfoSumScreen.InitAlloc();

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

	// 描画情報を解放
	ms_drawInfoSum.Release();
	ms_drawInfoSumOvr.Release();
	ms_drawInfoSumScreen.Release();
	ms_drawInfoSumScreenOvr.Release();
	CMemory::Release(&ms_drawInfoSum.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumOvr.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumScreen.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumScreenOvr.ms_drawInfos);

	// カメラの描画順情報を解放
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++)
		CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList);
	CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos);
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum; cnt++)
		CMemory::Release(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt].orderList);
	CMemory::Release(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos);

	// 登録情報を解放
	ms_resistInfoSum.Release();
	ms_resistInfoSumScreen.Release();

	// 頂点バッファを解放する
	CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
	CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
}

//========================================
// 描画開始処理
//========================================
bool CDrawMgr::StartDraw(void) {

	if (ms_processState == PROCESS_STATE::REGIST_ACCEPT)
	{// [ 登録受付 ]の時、
		{// <<< カメラの描画順情報を作成 >>>
			// カメラの数を取得し、その分描画順情報を確保
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum = RNLib::CameraMgr().GetNum() + 1;	// ※+1はスクリーンの分
			CMemory::Alloc(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum);

			// 情報を設定していく
			CCamera* camera = NULL;
			int cameraCount = 0;
			while (RNLib::CameraMgr().ListLoop((CObject**)&camera)) {
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].ID        = camera->GetID();
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].orderList = NULL;
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].posV      = camera->GetPosV();
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].nor       = camera->GetNor();
				cameraCount++;
			}
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].ID        = NONEDATA;	// ※スクリーンとして扱う
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].orderList = NULL;
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].posV      = INITPOS3D;
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].nor       = Vector3D(0.0f, 0.0f, 1.0f);
		}

		// [ 登録情報適用待ち ]にする
		ms_processState = PROCESS_STATE::REGIST_INFO_APPLY_WAIT;
	}
	else if ((ms_processState == PROCESS_STATE::DRAW_INFO_SWAP_WAIT && !RNSystem::GetSpace3DStop()) || RNSystem::GetSceneSwap())
	{// [ 描画入れ替え待ち ]の時、
		// 描画情報を上書きする
		ms_drawInfoSum.Overwrite(&ms_drawInfoSumOvr);
		ms_drawInfoSumScreen.Overwrite(&ms_drawInfoSumScreenOvr);

		{// カメラの描画順情報を上書きする
			// 古い情報を解放
			for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++)
				CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList);
			CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos);

			// 新しい情報で上書き
			ms_cameraDrawOrderInfoSum = ms_cameraDrawOrderInfoSumOvr;
			ms_cameraDrawOrderInfoSumOvr = {};
		}

		{// 不要メモリ破棄の為、再確保
			ms_resistInfoSum.ReAlloc();
			ms_resistInfoSumScreen.ReAlloc();

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
						CPolygon2D::CDrawInfo::m_allocNum   = allocLine;
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
						CPolygon3D::CDrawInfo::m_allocNum   = allocLine;
						CPolygon3D::CDrawInfo::m_allocPower = cntAlloc;

						break;
					}
				}
			}
		}

		// 番号カウント初期化
		CPolygon2D::CDrawInfo::m_idxCount = 0;
		CPolygon3D::CDrawInfo::m_idxCount = 0;

		// 頂点情報を代入
		AssignVertexInfo();

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

	// カメラのIDに対応する描画順リストを取得
	UShort* orderList = NULL;
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++) {
		if (cameraID == ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].ID) {
			orderList = ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList;
			break;
		}
	}
	// ※取得できなければ終了
	if (orderList == NULL)
		return;

	// ビューマトリックスを取得
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// 描画していく
	if (isOnScreen) {
		for (int cntDrawInfo = 0; cntDrawInfo < ms_drawInfoSumScreen.ms_drawInfoNum; cntDrawInfo++) {
			if (ms_drawInfoSumScreen.ms_drawInfos[orderList[cntDrawInfo]] != NULL) {

				// クリッピングIDが対象外であれば折り返す
				if (ms_drawInfoSumScreen.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != NONEDATA || isCameraClipping)
					if (ms_drawInfoSumScreen.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != cameraID)
						continue;

				ms_drawInfoSumScreen.ms_drawInfos[orderList[cntDrawInfo]]->Draw(device, viewMtx);
			}
		}
	}
	else {
		for (int cntDrawInfo = 0; cntDrawInfo < ms_drawInfoSum.ms_drawInfoNum; cntDrawInfo++) {
			if (ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]] != NULL) {
				
				// クリッピングIDが対象外であれば折り返す
				if (ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != NONEDATA || isCameraClipping)
					if (ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != cameraID)
						continue;

				ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->Draw(device, viewMtx);
			}
		}
	}
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::PutPolygon2D(const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen) {

	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (CPolygon2D::CDrawInfo::m_idxCount == CPolygon2D::CDrawInfo::m_allocNum) {
		CPolygon2D::CDrawInfo::m_allocPower++;
		CPolygon2D::CDrawInfo::m_allocNum = pow(2, CPolygon2D::CDrawInfo::m_allocPower);
		CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon2D::CDrawInfo::CreateVertexBuffer(CPolygon2D::CDrawInfo::m_allocNum);
	}

	// 登録情報
	CPolygon2D::CRegistInfo* registInfo = NULL;

	// スクリーン上フラグに応じて登録
	if (isOnScreen) registInfo = RegistPolygon2D(ms_resistInfoSumScreen);
	else            registInfo = RegistPolygon2D(ms_resistInfoSum);

	// 情報を代入
	if (registInfo != NULL) {
		registInfo->SetIdx(CPolygon2D::CDrawInfo::m_idxCount++);
		registInfo->SetPos(pos);
		registInfo->SetAngle(angle);
	}

	return registInfo;
}

//========================================
// 設置処理(ポリゴン3D)
//========================================
CPolygon3D::CRegistInfo* CDrawMgr::PutPolygon3D(const Matrix& mtx, const bool& isOnScreen) {
	
	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (CPolygon3D::CDrawInfo::m_idxCount == CPolygon3D::CDrawInfo::m_allocNum) {
		CPolygon3D::CDrawInfo::m_allocPower++;
		CPolygon3D::CDrawInfo::m_allocNum = pow(2, CPolygon3D::CDrawInfo::m_allocPower);
		CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon3D::CDrawInfo::CreateVertexBuffer(CPolygon3D::CDrawInfo::m_allocNum);
	}

	// 登録情報
	CPolygon3D::CRegistInfo* registInfo = NULL;

	// スクリーン上フラグに応じて登録
	if (isOnScreen) registInfo = RegistPolygon3D(ms_resistInfoSumScreen);
	else            registInfo = RegistPolygon3D(ms_resistInfoSum);

	// 情報を代入
	if (registInfo != NULL) {
		registInfo->SetIdx(CPolygon3D::CDrawInfo::m_idxCount++);
		registInfo->SetMtx(mtx);
	}

	return registInfo;
}

//========================================
// 設置処理(テキスト2D)
//========================================
CText2D::CRegistInfo* CDrawMgr::PutText2D(const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen) {

	// 登録情報
	CText2D::CRegistInfo* registInfo = NULL;

	// スクリーン上フラグに応じて登録
	if (isOnScreen) registInfo = RegistText2D(ms_resistInfoSumScreen);
	else            registInfo = RegistText2D(ms_resistInfoSum);

	// 情報を代入
	if (registInfo != NULL) {
		registInfo->SetPos(pos);
		registInfo->SetAngle(angle);
	}

	return registInfo;
}

//========================================
// 設置処理(テキスト3D)
//========================================
CText3D::CRegistInfo* CDrawMgr::PutText3D(const Matrix& mtx, const bool& isOnScreen) {

	// 登録情報
	CText3D::CRegistInfo* registInfo = NULL;

	// スクリーン上フラグに応じて登録
	if (isOnScreen) registInfo = RegistText3D(ms_resistInfoSumScreen);
	else            registInfo = RegistText3D(ms_resistInfoSum);

	// 情報を代入
	if (registInfo != NULL) {
		registInfo->SetMtx(mtx);
	}

	return registInfo;
}

//========================================
// 設置処理(モデル)
//========================================
CModel::CRegistInfo* CDrawMgr::PutModel(const Matrix& mtx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (ms_processState != PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	// 登録情報
	CModel::CRegistInfo* registInfo = NULL;

	// スクリーン上フラグに応じて登録
	if (isOnScreen) registInfo = RegistModel(ms_resistInfoSumScreen);
	else            registInfo = RegistModel(ms_resistInfoSum);

	// 情報を代入
	if (registInfo != NULL) {
		registInfo->SetMtx(mtx);
	}

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

		ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSum);
		ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSumScreen);

		// 頂点バッファをアンロック
		CPolygon2D::CDrawInfo::m_vtxBuff->Unlock();
	}

	// 頂点3D情報に変換
	if (CPolygon3D::CDrawInfo::m_vtxBuff != NULL) {

		// 頂点バッファをロック
		Vertex3D* vtxs = NULL;
		CPolygon3D::CDrawInfo::m_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSum);
		ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSumScreen);

		// 頂点バッファをアンロック
		CPolygon3D::CDrawInfo::m_vtxBuff->Unlock();
	}
}


//========================================
// 描画情報を頂点2D情報に変換
//========================================
void CDrawMgr::ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.ms_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.ms_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON2D)
		{// カウントの描画情報がポリゴン2Dの時、
			// 頂点情報を代入
			CPolygon2D::CDrawInfo& drawInfo = (CPolygon2D::CDrawInfo&)*drawInfoSum.ms_drawInfos[cntDrawInfo];
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

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.ms_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.ms_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON3D)
		{// カウントの描画情報がポリゴン2Dの時、
			// 頂点情報を代入
			CPolygon3D::CDrawInfo& drawInfo((CPolygon3D::CDrawInfo&)*drawInfoSum.ms_drawInfos[cntDrawInfo]);
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
		CMemory::ReAlloc(&resistInfo.m_polygon2DRegistInfos, oldAllocNum, resistInfo.m_polygon2DRegistInfoAllocNum);
	}

	resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum].ClearParameter();
	return &resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum++];
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
		CMemory::ReAlloc(&resistInfo.m_polygon3DRegistInfos, oldAllocNum, resistInfo.m_polygon3DRegistInfoAllocNum);
	}

	resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum].ClearParameter();
	return &resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum++];
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
		CMemory::ReAlloc(&resistInfo.m_text2DRegistInfos, oldAllocNum, resistInfo.m_text2DRegistInfoAllocNum);
	}

	resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum].ClearParameter();
	return &resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum++];
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
		CMemory::ReAlloc(&resistInfo.m_text3DRegistInfos, oldAllocNum, resistInfo.m_text3DRegistInfoAllocNum);
	}

	resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum].ClearParameter();
	return &resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum++];
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
		CMemory::ReAlloc(&resistInfo.m_modelRegistInfos, oldAllocNum, resistInfo.m_modelRegistInfoAllocNum);
	}

	resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum].ClearParameter();
	return &resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum++];
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
			// 登録情報を元に設置する
			PutBasedRegistInfo(ms_resistInfoSum, false);
			PutBasedRegistInfo(ms_resistInfoSumScreen, true);

			// 登録情報を描画情報に変換する
			ConvRegistInfoToDrawInfo(ms_resistInfoSum, ms_drawInfoSumOvr);
			ConvRegistInfoToDrawInfo(ms_resistInfoSumScreen, ms_drawInfoSumScreenOvr);

			{// 描画情報をソートする
				// カメラ
				int cnt = 0;
				for (; cnt < ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum - 1; cnt++)
					SortDrawInfo(ms_drawInfoSumOvr, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt]);

				// スクリーン
				SortDrawInfo(ms_drawInfoSumScreenOvr, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt]);
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
void CDrawMgr::PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const bool& isOnScreen) {

	//----------------------------------------
	// テキスト3Dからポリゴン3Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text3DRegistInfoNum; cnt++) {
		resistInfoSum.m_text3DRegistInfos[cnt].PutPolygon3D(isOnScreen);
	}

	// 解放
	resistInfoSum.m_text3DRegistInfoNum = 0;

	//----------------------------------------
	// テキスト2Dからポリゴン2Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text2DRegistInfoNum; cnt++) {
		resistInfoSum.m_text2DRegistInfos[cnt].PutPolygon2D(isOnScreen);
	}

	// 解放
	resistInfoSum.m_text2DRegistInfoNum = 0;
}

//========================================
// [静的]登録情報を描画情報に変換する
//========================================
void CDrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum) {

	int cntDrawInfo = 0;

	{// 描画情報のメモリ確保
		drawInfoSum.ms_drawInfoNum =
			resistInfoSum.m_polygon2DRegistInfoNum +	// ポリゴン2D
			resistInfoSum.m_polygon3DRegistInfoNum +	// ポリゴン3D
			resistInfoSum.m_modelRegistInfoNum;			// モデル
		CMemory::Alloc<CDrawInfoBase*>(&drawInfoSum.ms_drawInfos, drawInfoSum.ms_drawInfoNum, true);
	}

	//----------------------------------------
	// ポリゴン3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon3DRegistInfos[cnt].ConvToDrawInfo();
	}

	// 解放
	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// モデル
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_modelRegistInfos[cnt].ConvToDrawInfo();
	}

	// 解放
	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon2DRegistInfos[cnt].ConvToDrawInfo();
	}

	// 解放
	resistInfoSum.m_polygon2DRegistInfoNum = 0;
}

//========================================
// [静的]描画情報ソート処理
//========================================
void CDrawMgr::SortDrawInfo(CDrawInfoSum& drawInfoSum, CameraDrawOrderInfo& cameraDrawOrderInfo) {

	// 描画情報の数が0以下なら終了
	if (drawInfoSum.ms_drawInfoNum <= 0)
		return;

	// 描画順リストを描画情報数分メモリ確保
	CMemory::Alloc(&cameraDrawOrderInfo.orderList, drawInfoSum.ms_drawInfoNum, true);

	for (int cnt = 0; cnt < drawInfoSum.ms_drawInfoNum; cnt++) {

		// 描画順リストの値を初期化
		cameraDrawOrderInfo.orderList[cnt] = cnt;

		// ソート用にカメラまでの距離を調べる
		if (drawInfoSum.ms_drawInfos[cnt]->m_type == CDrawInfoBase::TYPE::MODEL) {
			CModel::CDrawInfo* castedInfo = (CModel::CDrawInfo*)drawInfoSum.ms_drawInfos[cnt];
			castedInfo->m_distance = CGeometry::FindDistanceToPlane(CMatrix::ConvMtxToPos(castedInfo->m_mtx), cameraDrawOrderInfo.posV, cameraDrawOrderInfo.nor);
		}
		else if (drawInfoSum.ms_drawInfos[cnt]->m_type == CDrawInfoBase::TYPE::POLYGON3D) {
			CPolygon3D::CDrawInfo* castedInfo = (CPolygon3D::CDrawInfo*)drawInfoSum.ms_drawInfos[cnt];
			castedInfo->m_distance = CGeometry::FindDistanceToPlane(CMatrix::ConvMtxToPos(castedInfo->m_mtx), cameraDrawOrderInfo.posV, cameraDrawOrderInfo.nor);
		}
	}

	//----------------------------------------
	// ローカル関数の定義
	//----------------------------------------
	struct LocalFunc {
		// [[[ 優先するか調べる(ポリゴン3Dとモデル) ]]]
		static bool FindPrioritizePolygon3DAndModel(CPolygon3D::CDrawInfo* base, CModel::CDrawInfo* target) {

			if (base->m_isZTest && !target->m_isZTest) return true;		// 基準のZテストがONで比較対象ののZテストがOFF
			if (!base->m_isZTest && target->m_isZTest) return false;	// 基準のZテストがOFFで比較対象のZテストがON
			if (base->m_distance > target->m_distance) return true;		// 基準の方が距離が遠い

			return false;
		}

		// [[[ 優先するか調べる ]]]
		// ※返すのは描画を優先するかなので注意
		static bool FindPrioritize(CDrawInfoBase& base, CDrawInfoBase& target) {

			// 片方が存在しない
			if (&base == NULL || &target == NULL)
				return false;
			
			// 基準の方が優先度が低い
			if (base.m_priority < target.m_priority) 
				return true;

			// 基準の方が優先度が高い
			if (base.m_priority > target.m_priority)
				return false;

			if (base.m_type == CDrawInfoBase::TYPE::POLYGON2D) {
				//----------------------------------------
				// [[[ Base:ポリゴン2D ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D)
				{// [[[ Target::ポリゴン2D ]]]
					CPolygon2D::CDrawInfo* castedBase   = (CPolygon2D::CDrawInfo*)&base;
					CPolygon2D::CDrawInfo* castedTarget = (CPolygon2D::CDrawInfo*)&target;

					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// 基準のZテストがONで比較対象ののZテストがOFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// 基準のZテストがOFFで比較対象のZテストがON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// 基準の方が距離が遠い
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::ポリゴン3D ]]]
					return false;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:モデル ]]]
					return false;
				}
				else assert(false);
			}
			else if (base.m_type == CDrawInfoBase::TYPE::POLYGON3D) {
				//----------------------------------------
				// [[[ Base:ポリゴン3D ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D) 
				{// [[[ Target::ポリゴン2D ]]]
					return true;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::ポリゴン3D ]]]
					CPolygon3D::CDrawInfo* castedBase   = (CPolygon3D::CDrawInfo*)&base;
					CPolygon3D::CDrawInfo* castedTarget = (CPolygon3D::CDrawInfo*)&target;

					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// 基準のZテストがONで比較対象ののZテストがOFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// 基準のZテストがOFFで比較対象のZテストがON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// 基準の方が距離が遠い
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:モデル ]]]
					return FindPrioritizePolygon3DAndModel((CPolygon3D::CDrawInfo*)&base, (CModel::CDrawInfo*)&target);
				}
				else assert(false);
			}
			else if (base.m_type == CDrawInfoBase::TYPE::MODEL) {
				//----------------------------------------
				// [[[ Base:モデル ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D)
				{// [[[ Target::ポリゴン2D ]]]
					return true;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::ポリゴン3D ]]]
					return !FindPrioritizePolygon3DAndModel((CPolygon3D::CDrawInfo*)&target, (CModel::CDrawInfo*)&base);
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:モデル ]]]
					CModel::CDrawInfo* castedBase = (CModel::CDrawInfo*)&base;
					CModel::CDrawInfo* castedTarget = (CModel::CDrawInfo*)&target;
					
					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// 基準のZテストがONで比較対象ののZテストがOFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// 基準のZテストがOFFで比較対象のZテストがON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// 基準の方が距離が遠い
				}
				else assert(false);
			}
			else assert(false);

			return false;
		}
	};

	//----------------------------------------
	// ソート処理
	//      cnt1
	// [ 0 ][ 1 ][ 2 ][ 3 ][ 4 ][ … ]
	// ※この場合2から比較していく
	//----------------------------------------
	int cnt1Num = drawInfoSum.ms_drawInfoNum - 1;
	for (int cnt1 = 0; cnt1 < cnt1Num; cnt1++) {

		// 最優先のカウントを調べる
		int cntTop = NONEDATA;
		for (int cnt2 = cnt1 + 1; cnt2 < drawInfoSum.ms_drawInfoNum; cnt2++) {
			CDrawInfoBase& drawInfoCnt2 = *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cnt2]];

			//----------------------------------------
			// Cnt2の描画情報 が 今の最優先描画情報 より
			// 優先度が高いかどうか判定する
			//----------------------------------------
			bool isTopPriorityCnt2 = false;

			// 最優先のカウントがNONE(つまり最初のカウント)の時、Cnt2の方が優先度が高いとする
			if (cntTop == NONEDATA) {
				isTopPriorityCnt2 = true;
			}
			else {
				isTopPriorityCnt2 = LocalFunc::FindPrioritize(drawInfoCnt2, *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cntTop]]);
			}
			
			// 判定の結果…
			// Cnt2の描画情報 の方が優先度が高かった時、
			// 最優先のカウントを上書きする
			if (isTopPriorityCnt2)
				cntTop = cnt2;
		}

		// Cnt1描画情報 の方が 最優先描画情報 より優先度が低い時、入れ替える
		if (!LocalFunc::FindPrioritize(*drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cnt1]], *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cntTop]])) {
			UShort temp = cameraDrawOrderInfo.orderList[cnt1];
			cameraDrawOrderInfo.orderList[cnt1] = cameraDrawOrderInfo.orderList[cntTop];
			cameraDrawOrderInfo.orderList[cntTop] = temp;
		}
	}
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

	ms_drawInfos   = NULL;
	ms_drawInfoNum = 0;
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
	if (ms_drawInfos != NULL) {
		for (int cnt = 0; cnt < ms_drawInfoNum; cnt++) {
			CMemory::Release(&ms_drawInfos[cnt]);
		}
		CMemory::Release(&ms_drawInfos);
	}
}

//========================================
// 上書き処理
//========================================
void CDrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

	// 解放処理
	Release();

	// 上書き
	ms_drawInfos         = pOvr->ms_drawInfos;
	ms_drawInfoNum       = pOvr->ms_drawInfoNum;
	pOvr->ms_drawInfos   = NULL;
	pOvr->ms_drawInfoNum = 0;
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
	m_polygon3DRegistInfos          = NULL;
	m_polygon3DRegistInfoNum        = 0;
	m_polygon3DRegistInfoAllocPower = 0;
	m_polygon3DRegistInfoAllocNum   = 0;
	m_text2DRegistInfos             = NULL;
	m_text2DRegistInfoNum           = 0;
	m_text2DRegistInfoAllocPower    = 0;
	m_text2DRegistInfoAllocNum      = 0;
	m_text3DRegistInfos             = NULL;
	m_text3DRegistInfoNum           = 0;
	m_text3DRegistInfoAllocPower    = 0;
	m_text3DRegistInfoAllocNum      = 0;
	m_modelRegistInfos              = NULL;
	m_modelRegistInfoNum            = 0;
	m_modelRegistInfoAllocPower     = 0;
	m_modelRegistInfoAllocNum       = 0;
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

	CMemory::Alloc(&m_polygon2DRegistInfos, allocNum);
	CMemory::Alloc(&m_polygon3DRegistInfos, allocNum);
	CMemory::Alloc(&m_text2DRegistInfos   , allocNum);
	CMemory::Alloc(&m_text3DRegistInfos   , allocNum);
	CMemory::Alloc(&m_modelRegistInfos    , allocNum);
	
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
void CDrawMgr::CRegistInfoSum::ReAlloc() {

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	if (m_polygon2DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_polygon2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon2DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAlloc(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum, allocLine);

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

			if (m_polygon3DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAlloc(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum, allocLine);

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

			if (m_text2DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAlloc(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum, allocLine);

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

			if (m_text3DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAlloc(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum, allocLine);

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

			if (m_modelRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				CMemory::ReAlloc(&m_modelRegistInfos, m_modelRegistInfoAllocNum, allocLine);

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

	CMemory::Release(&m_polygon2DRegistInfos);
	CMemory::Release(&m_polygon3DRegistInfos);
	CMemory::Release(&m_text2DRegistInfos);
	CMemory::Release(&m_text3DRegistInfos);
	CMemory::Release(&m_modelRegistInfos);
}