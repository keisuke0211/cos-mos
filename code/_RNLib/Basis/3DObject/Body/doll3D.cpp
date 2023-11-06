//========================================
// 
// ドール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//****************************************
// マクロ定義
//****************************************
#define PAUSE_RESET_TIME (10)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ドール3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDoll3D::CDoll3D(void) {

	m_pos			       = INITPOS3D;
	m_rot			       = INITROT3D;
	m_scale                = INITSCALE3D;
	m_col                  = INITCOLOR;
	m_brightnessOfEmission = 1.0f;
	m_boneStates           = NULL;
	m_setUpIdx             = NONEDATA;
	m_motionInfo           = {};
}

//========================================
// デストラクタ
//========================================
CDoll3D::~CDoll3D(void) {

	// ボーン状態のメモリ解放
	RNLib::Memory().Release(&m_boneStates);
}

//========================================
// 更新処理
//========================================
void CDoll3D::Update(void) {

	if (m_setUpIdx != NONEDATA)
	{// セットアップが設定されている時、
		// セットアップデータ取得
		const CSetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

		// モーションの更新処理
		UpdateMotion();

		// ボーンの更新処理
		UpdateBone(setUp);
	}
}

//========================================
// セットアップ処理
//========================================
void CDoll3D::SetUp(const short& setUpIdx) {

	// セットアップデータ取得
	const CSetUp3D::CData& setUp = RNLib::SetUp3D().GetData(setUpIdx);

	//----------------------------------------
	// ボーン状態メモリ
	// 部品数が0を越えている > メモリ確保
	// 部品数が0以下         > メモリ解放
	//----------------------------------------
	if (setUp.m_boneDataNum > 0)
		RNLib::Memory().Alloc<CBoneState>(&m_boneStates, setUp.m_boneDataNum);
	else
		RNLib::Memory().Release<CBoneState>(&m_boneStates);
}

//========================================
// モーション設定処理
//========================================
void CDoll3D::SetMotion(const short& motionIdx) {

	// 番号を代入
	m_motionInfo.idx     =
	m_motionInfo.oldIdx  = motionIdx;

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
//==========| [非公開]ドール3Dクラスのメンバ関数
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
	const CMotion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);

	// モーションカウンターを進める
	if (++m_motionInfo.counter >= motionData.loopTime)
		m_motionInfo.counter = motionData.isLoop ? 0 : m_motionInfo.counter = motionData.loopTime;
}

//========================================
// ボーンの更新処理
//========================================
void CDoll3D::UpdateBone(const CSetUp3D::CData& setUp) {

	// 部品数0以下の時、処理を終了する
	if (setUp.m_boneDataNum <= 0)
		return;

	// 本体マトリックス
	const Matrix selfMtx = CMatrix::ConvPosRotScaleToMtx(m_pos, m_rot, m_scale);

	for (int cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

		CBoneState& boneState = m_boneStates[cntBone];
		Pos3D       resultPos = INITPOS3D;
		Rot3D       resultRot = INITROT3D;

		// モーションの更新処理
		boneState.UpdateMotion(m_motionInfo.counter);

		// ワールドマトリックスを調べる
		const Matrix worldMtx = FindBoneWorldMtx(boneState, setUp.m_boneDatas[cntBone], selfMtx);

		// モデルの設置処理
		RNLib::Model().Put(worldMtx, setUp.m_boneDatas[cntBone].modelIdx)
			->SetCol(m_col)
			->SetOutLine(true)
			->SetBrightnessOfEmissive(m_brightnessOfEmission);
	}
}

//========================================
// ボーンのワールドマトリックスを調べる
//========================================
Matrix CDoll3D::FindBoneWorldMtx(CBoneState& boneState, const CSetUp3D::BoneData& boneData, const Matrix& selfMtx) {

	Matrix  worldMtx    = INITMATRIX;
	Pos3D   resultPos   = boneState.GetPos() + boneData.relativePos;
	Rot3D   resultRot   = boneState.GetRot() + boneData.relativeRot;
	Scale3D resultScale = boneState.GetScale();
	Matrix  parentMtx   = INITMATRIX;

	// 親ボーンがない時、
	if (boneState.GetParentBoneState() == NULL) {

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
		parentMtx = boneState.GetParentBoneState()->GetWorldMtx();
	}

	// ワールドマトリックスを算出
	worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosRotScaleToMtx(resultPos, resultRot, resultScale), parentMtx);

	// ワールドマトリックス設定
	boneState.SetWorldMtx(worldMtx);

	return worldMtx;
}

//========================================
// モーションの準備処理
//========================================
void CDoll3D::PrepareMotion(void) {

	const CMotion3D::CData& motionData(RNLib::Motion3D().GetData(m_motionInfo.idx));	// モーションデータ
	const CSetUp3D::CData&  setUpData(RNLib::SetUp3D().GetData(m_setUpIdx));			// セットアップデータ

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
//==========| [公開]ボーン状態クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDoll3D::CBoneState::CBoneState() {

	m_pos			  = INITPOS3D;
	m_rot			  = INITROT3D;
	m_scale			  = Scale3D(1.0f, 1.0f, 1.0f);
	m_worldMtx		  = INITMATRIX;
	m_animeStateSum   = {};
	m_motionData      = NULL;
	m_parentBoneState = NULL;
}

//========================================
// デストラクタ
//========================================
CDoll3D::CBoneState::~CBoneState() {

}

//========================================
// モーション更新処理
//========================================
void CDoll3D::CBoneState::UpdateMotion(const short& counter) {

	// 足踏フラグを偽にしておく
	m_animeStateSum.isStep = false;

	//----------------------------------------
	// コマンド読み取り
	//----------------------------------------
	for (int cntCommand = 0; cntCommand < m_motionData->commandDataNum; cntCommand++) {

		// カウンターが実行時間と一致していない時、折り返す
		if (counter != m_motionData->commandDatas[cntCommand].time)
			continue;

		CMotion3D::CommandData& commandData(m_motionData->commandDatas[cntCommand]);

		// コマンドに応じた処理
		switch (commandData.command) {
			// [[[ 移動 ]]]
		case CMotion3D::COMMAND::MOVE: {
			const Pos3D       targetPos = Pos3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
			const int         moveTime  = (int)commandData.datas[3];
			const CEase::TYPE ease      = (CEase::TYPE)((int)commandData.datas[4]);

			if (moveTime == 0)
			{// 移動にかかる時間が0の時、
				// 位置を直接代入
				m_pos = targetPos;
			}
			else
			{// 移動にかかる時間が0でない時、
				// 移動アニメ状態メモリを確保
				RNLib::Memory().Alloc(&m_animeStateSum.move);

				// 移動アニメ状態を設定
				m_animeStateSum.move->posEase   = ease;
				m_animeStateSum.move->oldPos    = m_pos;
				m_animeStateSum.move->targetPos = targetPos;
				m_animeStateSum.move->time      = moveTime;
			}
		}break;
			// [[[ 回転 ]]]
		case CMotion3D::COMMAND::SPIN: {
			const Rot3D       targetRot = Rot3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
			const int         spinTime  = (int)commandData.datas[3];
			const CEase::TYPE ease      = (CEase::TYPE)((int)commandData.datas[4]);

			if (spinTime == 0)
			{// 回転にかかる時間が0の時、
				// 向きを直接代入
				m_rot = targetRot;
			}
			else
			{// 回転にかかる時間が0でない時、
				// 回転アニメ状態メモリを確保
				RNLib::Memory().Alloc(&m_animeStateSum.spin);

				// 回転アニメ状態を設定
				m_animeStateSum.spin->rotEase   = ease;
				m_animeStateSum.spin->oldRot    = m_rot;
				m_animeStateSum.spin->targetRot = targetRot;
				m_animeStateSum.spin->time      = spinTime;
			}
		}break;
			// [[[ 拡縮 ]]]
		case CMotion3D::COMMAND::SCALING: {
			const Scale3D     targetScale = Scale3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
			const int         scalingTime = (int)commandData.datas[3];
			const CEase::TYPE ease        = (CEase::TYPE)((int)commandData.datas[4]);

			if (scalingTime == 0)
			{// 拡縮にかかる時間が0の時、
				// 拡大倍率を直接代入
				m_scale = targetScale;
			}
			else
			{// 拡縮にかかる時間が0でない時、
				// 拡縮アニメ状態メモリを確保
				RNLib::Memory().Alloc(&m_animeStateSum.scaling);

				// 拡縮アニメ状態を設定
				m_animeStateSum.scaling->scaleEase   = ease;
				m_animeStateSum.scaling->oldScale    = m_scale;
				m_animeStateSum.scaling->targetScale = targetScale;
				m_animeStateSum.scaling->time        = scalingTime;
			}
		}break;
			// [[[ 足踏 ]]]
		case CMotion3D::COMMAND::STEP: {

			// 足踏フラグを真にしておく
			m_animeStateSum.isStep = true;
		}break;
		}
	}

	//----------------------------------------
	// アニメ状態更新処理
	//----------------------------------------
	// [[[ 移動 ]]]
	if (m_animeStateSum.move != NULL) {

		// 割合を調べる
		const float rate = CEase::Easing(m_animeStateSum.move->posEase, m_animeStateSum.move->counter, m_animeStateSum.move->time);

		// 位置を更新
		m_pos = (m_animeStateSum.move->oldPos * (1.0f - rate)) + (m_animeStateSum.move->targetPos * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.move->counter >= m_animeStateSum.move->time) {
			RNLib::Memory().Release(&m_animeStateSum.move);
		}
	}

	// [[[ 回転 ]]]
	if (m_animeStateSum.spin != NULL) {

		// 割合を調べる
		const float rate = CEase::Easing(m_animeStateSum.spin->rotEase, m_animeStateSum.spin->counter, m_animeStateSum.spin->time);

		// 向きを更新
		m_rot = (m_animeStateSum.spin->oldRot * (1.0f - rate)) + (m_animeStateSum.spin->targetRot * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.spin->counter >= m_animeStateSum.spin->time) {
			RNLib::Memory().Release(&m_animeStateSum.spin);
		}
	}

	// [[[ 拡縮 ]]]
	if (m_animeStateSum.scaling != NULL) {

		// 割合を調べる
		const float rate = CEase::Easing(m_animeStateSum.scaling->scaleEase, m_animeStateSum.scaling->counter, m_animeStateSum.scaling->time);

		// 拡大倍率を更新
		m_scale = (m_animeStateSum.scaling->oldScale * (1.0f - rate)) + (m_animeStateSum.scaling->targetScale * rate);

		// カウンター到達時、メモリ解放
		if (++m_animeStateSum.scaling->counter >= m_animeStateSum.scaling->time) {
			RNLib::Memory().Release(&m_animeStateSum.scaling);
		}
	}
}

//========================================
// モーション準備処理
//========================================
void CDoll3D::CBoneState::PrepareMotion(const CMotion3D::BoneMotionData& boneMotionData) {

	// メモリ解放
	RNLib::Memory().Release(&m_animeStateSum.move);
	RNLib::Memory().Release(&m_animeStateSum.spin);
	RNLib::Memory().Release(&m_animeStateSum.scaling);

	// 移動しないモーションの時、位置変更しているのであれば、
	if (!boneMotionData.isMove && m_pos != INITPOS3D) {
		
		// 移動情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.move);

		// 初期位置に移動させる
		m_animeStateSum.move->oldPos    = m_pos;
		m_animeStateSum.move->targetPos = INITPOS3D;
		m_animeStateSum.move->time      = PAUSE_RESET_TIME;
		m_animeStateSum.move->posEase   = CEase::TYPE::LINEAR;
	}

	// 回転しないモーションの時、向き変更しているのであれば、
	if (!boneMotionData.isSpin && m_rot != INITROT3D) {
		
		// 回転情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.spin);

		// 初期向きに回転させる
		m_animeStateSum.spin->oldRot    = m_rot;
		m_animeStateSum.spin->targetRot = INITROT3D;
		m_animeStateSum.spin->time      = PAUSE_RESET_TIME;
		m_animeStateSum.spin->rotEase   = CEase::TYPE::LINEAR;
	}

	// 拡縮しないモーションの時、拡大倍率変更しているのであれば、
	if (!boneMotionData.isScale && m_scale != INITSCALE3D) {
		
		// 拡縮情報のメモリを確保し、
		RNLib::Memory().Alloc(&m_animeStateSum.scaling);

		// 初期拡大倍率に拡縮させる
		m_animeStateSum.scaling->oldScale    = m_scale;
		m_animeStateSum.scaling->targetScale = INITSCALE3D;
		m_animeStateSum.scaling->time        = PAUSE_RESET_TIME;
		m_animeStateSum.scaling->scaleEase   = CEase::TYPE::LINEAR;
	}
}