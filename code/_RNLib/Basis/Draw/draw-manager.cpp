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
CDrawMgr::CRegistInfoSum* CDrawMgr::ms_resistInfoSum		= NULL;
CDrawMgr::CRegistInfoSum* CDrawMgr::ms_resistInfoSumScreen	= NULL;
CDrawMgr::CDrawInfoSum*   CDrawMgr::ms_drawInfoSum			= NULL;
CDrawMgr::CDrawInfoSum*   CDrawMgr::ms_drawInfoSumOvr		= NULL;
CDrawMgr::CDrawInfoSum*   CDrawMgr::ms_drawInfoSumScreen	= NULL;
CDrawMgr::CDrawInfoSum*   CDrawMgr::ms_drawInfoSumScreenOvr = NULL;
UShort                    CDrawMgr::ms_priorityMax;

//========================================
// 描画開始処理
//========================================
void CDrawMgr::StartDraw(Device& device) {

	//----------------------------------------
	// 登録情報を適用
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {

		// 登録情報を元に設置する
		PutBasedRegistInfo(ms_resistInfoSum[cntPriority], cntPriority, false);
		PutBasedRegistInfo(ms_resistInfoSumScreen[cntPriority], cntPriority, true);

		// 登録情報を描画情報に変換する
		ConvRegistInfoToDrawInfo(ms_resistInfoSum[cntPriority], ms_drawInfoSumOvr[cntPriority], device);
		ConvRegistInfoToDrawInfo(ms_resistInfoSumScreen[cntPriority], ms_drawInfoSumScreenOvr[cntPriority], device);
	}

	//----------------------------------------
	// 描画情報を入れ替える
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {

		// 描画情報を上書きする
		ms_drawInfoSum[cntPriority].Overwrite(&ms_drawInfoSumOvr[cntPriority]);
		ms_drawInfoSumScreen[cntPriority].Overwrite(&ms_drawInfoSumScreenOvr[cntPriority]);

		// 不要メモリ破棄の為、再確保
		ms_resistInfoSum[cntPriority].ReAlloc();
		ms_resistInfoSumScreen[cntPriority].ReAlloc();
	}

	// ポリゴン2Dの頂点バッファ再確保
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

	// ポリゴン3Dの頂点バッファ再確保
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

	//----------------------------------------
	// 描画
	//----------------------------------------
	// 描画を開始
	device->BeginScene();

	// [[[ スクリーン描画 ]]]
	CCamera::StartRenderingScreen(device);
	Draw(device, NONEDATA, false, true);
}

//========================================
// 描画終了処理
//========================================
void CDrawMgr::EndDraw(Device& device) {

	{// [[[ カメラ描画 ]]]
		// レンダリングターゲット/Zバッファ/ビューポートを保存
		Surface  renderDef;
		Surface  ZBuffDef;
		Viewport viewPortDef;
		device->GetRenderTarget(0, &renderDef);
		device->GetDepthStencilSurface(&ZBuffDef);
		device->GetViewport(&viewPortDef);

		// カメラ1つ1つに対し描画していく
		CCamera* camera = NULL;
		CCameraMgr& cameraMgr = RNSystem::GetCameraMgr();
		while (cameraMgr.ListLoop((CObject**)&camera)) {
			if (!camera->GetDraw())
				continue;
			camera->StartRendering(device);
			Draw(device, camera->GetID(), camera->GetClipping(), false);
			camera->EndRendering(device);
			camera->SetDraw(false);
		}

		// レンダリングターゲット/Zバッファ/ビューポートを元に戻す
		device->SetRenderTarget(0, renderDef);
		device->SetDepthStencilSurface(ZBuffDef);
		device->SetViewport(&viewPortDef);
	}

	// 描画終了
	device->EndScene();

	// バックバッファとフロントバッファの入れ替え
	device->Present(NULL, NULL, NULL, NULL);
}

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
		CMemory::Release(&ms_drawInfoSum         [cntPriority].m_model);
		CMemory::Release(&ms_drawInfoSum         [cntPriority].m_polygon3D);
		CMemory::Release(&ms_drawInfoSum         [cntPriority].m_polygon2D);
		CMemory::Release(&ms_drawInfoSumOvr      [cntPriority].m_model);
		CMemory::Release(&ms_drawInfoSumOvr      [cntPriority].m_polygon3D);
		CMemory::Release(&ms_drawInfoSumOvr      [cntPriority].m_polygon2D);
		CMemory::Release(&ms_drawInfoSumScreen   [cntPriority].m_model);
		CMemory::Release(&ms_drawInfoSumScreen   [cntPriority].m_polygon3D);
		CMemory::Release(&ms_drawInfoSumScreen   [cntPriority].m_polygon2D);
		CMemory::Release(&ms_drawInfoSumScreenOvr[cntPriority].m_model);
		CMemory::Release(&ms_drawInfoSumScreenOvr[cntPriority].m_polygon3D);
		CMemory::Release(&ms_drawInfoSumScreenOvr[cntPriority].m_polygon2D);

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
		RegistPolygon2D(ms_resistInfoSumScreen[priority]) :
		RegistPolygon2D(ms_resistInfoSum[priority]);

	// 番号を代入
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
CText2D::CRegistInfo* CDrawMgr::PutText2D(const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen) {

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
// 描画処理
//========================================
void CDrawMgr::Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	// ビューマトリックスを取得
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// 描画していく
	if (isOnScreen) {
		ExecutionDraw(device, cameraID, isCameraClipping, ms_drawInfoSumScreen, viewMtx);
	}
	else {
		ExecutionDraw(device, cameraID, isCameraClipping, ms_drawInfoSum, viewMtx);
	}
}

//========================================
// 描画実行処理
//========================================
void CDrawMgr::ExecutionDraw(Device& device, const short& cameraID, const bool& isCameraClipping, CDrawInfoSum*& drawInfo, Matrix& viewMtx) {

	for (int cntPriority = 0; cntPriority < ms_priorityMax; cntPriority++) {
		//----------------------------------------
		// モデル描画
		//----------------------------------------
		for (int cntModel = 0; cntModel < drawInfo[cntPriority].m_modelNum; cntModel++) {

			if (drawInfo[cntPriority].m_model[cntModel] == NULL)
				continue;

			// クリッピングIDが対象外であれば折り返す
			if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != NONEDATA || isCameraClipping)
				if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != cameraID)
					continue;

			// 本体のワールドマトリックスの設定
			device->SetTransform(D3DTS_WORLD, &drawInfo[cntPriority].m_model[cntModel]->m_mtx);

			// [[[ Zテストの設定 ]]]
			if (drawInfo[cntPriority].m_model[cntModel]->m_isZTest) {
				device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else {
				device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			// 描画
			for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++) {

				// マテリアルの設定
				device->SetMaterial(&drawInfo[cntPriority].m_model[cntModel]->m_mats[cntMat]);

				// [[[ テクスチャの設定 ]]]
				device->SetTexture(0, drawInfo[cntPriority].m_model[cntModel]->m_texes[cntMat]);

				// [[[ 描画 ]]]
				drawInfo[cntPriority].m_model[cntModel]->m_mesh->DrawSubset(cntMat);
			}

			// 輪郭線の描画
			if (drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh != NULL) {

				// マテリアルの設定
				device->SetMaterial(&CModel::CDrawInfo::ms_outLineMat);

				// 裏面
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

				for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++) {
					drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh->DrawSubset(cntMat);
				}

				// 表面
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
		}

		//----------------------------------------
		// ポリゴン3D
		//----------------------------------------
		for (int cntPolygon3D = 0; cntPolygon3D < drawInfo[cntPriority].m_polygon3DNum; cntPolygon3D++) {

			if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D] == NULL)
				continue;

			// クリッピングIDが対象外であれば折り返す
			if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != NONEDATA || isCameraClipping)
				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != cameraID)
					continue;

			drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->Draw(device, viewMtx);
		}

		//----------------------------------------
		// ポリゴン2D
		//----------------------------------------
		for (int cntPolygon2D = 0; cntPolygon2D < drawInfo[cntPriority].m_polygon2DNum; cntPolygon2D++) {

			if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D] == NULL)
				continue;

			// クリッピングIDが対象外であれば折り返す
			if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != NONEDATA || isCameraClipping)
				if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != cameraID)
					continue;

			drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->Draw(device, viewMtx);
		}
	}
}

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

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.m_polygon2DNum; cntDrawInfo++) {

		// 頂点情報を代入
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon2D[cntDrawInfo]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < CPolygon2D::CDrawInfo::m_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon2D[cntDrawInfo]->m_vtxs[cntVtx];
		}
	}
}

//========================================
// 描画情報を頂点3D情報に変換
//========================================
void CDrawMgr::ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntPolygon3D = 0; cntPolygon3D < drawInfoSum.m_polygon3DNum; cntPolygon3D++) {

		// 頂点情報を代入
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon3D[cntPolygon3D]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < CPolygon3D::CDrawInfo::m_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon3D[cntPolygon3D]->m_vtxs[cntVtx];
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
void CDrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device) {

	{// 描画情報のメモリ確保
		drawInfoSum.m_modelNum     = resistInfoSum.m_modelRegistInfoNum;
		drawInfoSum.m_polygon3DNum = resistInfoSum.m_polygon3DRegistInfoNum;
		drawInfoSum.m_polygon2DNum = resistInfoSum.m_polygon2DRegistInfoNum;
		CMemory::Alloc(&drawInfoSum.m_model    , drawInfoSum.m_modelNum    , true);
		CMemory::Alloc(&drawInfoSum.m_polygon3D, drawInfoSum.m_polygon3DNum, true);
		CMemory::Alloc(&drawInfoSum.m_polygon2D, drawInfoSum.m_polygon2DNum, true);
	}

	//----------------------------------------
	// モデル
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++) {
		drawInfoSum.m_model[cnt] = resistInfoSum.m_modelRegistInfos[cnt]->ConvToDrawInfo(device);
	}

	resistInfoSum.m_modelDrawNum = resistInfoSum.m_modelRegistInfoNum;
	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon3D[cnt] = resistInfoSum.m_polygon3DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon3DDrawNum = resistInfoSum.m_polygon3DRegistInfoNum;
	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon2D[cnt] = resistInfoSum.m_polygon2DRegistInfos[cnt]->ConvToDrawInfo();
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

	m_model        = NULL;
	m_modelNum     = 0;
	m_polygon2D    = NULL;
	m_polygon2DNum = 0;
	m_polygon3D    = NULL;
	m_polygon3DNum = 0;
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
	if (m_model != NULL) {
		for (int cnt = 0; cnt < m_modelNum; cnt++) {
			CMemory::Release(&m_model[cnt]);
		}
		CMemory::Release(&m_model);
	}
	
	if (m_polygon3D != NULL) {
		for (int cnt = 0; cnt < m_polygon3DNum; cnt++) {
			CMemory::Release(&m_polygon3D[cnt]);
		}
		CMemory::Release(&m_polygon3D);
	}

	if (m_polygon2D != NULL) {
		for (int cnt = 0; cnt < m_polygon2DNum; cnt++) {
			CMemory::Release(&m_polygon2D[cnt]);
		}
		CMemory::Release(&m_polygon2D);
	}
}

//========================================
// 上書き処理
//========================================
void CDrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

	// 解放処理
	Release();

	// 上書き
	m_model        = pOvr->m_model;
	m_modelNum     = pOvr->m_modelNum;
	m_polygon3D    = pOvr->m_polygon3D;
	m_polygon3DNum = pOvr->m_polygon3DNum;
	m_polygon2D    = pOvr->m_polygon2D;
	m_polygon2DNum = pOvr->m_polygon2DNum;
	pOvr->m_model		 = NULL;
	pOvr->m_modelNum	 = 0;
	pOvr->m_polygon3D	 = NULL;
	pOvr->m_polygon3DNum = 0;
	pOvr->m_polygon2D	 = NULL;
	pOvr->m_polygon2DNum = 0;
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