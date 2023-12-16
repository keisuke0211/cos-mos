//========================================
// 
// ドール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include "../../../RNmode.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ドール3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDoll3D::CDoll3D(const UShort& priority, const short& setUpIdx) {

	// リストに追加
	RNLib::Doll3DMgr().AddList(this);

	m_priority             = priority;
	m_clippingID           = NONEDATA;
	m_pos			       = INITPOS3D;
	m_isSetPos             = false;
	m_isShow               = true;
	m_rot			       = INITROT3D;
	m_scale                = INITSCALE3D;
	m_col                  = COLOR_WHITE;
	m_boneStates           = NULL;
	m_motionInfo           = {};
	m_brightnessOfEmission = 1.0f;
	SetUp(setUpIdx);
}

//========================================
// デストラクタ
//========================================
CDoll3D::~CDoll3D() {

	// リストから削除
	RNLib::Doll3DMgr().SubList(this);

	// ボーン状態のメモリ解放
	RNLib::Memory().Release(&m_boneStates);
}

//========================================
// 更新処理
//========================================
void CDoll3D::Update(void) {

	// 位置が設定されていない時、処理を終了する
	if (!m_isSetPos)
		return;

	if (m_setUpIdx != NONEDATA)
	{// セットアップが設定されている時、
		// セットアップデータ取得
		_RNC_SetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

		// ボーンの更新処理
		UpdateBone(setUp);

		// モーションの更新処理
		// (※ボーンの後でなければ、time0のコマンドを通らなくなってしまう)
		UpdateMotion();
	}
}

//========================================
// セットアップ処理
//========================================
void CDoll3D::SetUp(const short& setUpIdx) {

	// セットアップ番号を設定
	m_setUpIdx = setUpIdx;

	// セットアップ番号が無しの時、解放して終了
	if (m_setUpIdx == NONEDATA) {
		RNLib::Memory().Release(&m_boneStates);
		return;
	}

	// セットアップデータ取得
	const _RNC_SetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

	// ボーン状態メモリ確保
	if (&setUp == NULL) {
		RNLib::Memory().Release(&m_boneStates);
	}
	else {
		// 部品数が0を越えている > メモリ確保
		// 部品数が0以下         > メモリ解放
		setUp.m_boneDataNum > 0 ?
			RNLib::Memory().Alloc(&m_boneStates, setUp.m_boneDataNum) :
			RNLib::Memory().Release(&m_boneStates);

		for (int cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

			// 揺れ情報が存在する時、揺れ状態を生成
			if (setUp.m_boneDatas[cntBone].swaying != NULL) {
				m_boneStates[cntBone].CreateSwayingState();
			}
			
			// 親番号が在りの時、同じ番号のボーンを検索し、見つけたら親のポインタを設定する
			m_boneStates[cntBone].SetParentBoneState(NULL);
			if (setUp.m_boneDatas[cntBone].parentIdx != NONEDATA) {
				for (int cntBone2 = 0; cntBone2 < setUp.m_boneDataNum; cntBone2++) {
					if (setUp.m_boneDatas[cntBone].parentIdx == setUp.m_boneDatas[cntBone2].idx) {
						m_boneStates[cntBone].SetParentBoneState(&m_boneStates[cntBone2]);
						break;
					}
				}
			}
		}
	}

	// モーション準備処理
	PrepareMotion();
}

//========================================
// モーション設定処理
//========================================
void CDoll3D::SetMotion(const short& motionIdx) {

	// 番号を代入
	m_motionInfo.idx    =
	m_motionInfo.oldIdx = motionIdx;

	// カウンターを初期化
	m_motionInfo.counter = 0;

	// モーション準備処理
	PrepareMotion();
}

//========================================
// モーション上書き処理
//========================================
void CDoll3D::OverwriteMotion(const short& motionIdx) {

	// 番号が過去の番号と異なる時、
	if (m_motionInfo.oldIdx != motionIdx)
		SetMotion(motionIdx);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]ドール3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// モーションの更新処理
//========================================
void CDoll3D::UpdateMotion(void) {

	// モーションが設定されていない時、終了
	if (m_motionInfo.idx == NONEDATA)
		return;

	// モーションデータ取得
	const _RNC_Motion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);
	if (&motionData == NULL)
		return;

	// モーションカウンターを進める
	if (++m_motionInfo.counter >= motionData.loopTime)
		m_motionInfo.counter = motionData.isLoop ? 0 : motionData.loopTime;
}

//========================================
// ボーンの更新処理
//========================================
void CDoll3D::UpdateBone(_RNC_SetUp3D::CData& setUp) {

	// セットアップデータが存在しない時、処理を終了する
	if (&setUp == NULL)
		return;

	// 部品数0以下の時、処理を終了する
	if (setUp.m_boneDataNum <= 0)
		return;

	// 本体マトリックス
	Matrix selfMtx = RNLib::Matrix().ConvPosRotScaleToMtx(m_pos, m_rot, m_scale);

	// 頂点情報
	_RNC_Model::Vertex3DInfo** vtxInfo = NULL;
	ULong* vtxNum = NULL;
	RNLib::Memory().Alloc(&vtxInfo, setUp.m_boneDataNum);
	RNLib::Memory().Alloc(&vtxNum, setUp.m_boneDataNum);

	// モデルの描画
	for (short cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

		CBoneState& boneState = m_boneStates[cntBone];
		Pos3D       resultPos = INITPOS3D;
		Rot3D       resultRot = INITROT3D;

		// ボーン状態の更新処理
		boneState.Update(m_motionInfo.counter, setUp.m_boneDatas[cntBone]);

		// ワールドマトリックスを調べる
		const Matrix worldMtx = FindBoneWorldMtx(cntBone, m_boneStates, setUp.m_boneDatas, selfMtx);

		// 頂点情報を取得
		vtxNum[cntBone] = RNLib::Model().GetData(setUp.m_boneDatas[cntBone].modelIdx).m_vtxNum;
		vtxInfo[cntBone] = NULL;
		RNLib::Model().StoreVtxInfo(worldMtx, setUp.m_boneDatas[cntBone].modelIdx, &vtxInfo[cntBone]);

		// 表示フラグが真の時、
		if (m_isShow) {

			// モデルの設置処理
			RNLib::Model().Put(m_priority, setUp.m_boneDatas[cntBone].modelIdx, worldMtx)
				->SetCol(m_col)
				->SetBrightnessOfEmissive(m_brightnessOfEmission)
				->SetClippingCamera(m_clippingID);

			// 頂点番号の描画
			if (RNLib::Doll3DMgr().GetEditDoll() == this &&
				RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx() &&
				RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx() == cntBone &&
				RNLib::Doll3DMgr().GetEditCamera() != NULL) {
				DrawModelVtxIdx(vtxInfo[cntBone], vtxNum[cntBone]);
			}
		}
	}

	// 表示フラグが真の時、
	if (m_isShow) {

		// フェイスを描画
		DrawFace(setUp, vtxInfo, vtxNum);
	}

	// 頂点情報を解放
	RNLib::Memory().ReleaseDouble(&vtxInfo, setUp.m_boneDataNum);
	RNLib::Memory().Release(&vtxNum);
}

//========================================
// モデルの頂点番号を描画
//========================================
void CDoll3D::DrawModelVtxIdx(_RNC_Model::Vertex3DInfo*& vtxInfo, ULong& vtxNum) {

	// 頂点番号描画数
	ULong drawVtxIdxNum = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum();
	if (drawVtxIdxNum > vtxNum)
		drawVtxIdxNum = vtxNum;

	// 0を越えている
	if (drawVtxIdxNum > 0) {

		// 頂点番号リストとカメラまでの距離リストを作成
		ULong* vtxIdxs = NULL;
		float* vtxDists = NULL;
		RNLib::Memory().Alloc(&vtxIdxs, vtxNum);
		RNLib::Memory().Alloc(&vtxDists, vtxNum);
		const Pos3D& cameraPos = RNLib::Doll3DMgr().GetEditCamera()->GetPosV();
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
			vtxIdxs[cntVtx] = cntVtx;
			vtxDists[cntVtx] = RNLib::Geometry().FindDistance(cameraPos, vtxInfo[cntVtx].pos);
		}

		// バブルソートを使用して頂点番号リストをソート
		for (ULong cntVtx = 0; cntVtx < vtxNum - 1; ++cntVtx) {
			for (ULong cntVtx2 = 0; cntVtx2 < vtxNum - cntVtx - 1; ++cntVtx2) {
				if (vtxDists[cntVtx2] > vtxDists[cntVtx2 + 1]) {

					// 距離を交換
					float temp = vtxDists[cntVtx2];
					vtxDists[cntVtx2] = vtxDists[cntVtx2 + 1];
					vtxDists[cntVtx2 + 1] = temp;

					// 頂点番号も交換
					int tempIndex = vtxIdxs[cntVtx2];
					vtxIdxs[cntVtx2] = vtxIdxs[cntVtx2 + 1];
					vtxIdxs[cntVtx2 + 1] = tempIndex;
				}
			}
		}

		for (int cntVtx = drawVtxIdxNum - 1; cntVtx >= 0; cntVtx--) {
			bool isOverwrite = false;
			for (ULong cntVtx2 = vtxIdxs[cntVtx] + 1; cntVtx2 < vtxNum; cntVtx2++) {
				if (vtxInfo[vtxIdxs[cntVtx]].worldPos == vtxInfo[cntVtx2].worldPos) {
					isOverwrite = true;
					break;
				}
			}
			if (isOverwrite)
				continue;

			RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, vtxInfo[vtxIdxs[cntVtx]].worldPos, INITROT3D)
				->SetSize(0.25f, 0.25f)
				->SetCol(Color{ 255,0,0,255 })
				->SetLighting(false)
				->SetZTest(false)
				->SetBillboard(true);
			RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, String("%d", vtxIdxs[cntVtx]), _RNC_Text::ALIGNMENT::CENTER, 0, vtxInfo[vtxIdxs[cntVtx]].worldPos, INITROT3D)
				->SetSize(Size2D(0.5f, 0.5f))
				->SetLighting(false)
				->SetZTest(false)
				->SetBillboard(true);
		}

		// 頂点番号リストとカメラまでの距離リストを作成
		RNLib::Memory().Release(&vtxIdxs);
		RNLib::Memory().Release(&vtxDists);
	}
	else if (drawVtxIdxNum == NONEDATA) {

		for (int cntVtx = drawVtxIdxNum - 1; cntVtx >= 0; cntVtx--) {
			bool isOverwrite = false;
			for (ULong cntVtx2 = cntVtx + 1; cntVtx2 < vtxNum; cntVtx2++) {
				if (vtxInfo[cntVtx].worldPos == vtxInfo[cntVtx2].worldPos) {
					isOverwrite = true;
					break;
				}
			}
			if (isOverwrite)
				continue;

			RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, vtxInfo[cntVtx].worldPos, INITROT3D)
				->SetSize(0.25f, 0.25f)
				->SetCol(Color{ 255,0,0,(int)(255 * ((float)(drawVtxIdxNum - cntVtx) / drawVtxIdxNum)) })
				->SetLighting(false)
				->SetZTest(false)
				->SetBillboard(true);
			RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, String("%d", cntVtx), _RNC_Text::ALIGNMENT::CENTER, 0, vtxInfo[cntVtx].worldPos, INITROT3D)
				->SetSize(Size2D(0.5f, 0.5f))
				->SetLighting(false)
				->SetZTest(false)
				->SetBillboard(true)
				->SetCol(Color{ 255,255,255,(int)(255 * ((float)(drawVtxIdxNum - cntVtx) / drawVtxIdxNum)) });
		}
	}
}

//========================================
// フェイスを描画
//========================================
void CDoll3D::DrawFace(_RNC_SetUp3D::CData& setUp, _RNC_Model::Vertex3DInfo**& vtxInfo, ULong*& vtxNum) {

	for (int cntFace = 0; cntFace < setUp.m_faceDataNum; cntFace++) {
		const _RNC_SetUp3D::FaceVtxData& vtx0 = setUp.m_faceDatas[cntFace].vtxs[0];
		const _RNC_SetUp3D::FaceVtxData& vtx1 = setUp.m_faceDatas[cntFace].vtxs[1];
		const _RNC_SetUp3D::FaceVtxData& vtx2 = setUp.m_faceDatas[cntFace].vtxs[2];
		const _RNC_SetUp3D::FaceVtxData& vtx3 = setUp.m_faceDatas[cntFace].vtxs[3];

		if (vtx0.boneIdx >= setUp.m_boneDataNum )continue;
		if (vtx0.vtxIdx  >= vtxNum[vtx0.boneIdx])continue;
		if (vtx1.boneIdx >= setUp.m_boneDataNum )continue;
		if (vtx1.vtxIdx  >= vtxNum[vtx1.boneIdx])continue;
		if (vtx2.boneIdx >= setUp.m_boneDataNum )continue;
		if (vtx2.vtxIdx  >= vtxNum[vtx2.boneIdx])continue;
		if (vtx3.boneIdx >= setUp.m_boneDataNum )continue;
		if (vtx3.vtxIdx  >= vtxNum[vtx3.boneIdx])continue;

		RNLib::Polygon3D().Put(m_priority, INITMATRIX)
			->SetVtxPos(
				vtxInfo[vtx0.boneIdx][vtx0.vtxIdx].worldPos,
				vtxInfo[vtx1.boneIdx][vtx1.vtxIdx].worldPos,
				vtxInfo[vtx2.boneIdx][vtx2.vtxIdx].worldPos,
				vtxInfo[vtx3.boneIdx][vtx3.vtxIdx].worldPos)
			->SetVtxNor(
				vtxInfo[vtx0.boneIdx][vtx0.vtxIdx].worldNor,
				vtxInfo[vtx1.boneIdx][vtx1.vtxIdx].worldNor,
				vtxInfo[vtx2.boneIdx][vtx2.vtxIdx].worldNor,
				vtxInfo[vtx3.boneIdx][vtx3.vtxIdx].worldNor)
			->SetCol(
				setUp.m_faceDatas[cntFace].col)
			->SetTexUV(
				setUp.m_faceDatas[cntFace].texIdx,
				vtx0.texPos,
				vtx1.texPos,
				vtx2.texPos,
				vtx3.texPos)
			->SetClippingCamera(m_clippingID);
	}
}

//========================================
// ボーンのワールドマトリックスを調べる
//========================================
Matrix CDoll3D::FindBoneWorldMtx(const short& idx, CBoneState*& boneState, _RNC_SetUp3D::BoneData*& boneData, Matrix& selfMtx) {

	Matrix  worldMtx  = INITMATRIX;
	Pos3D   resultPos = boneState[idx].GetAddPos() + boneState[idx].GetAnimPos() + boneData[idx].relativePos;
	Rot3D   resultRot = boneState[idx].GetAddRot() + boneState[idx].GetAnimRot() + boneData[idx].relativeRot;
	Scale3D resultScale;
	resultScale.x = boneState[idx].GetAddScale().x * boneState[idx].GetAnimScale().x;
	resultScale.y = boneState[idx].GetAddScale().y * boneState[idx].GetAnimScale().y;
	resultScale.z = boneState[idx].GetAddScale().z * boneState[idx].GetAnimScale().z;
	Matrix  parentMtx   = INITMATRIX;

	// 追従処理
	if (boneData[idx].follow != NULL) {
		Pos3D   followPos   = boneState[boneData[idx].follow->followIdx].GetAddPos()   + boneState[boneData[idx].follow->followIdx].GetAnimPos();
		Rot3D   followRot   = boneState[boneData[idx].follow->followIdx].GetAddRot()   + boneState[boneData[idx].follow->followIdx].GetAnimRot();
		Scale3D followScale = boneState[boneData[idx].follow->followIdx].GetAddScale();
		followScale.x *= boneState[boneData[idx].follow->followIdx].GetAnimScale().x;
		followScale.y *= boneState[boneData[idx].follow->followIdx].GetAnimScale().y;
		followScale.z *= boneState[boneData[idx].follow->followIdx].GetAnimScale().z;
		resultPos.x += followPos.x * boneData[idx].follow->posRate.x;
		resultPos.y += followPos.y * boneData[idx].follow->posRate.y;
		resultPos.z += followPos.z * boneData[idx].follow->posRate.z;
		resultRot.x += followRot.x * boneData[idx].follow->rotRate.x;
		resultRot.y += followRot.y * boneData[idx].follow->rotRate.y;
		resultRot.z += followRot.z * boneData[idx].follow->rotRate.z;
		resultScale.x += (followScale.x - 1.0f) * boneData[idx].follow->scaleRate.x;
		resultScale.y += (followScale.y - 1.0f) * boneData[idx].follow->scaleRate.y;
		resultScale.z += (followScale.z - 1.0f) * boneData[idx].follow->scaleRate.z;
	}

	{// 揺れ状態である時、揺れの加算位置を加算
		CBoneState::SwayingState*& swayingState = boneState[idx].GetSwayingState();
		if (swayingState != NULL)
			resultPos += swayingState->addPos;
	}

	// 親ボーンがない時、
	if (boneState[idx].GetParentBoneState() == NULL) {

		// ドール自体の拡大倍率を乗算
		resultScale.x *= m_scale.x;
		resultScale.y *= m_scale.y;
		resultScale.z *= m_scale.z;

		// 親マトリックスをドール自体のマトリックスに設定
		parentMtx = selfMtx;
	}
	else 
	{// 親ボーンがある時、
		// 親マトリックスを親ボーンのマトリックスに設定
		parentMtx = boneState[idx].GetParentBoneState()->GetWorldMtx();
	}

	// ワールドマトリックスを算出
	worldMtx = RNLib::Matrix().MultiplyMtx(RNLib::Matrix().ConvPosRotScaleToMtx(resultPos, resultRot, resultScale), parentMtx);

	// ワールドマトリックス設定
	boneState[idx].SetWorldMtx(worldMtx);

	return worldMtx;
}

//========================================
// モーションの準備処理
//========================================
void CDoll3D::PrepareMotion(void) {

	// セットアップデータが存在しない時、終了
	const _RNC_SetUp3D::CData& setUpData = RNLib::SetUp3D().GetData(m_setUpIdx);
	if (&setUpData == NULL)
		return;

	// セットアップデータが存在しない時、終了
	const _RNC_Motion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);
	if (&motionData == NULL)
		return;

	// ボーン毎に準備処理
	for (int cntBone = 0; cntBone < setUpData.m_boneDataNum; cntBone++) {

		// ボーン毎のモーションデータポインタを渡す
		for (int cntMotionBone = 0; cntMotionBone < motionData.boneNum; cntMotionBone++) {
			if (setUpData.m_boneDatas[cntBone].idx == cntMotionBone){
				m_boneStates[cntBone].SetMotionData(&motionData.boneMotionDatas[cntMotionBone]);
				break;
			}
		}

		// 準備処理
		m_boneStates[cntBone].PrepareMotion(motionData.boneMotionDatas[cntBone]);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ボーン状態クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDoll3D::CBoneState::CBoneState() {

	m_animPos         = INITPOS3D;
	m_addPos          = INITPOS3D;
	m_animRot         = INITROT3D;
	m_addRot          = INITROT3D;
	m_animScale       = INITSCALE3D;
	m_addScale        = INITSCALE3D;
	m_worldMtx		  = INITMATRIX;
	m_animeStateSum   = {};
	m_swayingState    = NULL;
	m_motionData      = NULL;
	m_parentBoneState = NULL;
}

//========================================
// デストラクタ
//========================================
CDoll3D::CBoneState::~CBoneState() {

	// 揺れ状態のメモリ解放
	RNLib::Memory().Release(&m_swayingState);

	// アニメの情報を解放
	RNLib::Memory().Release(&m_animeStateSum.move);
	RNLib::Memory().Release(&m_animeStateSum.spin);
	RNLib::Memory().Release(&m_animeStateSum.scaling);
}

//========================================
// 更新処理
//========================================
void CDoll3D::CBoneState::Update(const short& motionCounter, const _RNC_SetUp3D::BoneData& boneData) {

	// 揺れ状態
	if (m_swayingState != NULL) {
		
		if (--m_swayingState->counter <= 0) {
			m_swayingState->counterMax   = boneData.swaying->timeMin + rand() % boneData.swaying->timeAdd;
			m_swayingState->counter      = m_swayingState->counterMax;
			m_swayingState->oldAddPos    = m_swayingState->addPos;
			m_swayingState->targetAddPos = RNLib::Geometry().GetRandomVec() * (boneData.swaying->distMin + RNLib::Number().GetRandomFloat(1.0f) * boneData.swaying->distAdd);
		}

		float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_swayingState->counter, m_swayingState->counterMax);
		m_swayingState->addPos = (m_swayingState->oldAddPos * rate) + (m_swayingState->targetAddPos * (1.0f - rate));
	}

	//----------------------------------------
	// コマンド読み取り
	//----------------------------------------
	if (m_motionData != NULL){
		for (int cntCommand = 0; cntCommand < m_motionData->commandDataNum; cntCommand++) {

			// カウンターが実行時間と一致していない時、折り返す
			if (motionCounter != m_motionData->commandDatas[cntCommand].time)
				continue;

			// コマンドに応じた処理
			switch (m_motionData->commandDatas[cntCommand].command) {
				// [[[ 移動 ]]]
			case _RNC_Motion3D::COMMAND::MOVE: {
				_RNC_Motion3D::CommandData_Move& commandData = *(_RNC_Motion3D::CommandData_Move*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// 移動にかかる時間が0の時、
					// 位置を直接代入
					m_animPos = commandData.targetPos;
				}
				else
				{// 移動にかかる時間が0でない時、
					// 移動アニメ状態メモリを確保
					RNLib::Memory().Alloc(&m_animeStateSum.move);

					// 移動アニメ状態を設定
					m_animeStateSum.move->posEase   = commandData.easeType;
					m_animeStateSum.move->oldPos    = m_animPos;
					m_animeStateSum.move->targetPos = commandData.targetPos;
					m_animeStateSum.move->time      = commandData.time;
				}
			}break;
				// [[[ 回転 ]]]
			case _RNC_Motion3D::COMMAND::SPIN: {
				_RNC_Motion3D::CommandData_Spin& commandData = *(_RNC_Motion3D::CommandData_Spin*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// 回転にかかる時間が0の時、
					// 向きを直接代入
					m_animRot = commandData.targetRot;
				}
				else
				{// 回転にかかる時間が0でない時、
					// 回転アニメ状態メモリを確保
					RNLib::Memory().Alloc(&m_animeStateSum.spin);

					// 回転アニメ状態を設定
					m_animeStateSum.spin->rotEase   = commandData.easeType;
					m_animeStateSum.spin->oldRot    = m_animRot;
					m_animeStateSum.spin->targetRot = commandData.targetRot;
					m_animeStateSum.spin->time      = commandData.time;
				}
			}break;
				// [[[ 拡縮 ]]]
			case _RNC_Motion3D::COMMAND::SCALING: {
				_RNC_Motion3D::CommandData_Scaling& commandData = *(_RNC_Motion3D::CommandData_Scaling*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// 拡縮にかかる時間が0の時、
					// 拡大倍率を直接代入
					m_animScale = commandData.targetScale;
				}
				else
				{// 拡縮にかかる時間が0でない時、
					// 拡縮アニメ状態メモリを確保
					RNLib::Memory().Alloc(&m_animeStateSum.scaling);

					// 拡縮アニメ状態を設定
					m_animeStateSum.scaling->scaleEase   = commandData.easeType;
					m_animeStateSum.scaling->oldScale    = m_animScale;
					m_animeStateSum.scaling->targetScale = commandData.targetScale;
					m_animeStateSum.scaling->time        = commandData.time;
				}
			}break;
			}
		}
	}

	//----------------------------------------
	// アニメ状態更新処理
	//----------------------------------------
	// [[[ 移動 ]]]
	if (m_animeStateSum.move != NULL) {

		// 割合を調べる
		const float rate = RNLib::Ease().Easing(m_animeStateSum.move->posEase, m_animeStateSum.move->counter, m_animeStateSum.move->time);

		// 位置を更新
		m_animPos = (m_animeStateSum.move->oldPos * (1.0f - rate)) + (m_animeStateSum.move->targetPos * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.move->counter >= m_animeStateSum.move->time)
			RNLib::Memory().Release(&m_animeStateSum.move);
	}

	// [[[ 回転 ]]]
	if (m_animeStateSum.spin != NULL) {

		// 割合を調べる
		const float rate = RNLib::Ease().Easing(m_animeStateSum.spin->rotEase, m_animeStateSum.spin->counter, m_animeStateSum.spin->time);

		// 向きを更新
		m_animRot = (m_animeStateSum.spin->oldRot * (1.0f - rate)) + (m_animeStateSum.spin->targetRot * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.spin->counter >= m_animeStateSum.spin->time)
			RNLib::Memory().Release(&m_animeStateSum.spin);
	}

	// [[[ 拡縮 ]]]
	if (m_animeStateSum.scaling != NULL) {

		// 割合を調べる
		const float rate = RNLib::Ease().Easing(m_animeStateSum.scaling->scaleEase, m_animeStateSum.scaling->counter, m_animeStateSum.scaling->time);

		// 拡大倍率を更新
		m_animScale = (m_animeStateSum.scaling->oldScale * (1.0f - rate)) + (m_animeStateSum.scaling->targetScale * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.scaling->counter >= m_animeStateSum.scaling->time)
			RNLib::Memory().Release(&m_animeStateSum.scaling);
	}
}

//========================================
// モーション準備処理
//========================================
void CDoll3D::CBoneState::PrepareMotion(const _RNC_Motion3D::BoneMotionData& boneMotionData) {

	// メモリ解放
	RNLib::Memory().Release(&m_animeStateSum.move);
	RNLib::Memory().Release(&m_animeStateSum.spin);
	RNLib::Memory().Release(&m_animeStateSum.scaling);

	// 移動しないモーションの時、位置変更しているのであれば、
	if (!boneMotionData.isMove && m_animPos != INITPOS3D) {
		
		// 移動情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.move);

		// 初期位置に移動させる
		m_animeStateSum.move->oldPos    = m_animPos;
		m_animeStateSum.move->targetPos = INITPOS3D;
		m_animeStateSum.move->time      = PAUSE_RESET_TIME;
		m_animeStateSum.move->posEase   = _RNC_Ease::TYPE::LINEAR;
	}

	// 回転しないモーションの時、向き変更しているのであれば、
	if (!boneMotionData.isSpin && m_animRot != INITROT3D) {
		
		// 回転情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.spin);

		// 初期向きに回転させる
		m_animeStateSum.spin->oldRot    = m_animRot;
		m_animeStateSum.spin->targetRot = INITROT3D;
		m_animeStateSum.spin->time      = PAUSE_RESET_TIME;
		m_animeStateSum.spin->rotEase   = _RNC_Ease::TYPE::LINEAR;
	}

	// 拡縮しないモーションの時、拡大倍率変更しているのであれば、
	if (!boneMotionData.isScale && m_animScale != INITSCALE3D) {
		
		// 拡縮情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.scaling);

		// 初期拡大倍率に拡縮させる
		m_animeStateSum.scaling->oldScale    = m_animScale;
		m_animeStateSum.scaling->targetScale = INITSCALE3D;
		m_animeStateSum.scaling->time        = PAUSE_RESET_TIME;
		m_animeStateSum.scaling->scaleEase   = _RNC_Ease::TYPE::LINEAR;
	}
}