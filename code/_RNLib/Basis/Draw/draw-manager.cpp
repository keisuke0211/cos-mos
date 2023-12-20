//========================================
// 
// 描画処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画マネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_DrawMgr::_RNC_DrawMgr() {

	m_reAllocCount     = 0;
	for(int cnt = 0;cnt < 2;cnt++){
		m_isDrawPolygon2D [cnt] = true;
		m_isDrawPolygon3D [cnt] = true;
		m_isDrawModel     [cnt] = true;
		m_isDrawStaticMesh[cnt] = true;
	}
}

//========================================
// デストラクタ
//========================================
_RNC_DrawMgr::~_RNC_DrawMgr() {

}

//========================================
// 初期化処理
//========================================
void _RNC_DrawMgr::Init(const UShort& priorityMax) {

	// 最大優先度を保存
	if (priorityMax <= 0) {
		assert(false);
		m_priorityMax = 1;
	}
	else {
		m_priorityMax = priorityMax;
	}

	// 登録/描画情報のメモリ確保
	RNLib::Memory().Alloc(&m_drawInfoSum		 , m_priorityMax);
	RNLib::Memory().Alloc(&m_drawInfoSumOvr	     , m_priorityMax);
	RNLib::Memory().Alloc(&m_drawInfoSumScreen   , SCREEN_PRIORITY_MAX);
	RNLib::Memory().Alloc(&m_drawInfoSumScreenOvr, SCREEN_PRIORITY_MAX);
	RNLib::Memory().Alloc(&m_resistInfoSum	     , m_priorityMax);
	RNLib::Memory().Alloc(&m_resistInfoSumScreen , SCREEN_PRIORITY_MAX);

	// 登録情報の初期メモリ確保
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
		m_resistInfoSum[cntPriority].InitAlloc();

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
		m_resistInfoSumScreen[cntPriority].InitAlloc();

	// 頂点バッファの初期生成
	_RNC_Polygon2D::CDrawInfo::InitCreateVertexBuffer();
	_RNC_Polygon3D::CDrawInfo::InitCreateVertexBuffer();
}

//========================================
// 終了処理
//========================================
void _RNC_DrawMgr::Uninit(void) {

	// 解放処理
	Release();
}

//========================================
// 更新処理
//========================================
void _RNC_DrawMgr::Update(void) {

}

//========================================
// 描画開始処理
//========================================
void _RNC_DrawMgr::StartDraw(Device& device) {

	//----------------------------------------
	// 登録情報を適用
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// 登録情報を元に設置する
		PutBasedRegistInfo(m_resistInfoSum[cntPriority], cntPriority, false);

		// 登録情報を描画情報に変換する
		ConvRegistInfoToDrawInfo(m_resistInfoSum[cntPriority], m_drawInfoSumOvr[cntPriority], device);
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// 登録情報を元に設置する
		PutBasedRegistInfo(m_resistInfoSumScreen[cntPriority], cntPriority, true);

		// 登録情報を描画情報に変換する
		ConvRegistInfoToDrawInfo(m_resistInfoSumScreen[cntPriority], m_drawInfoSumScreenOvr[cntPriority], device);
	}

	//----------------------------------------
	// 描画情報を入れ替える
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// 描画情報を上書きする
		m_drawInfoSum[cntPriority].Overwrite(&m_drawInfoSumOvr[cntPriority]);

		// 不要メモリ破棄の為、再確保
		m_resistInfoSum[cntPriority].ReAlloc();
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// 描画情報を上書きする
		m_drawInfoSumScreen[cntPriority].Overwrite(&m_drawInfoSumScreenOvr[cntPriority]);

		// 不要メモリ破棄の為、再確保
		m_resistInfoSumScreen[cntPriority].ReAlloc();
	}

	// ポリゴン2Dの頂点バッファ再確保
	if (_RNC_Polygon2D::CDrawInfo::ms_allocPower > POLYGON2D_ALLOC_BASE_POWER) {

		for (int cntAlloc = POLYGON2D_ALLOC_BASE_POWER; cntAlloc < _RNC_Polygon2D::CDrawInfo::ms_allocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (_RNC_Polygon2D::CDrawInfo::ms_idxCount < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
				_RNC_Polygon2D::CDrawInfo::CreateVertexBuffer(allocLine);

				// 確保した数/べき乗数を保存
				_RNC_Polygon2D::CDrawInfo::ms_allocNum = allocLine;
				_RNC_Polygon2D::CDrawInfo::ms_allocPower = cntAlloc;

				break;
			}
		}
	}

	// ポリゴン3Dの頂点バッファ再確保
	if (_RNC_Polygon3D::CDrawInfo::ms_allocPower > POLYGON3D_ALLOC_BASE_POWER) {

		for (int cntAlloc = POLYGON3D_ALLOC_BASE_POWER; cntAlloc < _RNC_Polygon3D::CDrawInfo::ms_allocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (_RNC_Polygon3D::CDrawInfo::ms_idxCount < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
				_RNC_Polygon3D::CDrawInfo::CreateVertexBuffer(allocLine);

				// 確保した数/べき乗数を保存
				_RNC_Polygon3D::CDrawInfo::ms_allocNum = allocLine;
				_RNC_Polygon3D::CDrawInfo::ms_allocPower = cntAlloc;

				break;
			}
		}
	}

	// 番号カウント初期化
	_RNC_Polygon2D::CDrawInfo::ms_idxCount = 0;
	_RNC_Polygon3D::CDrawInfo::ms_idxCount = 0;

	// 頂点情報を代入
	AssignVertexInfo();

	//----------------------------------------
	// 描画
	//----------------------------------------
	// 描画を開始
	device->BeginScene();

	// [[[ スクリーン描画 ]]]
	CCamera::StartRenderingScreen(device);
	Draw(device, NULL, true);
}

//========================================
// 描画終了処理
//========================================
void _RNC_DrawMgr::EndDraw(Device& device) {

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
		_RNC_CameraMgr& cameraMgr = RNSystem::GetCameraMgr();
		while (cameraMgr.ListLoop((CObject**)&camera)) {
			if (!camera->GetIsDraw())
				continue;
			camera->StartRendering(device);
			Draw(device, camera, false);
			camera->EndRendering(device);
			camera->SetIsDraw(false);
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
// 解放処理
//========================================
void _RNC_DrawMgr::Release(void) {

	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// 描画情報を解放
		m_drawInfoSum   [cntPriority].Release();
		m_drawInfoSumOvr[cntPriority].Release();
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_polygon2D);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_polygon2D);

		// 登録情報を解放
		m_resistInfoSum      [cntPriority].Release();
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// 描画情報を解放
		m_drawInfoSumScreen   [cntPriority].Release();
		m_drawInfoSumScreenOvr[cntPriority].Release();
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_polygon2D);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_polygon2D);

		// 登録情報を解放
		m_resistInfoSumScreen[cntPriority].Release();
	}

	// 登録/描画情報のメモリ解放
	RNLib::Memory().Release(&m_drawInfoSum);
	RNLib::Memory().Release(&m_drawInfoSumOvr);
	RNLib::Memory().Release(&m_drawInfoSumScreen);
	RNLib::Memory().Release(&m_drawInfoSumScreenOvr);
	RNLib::Memory().Release(&m_resistInfoSum);
	RNLib::Memory().Release(&m_resistInfoSumScreen);

	// 頂点バッファを解放する
	_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
	_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
_RNC_Polygon2D::CRegistInfo* _RNC_DrawMgr::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// 優先度正常チェック
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (_RNC_Polygon2D::CDrawInfo::ms_idxCount == _RNC_Polygon2D::CDrawInfo::ms_allocNum) {
		_RNC_Polygon2D::CDrawInfo::ms_allocPower++;
		_RNC_Polygon2D::CDrawInfo::ms_allocNum = pow(2, _RNC_Polygon2D::CDrawInfo::ms_allocPower);
		_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
		_RNC_Polygon2D::CDrawInfo::CreateVertexBuffer(_RNC_Polygon2D::CDrawInfo::ms_allocNum);
	}

	// 登録情報
	_RNC_Polygon2D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon2D(m_resistInfoSumScreen[priority]) :
		RegistPolygon2D(m_resistInfoSum[priority]);

	// 番号を代入
	registInfo->SetIdx(_RNC_Polygon2D::CDrawInfo::ms_idxCount++);

	return registInfo;
}

//========================================
// 設置処理(ポリゴン3D)
//========================================
_RNC_Polygon3D::CRegistInfo* _RNC_DrawMgr::PutPolygon3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {
	
	// 優先度正常チェック
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// 番号カウントが最大数に達した時、頂点バッファを再生成する
	if (_RNC_Polygon3D::CDrawInfo::ms_idxCount == _RNC_Polygon3D::CDrawInfo::ms_allocNum) {
		_RNC_Polygon3D::CDrawInfo::ms_allocPower++;
		_RNC_Polygon3D::CDrawInfo::ms_allocNum = pow(2, _RNC_Polygon3D::CDrawInfo::ms_allocPower);
		_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
		_RNC_Polygon3D::CDrawInfo::CreateVertexBuffer(_RNC_Polygon3D::CDrawInfo::ms_allocNum);
	}

	// 登録情報
	_RNC_Polygon3D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon3D(m_resistInfoSumScreen[priority]) :
		RegistPolygon3D(m_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetIdx(_RNC_Polygon3D::CDrawInfo::ms_idxCount++);
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// 設置処理(テキスト2D)
//========================================
_RNC_Text2D::CRegistInfo* _RNC_DrawMgr::PutText2D(const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen) {

	// 優先度正常チェック
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// 登録情報
	_RNC_Text2D::CRegistInfo* registInfo = isOnScreen ?
		RegistText2D(m_resistInfoSumScreen[priority]) :
		RegistText2D(m_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetPos(pos);
	registInfo->SetAngle(angle);

	return registInfo;
}

//========================================
// 設置処理(テキスト3D)
//========================================
_RNC_Text3D::CRegistInfo* _RNC_DrawMgr::PutText3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// 優先度正常チェック
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// 登録情報
	_RNC_Text3D::CRegistInfo* registInfo = isOnScreen ?
		RegistText3D(m_resistInfoSumScreen[priority]) :
		RegistText3D(m_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// 設置処理(モデル)
//========================================
_RNC_Model::CRegistInfo* _RNC_DrawMgr::PutModel(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// 優先度正常チェック
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// 登録情報
	_RNC_Model::CRegistInfo* registInfo = isOnScreen ?
		RegistModel(m_resistInfoSumScreen[priority]) :
		RegistModel(m_resistInfoSum[priority]);

	// 情報を代入
	registInfo->SetMtx(mtx);

	return registInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]描画マネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 描画処理
//========================================
void _RNC_DrawMgr::Draw(Device& device, CCamera* camera, const bool& isOnScreen) {

	// ビューマトリックスを取得
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// 描画していく
	if (isOnScreen) {
		ExecutionDraw(device, camera, m_drawInfoSumScreen, viewMtx, isOnScreen);
	}
	else {
		ExecutionDraw(device, camera, m_drawInfoSum, viewMtx, isOnScreen);
	}
}

//========================================
// 描画実行処理
//========================================
void _RNC_DrawMgr::ExecutionDraw(Device& device, CCamera* camera, CDrawInfoSum*& drawInfo, Matrix& viewMtx, const bool& isOnSreen) {

	const short   cameraID         = camera == NULL ? NONEDATA : camera->GetID();
	const bool    isCameraClipping = camera == NULL ? false    : camera->GetIsClipping();
	const Pos3D   cameraPosV       = camera == NULL ? Pos3D(0.0f, 0.0f, 0.0f) : camera->GetPosV();
	const Pos3D   cameraPosR       = camera == NULL ? Pos3D(0.0f, 0.0f, 1.0f) : camera->GetPosR();
	const Scale2D cameraScale      = camera == NULL ? RNLib::Window().GetSize() : camera->GetScale2D();
	Matrix  cameraBillboardMtx;
	D3DXMatrixInverse(&cameraBillboardMtx, NULL, &viewMtx);
	cameraBillboardMtx._41 = 0.0f;
	cameraBillboardMtx._42 = 0.0f;
	cameraBillboardMtx._43 = 0.0f;

	// [[[ ライト3Dの設定 ]]]
	bool isFog = false;
	if (camera == NULL) {
		CLight3D::AllDisable(device);
	}
	else {
		const short lightID = camera->GetLightID();
		if (lightID == NONEDATA) {
			CLight3D::AllDisable(device);
		}
		else {
			CLight3D* light = NULL;
			bool      isSetting = false;
			while (RNSystem::GetLight3DMgr().ListLoop((CObject**)&light)) {
				if (light->GetID() == lightID) {
					light->Setting(device, camera->GetOverwriteBGCol());
					isFog = light->GetIsFog();
					isSetting = true;
					break;
				}
			}

			if (!isSetting) {
				CLight3D::AllDisable(device);
			}
		}
	}

	const UShort priorityMax = isOnSreen ? SCREEN_PRIORITY_MAX : m_priorityMax;
	for (int cntPriority = 0; cntPriority < priorityMax; cntPriority++) {

		//----------------------------------------
		// モデル描画
		//----------------------------------------
		if (m_isDrawModel[isOnSreen]) {
			for (int cntModel = 0; cntModel < drawInfo[cntPriority].m_modelNum; cntModel++) {

				if (drawInfo[cntPriority].m_model[cntModel] == NULL)
					continue;

				// クリッピングIDが対象外であれば折り返す
				if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != cameraID)
						continue;

				/*if (!CHitTest::XYZ::InPointToCameraView(CMatrix::ConvMtxToPos(drawInfo[cntPriority].m_model[cntModel]->m_mtx), cameraPosV, cameraPosR, cameraScale.x, cameraScale.y, D3DXToRadian(45.0f)))
					continue;*/

				// 本体のワールドマトリックスの設定
				device->SetTransform(D3DTS_WORLD, &drawInfo[cntPriority].m_model[cntModel]->m_mtx);

				// [[[ Zテストの設定 ]]]
				RNLib::DrawStateMgr().SetIsZTest(device, drawInfo[cntPriority].m_model[cntModel]->m_isZTest);

				// [[[ ライティングを有効/無効にする ]]]
				RNLib::DrawStateMgr().SetIsLighting(device, drawInfo[cntPriority].m_model[cntModel]->m_isLighting);

				// [[[ 補間モードの設定 ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_model[cntModel]->m_interpolationMode);

				// 描画
				for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++) {

					// マテリアルの設定
					device->SetMaterial(&drawInfo[cntPriority].m_model[cntModel]->m_mats[cntMat]);

					// [[[ テクスチャの設定 ]]]
					device->SetTexture(0, drawInfo[cntPriority].m_model[cntModel]->m_texes[cntMat]);

					// 描画
					drawInfo[cntPriority].m_model[cntModel]->m_mesh->DrawSubset(cntMat);
				}

				// 輪郭線の描画
				if (drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh != NULL) {

					// マテリアルの設定
					device->SetMaterial(&_RNC_Model::CDrawInfo::ms_outLineMat);

					// 裏面
					RNLib::DrawStateMgr().SetCullingMode(device, _RNC_DrawState::CULLING_MODE::BACK_SIDE);

					for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++)
						drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh->DrawSubset(cntMat);

					// 表面
					RNLib::DrawStateMgr().SetCullingMode(device, _RNC_DrawState::CULLING_MODE::FRONT_SIDE);
				}
			}

			// 可変設定のリセット
			RNLib::DrawStateMgr().ResetVariableSetting(device);

			{// マテリアルの初期化
				const Material initMat = INITMATERIAL;
				device->SetMaterial(&initMat);
			}
		}

		//----------------------------------------
		// 頂点フォーマットの設定
		//----------------------------------------
		device->SetFVF(FVF_VERTEX_3D);

		//----------------------------------------
		// スタティックメッシュ描画
		//----------------------------------------
		if (m_isDrawStaticMesh[isOnSreen]) {

			// ワールドマトリックスの設定
			device->SetTransform(D3DTS_WORLD, &INITMATRIX);

			RNLib::StaticMesh().Draw(device, cntPriority, cameraID, isCameraClipping, isOnSreen);
		}

		//----------------------------------------
		// ポリゴン3D
		//----------------------------------------
		if (_RNC_Polygon3D::CDrawInfo::ms_vtxBuff != NULL && m_isDrawPolygon3D[isOnSreen]) {

			// 頂点バッファをデータストリームに設定
			device->SetStreamSource(0, _RNC_Polygon3D::CDrawInfo::ms_vtxBuff, 0, sizeof(Vertex3D));

			for (int cntPolygon3D = 0; cntPolygon3D < drawInfo[cntPriority].m_polygon3DNum; cntPolygon3D++) {

				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D] == NULL)
					continue;

				// クリッピングIDが対象外であれば折り返す
				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != cameraID)
						continue;

				// [[[ Zテストの設定 ]]]
				RNLib::DrawStateMgr().SetIsZTest(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isZTest);

				// [[[ ライティングを有効/無効にする ]]]
				RNLib::DrawStateMgr().SetIsLighting(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isLighting);

				// [[[ 加算合成を有効/無効にする ]]]
				RNLib::DrawStateMgr().SetAlphaBlendMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_alphaBlendMode);

				// [[[ カリングの設定 ]]]
				RNLib::DrawStateMgr().SetCullingMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_cullingMode);

				// [[[ 補間モードの設定 ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_interpolationMode);

				// [[[ ビルボードフラグに応じてマトリックスを設定 ]]]
				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isBillboard)
					device->SetTransform(D3DTS_WORLD, &RNLib::Matrix().MultiplyMtx(cameraBillboardMtx, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_mtx));
				else
					device->SetTransform(D3DTS_WORLD, &drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_mtx);

				// [[[ テクスチャの設定 ]]]
				Polygon2DAnd3D::SetTexture(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_tex, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_texType);

				// 描画
				device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_idx, 2);
			}
		}

		// 可変設定のリセット
		RNLib::DrawStateMgr().ResetVariableSetting(device);

		//----------------------------------------
		// ポリゴン2D
		//----------------------------------------
		if (_RNC_Polygon2D::CDrawInfo::ms_vtxBuff != NULL && m_isDrawPolygon2D[isOnSreen]) {

			// ZテストをOFFに
			RNLib::DrawStateMgr().SetIsZTest(device, false);

			// フォグを解除
			if (isFog)
				RNLib::DrawStateMgr().SetIsFog(device, false);

			// 頂点フォーマットの設定
			device->SetFVF(FVF_VERTEX_2D);

			// 頂点バッファをデータストリームに設定
			device->SetStreamSource(0, _RNC_Polygon2D::CDrawInfo::ms_vtxBuff, 0, sizeof(Vertex2D));

			for (int cntPolygon2D = 0; cntPolygon2D < drawInfo[cntPriority].m_polygon2DNum; cntPolygon2D++) {

				if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D] == NULL)
					continue;

				// クリッピングIDが対象外であれば折り返す
				if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != cameraID)
						continue;

				// [[[ 補間モードの設定 ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_interpolationMode);

				// [[[ テクスチャの設定 ]]]
				Polygon2DAnd3D::SetTexture(device, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_tex, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_texType);

				// ポリゴンの描画
				device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_idx, 2);
			}

			// フォグを戻す
			if (isFog)
				RNLib::DrawStateMgr().SetIsFog(device, true);
		}

		// 可変設定のリセット
		RNLib::DrawStateMgr().ResetVariableSetting(device);
	}
}

//========================================
// 頂点情報代入処理
//========================================
void _RNC_DrawMgr::AssignVertexInfo(void) {

	// 頂点2D情報に変換
	if (_RNC_Polygon2D::CDrawInfo::ms_vtxBuff != NULL) {

		// 頂点バッファをロック
		Vertex2D* vtxs = NULL;
		_RNC_Polygon2D::CDrawInfo::ms_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
			ConvDrawInfoToVertex2DInfo(vtxs, m_drawInfoSum[cntPriority]);
		
		for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
			ConvDrawInfoToVertex2DInfo(vtxs, m_drawInfoSumScreen[cntPriority]);

		// 頂点バッファをアンロック
		_RNC_Polygon2D::CDrawInfo::ms_vtxBuff->Unlock();
	}

	// 頂点3D情報に変換
	if (_RNC_Polygon3D::CDrawInfo::ms_vtxBuff != NULL) {

		// 頂点バッファをロック
		Vertex3D* vtxs = NULL;
		_RNC_Polygon3D::CDrawInfo::ms_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
			ConvDrawInfoToVertex3DInfo(vtxs, m_drawInfoSum[cntPriority]);

		for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
			ConvDrawInfoToVertex3DInfo(vtxs, m_drawInfoSumScreen[cntPriority]);

		// 頂点バッファをアンロック
		_RNC_Polygon3D::CDrawInfo::ms_vtxBuff->Unlock();
	}
}

//========================================
// 描画情報を頂点2D情報に変換
//========================================
void _RNC_DrawMgr::ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.m_polygon2DNum; cntDrawInfo++) {

		// 頂点情報を代入
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon2D[cntDrawInfo]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < _RNC_Polygon2D::CDrawInfo::ms_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon2D[cntDrawInfo]->m_vtxs[cntVtx];
		}
	}
}

//========================================
// 描画情報を頂点3D情報に変換
//========================================
void _RNC_DrawMgr::ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntPolygon3D = 0; cntPolygon3D < drawInfoSum.m_polygon3DNum; cntPolygon3D++) {

		// 頂点情報を代入
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon3D[cntPolygon3D]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < _RNC_Polygon3D::CDrawInfo::ms_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon3D[cntPolygon3D]->m_vtxs[cntVtx];
		}
	}
}

//========================================
// 登録処理(ポリゴン2D)
//========================================
_RNC_Polygon2D::CRegistInfo* _RNC_DrawMgr::RegistPolygon2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon2DRegistInfoNum == resistInfo.m_polygon2DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_polygon2DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_polygon2DRegistInfoAllocNum;
		resistInfo.m_polygon2DRegistInfoAllocNum = pow(2, resistInfo.m_polygon2DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		RNLib::Memory().ReAllocDouble(&resistInfo.m_polygon2DRegistInfos, oldAllocNum, resistInfo.m_polygon2DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum++];
}

//========================================
// 登録処理(ポリゴン3D)
//========================================
_RNC_Polygon3D::CRegistInfo* _RNC_DrawMgr::RegistPolygon3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon3DRegistInfoNum == resistInfo.m_polygon3DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_polygon3DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_polygon3DRegistInfoAllocNum;
		resistInfo.m_polygon3DRegistInfoAllocNum = pow(2, resistInfo.m_polygon3DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		RNLib::Memory().ReAllocDouble(&resistInfo.m_polygon3DRegistInfos, oldAllocNum, resistInfo.m_polygon3DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum++];
}

//========================================
// 登録処理(テキスト2D)
//========================================
_RNC_Text2D::CRegistInfo* _RNC_DrawMgr::RegistText2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text2DRegistInfoNum == resistInfo.m_text2DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_text2DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_text2DRegistInfoAllocNum;
		resistInfo.m_text2DRegistInfoAllocNum = pow(2, resistInfo.m_text2DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		RNLib::Memory().ReAllocDouble(&resistInfo.m_text2DRegistInfos, oldAllocNum, resistInfo.m_text2DRegistInfoAllocNum, true);
	}

	resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum++];
}

//========================================
// 登録処理(テキスト3D)
//========================================
_RNC_Text3D::CRegistInfo* _RNC_DrawMgr::RegistText3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text3DRegistInfoNum == resistInfo.m_text3DRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_text3DRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_text3DRegistInfoAllocNum;
		resistInfo.m_text3DRegistInfoAllocNum = pow(2, resistInfo.m_text3DRegistInfoAllocPower);

		// その確保ライン分確保し直す
		RNLib::Memory().ReAllocDouble(&resistInfo.m_text3DRegistInfos, oldAllocNum, resistInfo.m_text3DRegistInfoAllocNum, true);
	}

	resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum++];
}

//========================================
// 登録処理(モデル)
//========================================
_RNC_Model::CRegistInfo* _RNC_DrawMgr::RegistModel(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_modelRegistInfoNum == resistInfo.m_modelRegistInfoAllocNum) {

		// 確保べき乗数を増やす
		resistInfo.m_modelRegistInfoAllocPower++;

		// 確保した数を算出
		const UShort oldAllocNum = resistInfo.m_modelRegistInfoAllocNum;
		resistInfo.m_modelRegistInfoAllocNum = pow(2, resistInfo.m_modelRegistInfoAllocPower);

		// その確保ライン分確保し直す
		RNLib::Memory().ReAllocDouble(&resistInfo.m_modelRegistInfos, oldAllocNum, resistInfo.m_modelRegistInfoAllocNum, true);
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
void _RNC_DrawMgr::PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen) {
	
	//----------------------------------------
	// テキスト3Dからポリゴン3Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text3DRegistInfoNum; cnt++) {
		resistInfoSum.m_text3DRegistInfos[cnt]->PutPolygon3D(priority, isOnScreen);
	}

	resistInfoSum.m_text3DRegistInfoNum = 0;

	//----------------------------------------
	// テキスト2Dからポリゴン2Dを設置
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text2DRegistInfoNum; cnt++) {
		resistInfoSum.m_text2DRegistInfos[cnt]->PutPolygon2D(priority, isOnScreen);
	}

	resistInfoSum.m_text2DRegistInfoNum = 0;
}

//========================================
// [静的]登録情報を描画情報に変換する
//========================================
void _RNC_DrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device) {

	{// 描画情報のメモリ確保
		drawInfoSum.m_modelNum     = resistInfoSum.m_modelRegistInfoNum;
		drawInfoSum.m_polygon3DNum = resistInfoSum.m_polygon3DRegistInfoNum;
		drawInfoSum.m_polygon2DNum = resistInfoSum.m_polygon2DRegistInfoNum;
		RNLib::Memory().Alloc(&drawInfoSum.m_model    , drawInfoSum.m_modelNum    , true);
		RNLib::Memory().Alloc(&drawInfoSum.m_polygon3D, drawInfoSum.m_polygon3DNum, true);
		RNLib::Memory().Alloc(&drawInfoSum.m_polygon2D, drawInfoSum.m_polygon2DNum, true);
	}

	//----------------------------------------
	// モデル
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++) {
		drawInfoSum.m_model[cnt] = resistInfoSum.m_modelRegistInfos[cnt]->ConvToDrawInfo(device);
	}

	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon3D[cnt] = resistInfoSum.m_polygon3DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon2D[cnt] = resistInfoSum.m_polygon2DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon2DRegistInfoNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]描画情報総括クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_DrawMgr::CDrawInfoSum::CDrawInfoSum() {

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
_RNC_DrawMgr::CDrawInfoSum::~CDrawInfoSum() {

}

//========================================
// 解放処理
//========================================
void _RNC_DrawMgr::CDrawInfoSum::Release(void) {

	// 描画情報を解放
	if (m_model != NULL) {
		for (int cnt = 0; cnt < m_modelNum; cnt++) {
			RNLib::Memory().Release(&m_model[cnt]);
		}
		RNLib::Memory().Release(&m_model);
	}
	
	if (m_polygon3D != NULL) {
		for (int cnt = 0; cnt < m_polygon3DNum; cnt++) {
			RNLib::Memory().Release(&m_polygon3D[cnt]);
		}
		RNLib::Memory().Release(&m_polygon3D);
	}

	if (m_polygon2D != NULL) {
		for (int cnt = 0; cnt < m_polygon2DNum; cnt++) {
			RNLib::Memory().Release(&m_polygon2D[cnt]);
		}
		RNLib::Memory().Release(&m_polygon2D);
	}
}

//========================================
// 上書き処理
//========================================
void _RNC_DrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

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
//==========| [公開]登録情報情報総括クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_DrawMgr::CRegistInfoSum::CRegistInfoSum() {

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
_RNC_DrawMgr::CRegistInfoSum::~CRegistInfoSum() {

}

//========================================
// 初期確保処理
//========================================
void _RNC_DrawMgr::CRegistInfoSum::InitAlloc(void) {

	m_polygon2DRegistInfos			= NULL;
	m_polygon3DRegistInfos			= NULL;
	m_text2DRegistInfos   			= NULL;
	m_text3DRegistInfos   			= NULL;
	m_modelRegistInfos    			= NULL;

	m_polygon2DRegistInfoAllocNum   = 
	m_polygon3DRegistInfoAllocNum   = 
	m_text2DRegistInfoAllocNum      = 
	m_text3DRegistInfoAllocNum      = 
	m_modelRegistInfoAllocNum       = 0;

	m_polygon2DRegistInfoAllocPower = 
	m_polygon3DRegistInfoAllocPower = 
	m_text2DRegistInfoAllocPower    = 
	m_text3DRegistInfoAllocPower    = 
	m_modelRegistInfoAllocPower     = 0;
}

//========================================
// 再確保処理
//========================================
void _RNC_DrawMgr::CRegistInfoSum::ReAlloc(void) {

	//----------------------------------------
	// ポリゴン2D
	//----------------------------------------
	if (m_polygon2DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_polygon2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon2DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				RNLib::Memory().ReAllocDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum, allocLine);

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
	if (m_polygon3DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_polygon3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon3DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				RNLib::Memory().ReAllocDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum, allocLine);

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
	if (m_text2DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_text2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text2DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				RNLib::Memory().ReAllocDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum, allocLine);

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
	if (m_text3DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_text3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text3DRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				RNLib::Memory().ReAllocDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum, allocLine);

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
	if (m_modelRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_modelRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_modelRegistInfoNum < allocLine)
			{// 確保ラインよりも数が少ない時、
				// その確保ライン分確保し直す
				RNLib::Memory().ReAllocDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum, allocLine);

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
void _RNC_DrawMgr::CRegistInfoSum::Release() {

	RNLib::Memory().ReleaseDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum);
}