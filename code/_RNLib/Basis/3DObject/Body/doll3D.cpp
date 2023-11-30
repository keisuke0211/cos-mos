//========================================
// 
// �h�[��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include "../../../RNmode.h"

//****************************************
// �}�N����`
//****************************************
#define PAUSE_RESET_TIME (10)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�h�[��3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CDoll3D::CDoll3D(const UShort& priority, const short& setUpIdx) {

	RNLib::Doll3DMgr().AddList(this);
	m_priority             = priority;
	m_pos			       = INITPOS3D;
	m_isSetPos             = false;
	m_rot			       = INITROT3D;
	m_scale                = INITSCALE3D;
	m_col                  = INITCOLOR;
	m_boneStates           = NULL;
	m_motionInfo           = {};
	m_brightnessOfEmission = 1.0f;
	SetUp(setUpIdx);
}

//========================================
// �f�X�g���N�^
//========================================
CDoll3D::~CDoll3D() {

	RNLib::Doll3DMgr().SubList(this);

	// �{�[����Ԃ̃��������
	CMemory::Release(&m_boneStates);
}

//========================================
// �X�V����
//========================================
void CDoll3D::Update(void) {

	// �ʒu���ݒ肳��Ă��Ȃ����A�������I������
	if (!m_isSetPos)
		return;

	if (m_setUpIdx != NONEDATA)
	{// �Z�b�g�A�b�v���ݒ肳��Ă��鎞�A
		// �Z�b�g�A�b�v�f�[�^�擾
		CSetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

		// �{�[���̍X�V����
		UpdateBone(setUp);

		// ���[�V�����̍X�V����
		// (���{�[���̌�łȂ���΁Atime0�̃R�}���h��ʂ�Ȃ��Ȃ��Ă��܂�)
		UpdateMotion();
	}
}

//========================================
// �Z�b�g�A�b�v����
//========================================
void CDoll3D::SetUp(const short& setUpIdx) {

	// �Z�b�g�A�b�v�ԍ���ݒ�
	m_setUpIdx = setUpIdx;

	// �Z�b�g�A�b�v�ԍ��������̎��A������ďI��
	if (m_setUpIdx == NONEDATA) {
		CMemory::Release(&m_boneStates);
		return;
	}

	// �Z�b�g�A�b�v�f�[�^�擾
	const CSetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

	// �{�[����ԃ������m��
	if (&setUp == NULL) {
		CMemory::Release(&m_boneStates);
	}
	else {
		// ���i����0���z���Ă��� > �������m��
		// ���i����0�ȉ�         > ���������
		setUp.m_boneDataNum > 0 ?
			CMemory::Alloc(&m_boneStates, setUp.m_boneDataNum) :
			CMemory::Release(&m_boneStates);

		for (int cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

			// �h���񂪑��݂��鎞�A�h���Ԃ𐶐�
			if (setUp.m_boneDatas[cntBone].swaying != NULL) {
				m_boneStates[cntBone].CreateSwayingState();
			}

			if (setUp.m_boneDatas[cntBone].parentIdx != NONEDATA) 
			{// �e�ԍ����݂�̎��A
				// �����ԍ��̃{�[�����������A��������e�̃|�C���^��ݒ肷��
				for (int cntBone2 = 0; cntBone2 < setUp.m_boneDataNum; cntBone2++) {
					if (setUp.m_boneDatas[cntBone].parentIdx == setUp.m_boneDatas[cntBone2].idx) {
						m_boneStates[cntBone].SetParentBoneState(&m_boneStates[cntBone2]);
						break;
					}
				}
			}
		}
	}

	// ���[�V������������
	PrepareMotion();
}

//========================================
// ���[�V�����ݒ菈��
//========================================
void CDoll3D::SetMotion(const short& motionIdx) {

	// �ԍ�����
	m_motionInfo.idx     =
	m_motionInfo.oldIdx  = motionIdx;

	// �J�E���^�[��������
	m_motionInfo.counter = 0;

	// ���[�V������������
	PrepareMotion();
}

//========================================
// ���[�V�����㏑������
//========================================
void CDoll3D::OverwriteMotion(const short& motionIdx) {

	// �ԍ����ߋ��̔ԍ��ƈقȂ鎞�A
	if (m_motionInfo.oldIdx != motionIdx)
		SetMotion(motionIdx);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�h�[��3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ���[�V�����̍X�V����
//========================================
void CDoll3D::UpdateMotion(void) {

	// ���[�V�������ݒ肳��Ă��Ȃ����A�I��
	if (m_motionInfo.idx == NONEDATA)
		return;

	// ���[�V�����f�[�^�擾
	const CMotion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);

	// ���[�V�����J�E���^�[��i�߂�
	if (++m_motionInfo.counter >= motionData.loopTime)
		m_motionInfo.counter = motionData.isLoop ? 0 : motionData.loopTime;
}

//========================================
// �{�[���̍X�V����
//========================================
void CDoll3D::UpdateBone(CSetUp3D::CData& setUp) {

	// �Z�b�g�A�b�v�f�[�^�����݂��Ȃ����A�������I������
	if (&setUp == NULL)
		return;

	// ���i��0�ȉ��̎��A�������I������
	if (setUp.m_boneDataNum <= 0)
		return;

	// �{�̃}�g���b�N�X
	const Matrix selfMtx = CMatrix::ConvPosRotScaleToMtx(m_pos, m_rot, m_scale);

	// ���_���
	CModel::Vertex3DInfo** vtxInfo = NULL;
	UInt* vtxNum = NULL;
	CMemory::Alloc(&vtxInfo, setUp.m_boneDataNum);
	CMemory::Alloc(&vtxNum, setUp.m_boneDataNum);

	// ���f���̕`��
	for (int cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

		CBoneState& boneState = m_boneStates[cntBone];
		Pos3D       resultPos = INITPOS3D;
		Rot3D       resultRot = INITROT3D;

		// �{�[���̍X�V����
		boneState.Update(m_motionInfo.counter, setUp.m_boneDatas[cntBone]);

		// ���[���h�}�g���b�N�X�𒲂ׂ�
		const Matrix worldMtx = FindBoneWorldMtx(cntBone, m_boneStates, setUp.m_boneDatas, selfMtx);

		// ���f���̐ݒu����
		RNLib::Model().Put(m_priority, setUp.m_boneDatas[cntBone].modelIdx, worldMtx)
			->SetCol(m_col)
			->SetBrightnessOfEmissive(m_brightnessOfEmission);

		// ���_�����擾
		vtxNum[cntBone] = 0;
		vtxInfo[cntBone] = NULL;
		RNLib::Model().StoreVtxInfo(&vtxNum[cntBone], &vtxInfo[cntBone], setUp.m_boneDatas[cntBone].modelIdx, worldMtx);
		// OK

		// ���_�ԍ��̕`��
		if (RNLib::Doll3DMgr().GetEditDoll() == this &&
			RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx() &&
			RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx() == cntBone &&
			RNLib::Doll3DMgr().GetEditCamera() != NULL) {

			// ���_�ԍ��`�搔
			UInt drawVtxIdxNum = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum();
			if (drawVtxIdxNum > vtxNum[cntBone])
				drawVtxIdxNum = vtxNum[cntBone];

			// 0���z���Ă���
			if (drawVtxIdxNum > 0) {

				// ���_�ԍ����X�g�ƃJ�����܂ł̋������X�g���쐬
				UShort* vtxIdxs = NULL;
				float* vtxDists = NULL;
				CMemory::Alloc(&vtxIdxs, vtxNum[cntBone]);
				CMemory::Alloc(&vtxDists, vtxNum[cntBone]);
				const Pos3D& cameraPos = RNLib::Doll3DMgr().GetEditCamera()->GetPosV();
				for (UShort cntVtx = 0; cntVtx < vtxNum[cntBone]; cntVtx++) {
					vtxIdxs[cntVtx] = cntVtx;
					vtxDists[cntVtx] = CGeometry::FindDistance(cameraPos, vtxInfo[cntBone][cntVtx].pos);
				}

				// �o�u���\�[�g���g�p���Ē��_�ԍ����X�g���\�[�g
				for (UShort cntVtx = 0; cntVtx < vtxNum[cntBone] - 1; ++cntVtx) {
					for (UShort cntVtx2 = 0; cntVtx2 < vtxNum[cntBone] - cntVtx - 1; ++cntVtx2) {
						if (vtxDists[cntVtx2] > vtxDists[cntVtx2 + 1]) {

							// ����������
							float temp = vtxDists[cntVtx2];
							vtxDists[cntVtx2] = vtxDists[cntVtx2 + 1];
							vtxDists[cntVtx2 + 1] = temp;

							// ���_�ԍ�������
							int tempIndex = vtxIdxs[cntVtx2];
							vtxIdxs[cntVtx2] = vtxIdxs[cntVtx2 + 1];
							vtxIdxs[cntVtx2 + 1] = tempIndex;
						}
					}
				}

				for (int cntVtx = drawVtxIdxNum - 1; cntVtx >= 0; cntVtx--) {
					bool isOverwrite = false;
					for (UInt cntVtx2 = vtxIdxs[cntVtx] + 1; cntVtx2 < vtxNum[cntBone]; cntVtx2++) {
						if (vtxInfo[cntBone][vtxIdxs[cntVtx]].worldPos == vtxInfo[cntBone][cntVtx2].worldPos) {
							isOverwrite = true;
							break;
						}
					}
					if (isOverwrite)
						continue;

					RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, vtxInfo[cntBone][vtxIdxs[cntVtx]].worldPos, INITROT3D)
						->SetSize(0.25f, 0.25f)
						->SetCol(Color{ 255,0,0,255 })
						->SetLighting(false)
						->SetZTest(false)
						->SetBillboard(true);
					RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, CreateText("%d", vtxIdxs[cntVtx]), CText::ALIGNMENT::CENTER, 1, vtxInfo[cntBone][vtxIdxs[cntVtx]].worldPos, INITROT3D)
						->SetSize(Size2D(0.5f, 0.5f))
						->SetLighting(false)
						->SetZTest(false)
						->SetBillboard(true);
				}

				// ���_�ԍ����X�g�ƃJ�����܂ł̋������X�g���쐬
				CMemory::Release(&vtxIdxs);
				CMemory::Release(&vtxDists);
			}
			else if (drawVtxIdxNum == NONEDATA) {

				for (int cntVtx = drawVtxIdxNum - 1; cntVtx >= 0; cntVtx--) {
					bool isOverwrite = false;
					for (UInt cntVtx2 = cntVtx + 1; cntVtx2 < vtxNum[cntBone]; cntVtx2++) {
						if (vtxInfo[cntBone][cntVtx].worldPos == vtxInfo[cntBone][cntVtx2].worldPos) {
							isOverwrite = true;
							break;
						}
					}
					if (isOverwrite)
						continue;

					RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, vtxInfo[cntBone][cntVtx].worldPos, INITROT3D)
						->SetSize(0.25f, 0.25f)
						->SetCol(Color{ 255,0,0,(UShort)(255 * ((float)(drawVtxIdxNum - cntVtx) / drawVtxIdxNum)) })
						->SetLighting(false)
						->SetZTest(false)
						->SetBillboard(true);
					RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, CreateText("%d", cntVtx), CText::ALIGNMENT::CENTER, 1, vtxInfo[cntBone][cntVtx].worldPos, INITROT3D)
						->SetSize(Size2D(0.5f, 0.5f))
						->SetLighting(false)
						->SetZTest(false)
						->SetBillboard(true)
						->SetCol(Color{ 255,255,255,(UShort)(255 * ((float)(drawVtxIdxNum - cntVtx) / drawVtxIdxNum)) });
				}
			}
		}
	}

	// �t�F�C�X�̕`��
	for (int cntFace = 0; cntFace < setUp.m_faceDataNum; cntFace++) {
		const CSetUp3D::FaceVtxData& vtx0 = setUp.m_faceDatas[cntFace].vtxs[0];
		const CSetUp3D::FaceVtxData& vtx1 = setUp.m_faceDatas[cntFace].vtxs[1];
		const CSetUp3D::FaceVtxData& vtx2 = setUp.m_faceDatas[cntFace].vtxs[2];
		const CSetUp3D::FaceVtxData& vtx3 = setUp.m_faceDatas[cntFace].vtxs[3];

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
				vtx3.texPos);
	}

	// ���_�������
	CMemory::ReleaseDouble(&vtxInfo, setUp.m_boneDataNum);
	CMemory::Release(&vtxNum);
}

//========================================
// �{�[���̃��[���h�}�g���b�N�X�𒲂ׂ�
//========================================
Matrix CDoll3D::FindBoneWorldMtx(const short& idx, CBoneState*& boneState, CSetUp3D::BoneData*& boneData, const Matrix& selfMtx) {

	Matrix  worldMtx    = INITMATRIX;
	Pos3D   resultPos   = boneState[idx].GetAddPos() + boneState[idx].GetAnimPos() + boneData[idx].relativePos;
	Rot3D   resultRot   = boneState[idx].GetAddRot() + boneState[idx].GetAnimRot() + boneData[idx].relativeRot;
	Scale3D resultScale;
	resultScale.x = boneState[idx].GetAddScale().x * boneState[idx].GetAnimScale().x;
	resultScale.y = boneState[idx].GetAddScale().y * boneState[idx].GetAnimScale().y;
	resultScale.z = boneState[idx].GetAddScale().z * boneState[idx].GetAnimScale().z;
	Matrix  parentMtx   = INITMATRIX;

	// �Ǐ]����
	if (boneData[idx].follow != NULL) {
		const Pos3D   followPos   = boneState[boneData[idx].follow->followIdx].GetAddPos()   + boneState[boneData[idx].follow->followIdx].GetAnimPos();
		const Rot3D   followRot   = boneState[boneData[idx].follow->followIdx].GetAddRot()   + boneState[boneData[idx].follow->followIdx].GetAnimRot();
		const Scale3D followScale = boneState[boneData[idx].follow->followIdx].GetAddScale() + boneState[boneData[idx].follow->followIdx].GetAnimScale();
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

	{// �h���Ԃł��鎞�A�h��̉��Z�ʒu�����Z
		CBoneState::SwayingState*& swayingState = boneState[idx].GetSwayingState();
		if (swayingState != NULL) {
			resultPos += swayingState->addPos;
		}
	}

	// �e�{�[�����Ȃ����A
	if (boneState[idx].GetParentBoneState() == NULL) {

		// �h�[�����̂̊g��{������Z
		resultScale.x *= m_scale.x;
		resultScale.y *= m_scale.y;
		resultScale.z *= m_scale.z;

		// �e�}�g���b�N�X���h�[�����̂̃}�g���b�N�X�ɐݒ�
		parentMtx = selfMtx;
	}
	else 
	{// �e�{�[�������鎞�A
		// �e�}�g���b�N�X��e�{�[���̃}�g���b�N�X�ɐݒ�
		parentMtx = boneState[idx].GetParentBoneState()->GetWorldMtx();
	}

	// ���[���h�}�g���b�N�X���Z�o
	worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosRotScaleToMtx(resultPos, resultRot, resultScale), parentMtx);

	// ���[���h�}�g���b�N�X�ݒ�
	boneState[idx].SetWorldMtx(worldMtx);

	return worldMtx;
}

//========================================
// ���[�V�����̏�������
//========================================
void CDoll3D::PrepareMotion(void) {

	const CMotion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);	// ���[�V�����f�[�^
	const CSetUp3D::CData&  setUpData  = RNLib::SetUp3D().GetData(m_setUpIdx);			// �Z�b�g�A�b�v�f�[�^

	if (&setUpData == NULL)
		return;

	if (&motionData == NULL)
		return;

	// �{�[�����ɏ�������
	for (int cntBone = 0; cntBone < setUpData.m_boneDataNum; cntBone++) {

		// �{�[�����̃��[�V�����f�[�^�|�C���^��n��
		for (int cntMotionBone = 0; cntMotionBone < motionData.boneNum; cntMotionBone++) {
			if (setUpData.m_boneDatas[cntBone].idx == cntMotionBone){
				m_boneStates[cntBone].SetMotionData(&motionData.boneMotionDatas[cntMotionBone]);
				break;
			}
		}

		// ��������
		m_boneStates[cntBone].PrepareMotion(motionData.boneMotionDatas[cntBone]);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�{�[����ԃN���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//========================================
CDoll3D::CBoneState::~CBoneState() {

	// �h���Ԃ̃��������
	CMemory::Release(&m_swayingState);

	// �A�j���̏������
	CMemory::Release(&m_animeStateSum.move);
	CMemory::Release(&m_animeStateSum.spin);
	CMemory::Release(&m_animeStateSum.scaling);
}

//========================================
// �X�V����
//========================================
void CDoll3D::CBoneState::Update(const short& motionCounter, const CSetUp3D::BoneData& boneData) {

	// �h����
	if (m_swayingState != NULL) {
		
		if (--m_swayingState->counter <= 0) {
			m_swayingState->counterMax   = boneData.swaying->timeMin + rand() % boneData.swaying->timeAdd;
			m_swayingState->counter      = m_swayingState->counterMax;
			m_swayingState->oldAddPos    = m_swayingState->addPos;
			m_swayingState->targetAddPos = CGeometry::GetRandomVec() * (boneData.swaying->distMin + fRand() * boneData.swaying->distAdd);
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_swayingState->counter, m_swayingState->counterMax);
		m_swayingState->addPos = (m_swayingState->oldAddPos * rate) + (m_swayingState->targetAddPos * (1.0f - rate));
	}

	// �����t���O���U�ɂ��Ă���
	m_animeStateSum.isStep = false;

	//----------------------------------------
	// �R�}���h�ǂݎ��
	//----------------------------------------
	if (m_motionData != NULL){
		for (int cntCommand = 0; cntCommand < m_motionData->commandDataNum; cntCommand++) {

			// �J�E���^�[�����s���Ԃƈ�v���Ă��Ȃ����A�܂�Ԃ�
			if (motionCounter != m_motionData->commandDatas[cntCommand].time)
				continue;

			CMotion3D::CommandData& commandData(m_motionData->commandDatas[cntCommand]);

			// �R�}���h�ɉ���������
			switch (commandData.command) {
				// [[[ �ړ� ]]]
			case CMotion3D::COMMAND::MOVE: {
				const Pos3D       targetPos = Pos3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
				const int         moveTime  = (int)commandData.datas[3];
				const CEase::TYPE ease      = (CEase::TYPE)((int)commandData.datas[4]);

				if (moveTime == 0)
				{// �ړ��ɂ����鎞�Ԃ�0�̎��A
					// �ʒu�𒼐ڑ��
					m_animPos = targetPos;
				}
				else
				{// �ړ��ɂ����鎞�Ԃ�0�łȂ����A
					// �ړ��A�j����ԃ��������m��
					CMemory::Alloc(&m_animeStateSum.move);

					// �ړ��A�j����Ԃ�ݒ�
					m_animeStateSum.move->posEase   = ease;
					m_animeStateSum.move->oldPos    = m_animPos;
					m_animeStateSum.move->targetPos = targetPos;
					m_animeStateSum.move->time      = moveTime;
				}
			}break;
				// [[[ ��] ]]]
			case CMotion3D::COMMAND::SPIN: {
				const Rot3D       targetRot = Rot3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
				const int         spinTime  = (int)commandData.datas[3];
				const CEase::TYPE ease      = (CEase::TYPE)((int)commandData.datas[4]);

				if (spinTime == 0)
				{// ��]�ɂ����鎞�Ԃ�0�̎��A
					// �����𒼐ڑ��
					m_animRot = targetRot;
				}
				else
				{// ��]�ɂ����鎞�Ԃ�0�łȂ����A
					// ��]�A�j����ԃ��������m��
					CMemory::Alloc(&m_animeStateSum.spin);

					// ��]�A�j����Ԃ�ݒ�
					m_animeStateSum.spin->rotEase   = ease;
					m_animeStateSum.spin->oldRot    = m_animRot;
					m_animeStateSum.spin->targetRot = targetRot;
					m_animeStateSum.spin->time      = spinTime;
				}
			}break;
				// [[[ �g�k ]]]
			case CMotion3D::COMMAND::SCALING: {
				const Scale3D     targetScale = Scale3D(commandData.datas[0], commandData.datas[1], commandData.datas[2]);
				const int         scalingTime = (int)commandData.datas[3];
				const CEase::TYPE ease        = (CEase::TYPE)((int)commandData.datas[4]);

				if (scalingTime == 0)
				{// �g�k�ɂ����鎞�Ԃ�0�̎��A
					// �g��{���𒼐ڑ��
					m_animScale = targetScale;
				}
				else
				{// �g�k�ɂ����鎞�Ԃ�0�łȂ����A
					// �g�k�A�j����ԃ��������m��
					CMemory::Alloc(&m_animeStateSum.scaling);

					// �g�k�A�j����Ԃ�ݒ�
					m_animeStateSum.scaling->scaleEase   = ease;
					m_animeStateSum.scaling->oldScale    = m_animScale;
					m_animeStateSum.scaling->targetScale = targetScale;
					m_animeStateSum.scaling->time        = scalingTime;
				}
			}break;
				// [[[ ���� ]]]
			case CMotion3D::COMMAND::STEP: {

				// �����t���O��^�ɂ��Ă���
				m_animeStateSum.isStep = true;
			}break;
			}
		}
	}

	//----------------------------------------
	// �A�j����ԍX�V����
	//----------------------------------------
	// [[[ �ړ� ]]]
	if (m_animeStateSum.move != NULL) {

		// �����𒲂ׂ�
		const float rate = CEase::Easing(m_animeStateSum.move->posEase, m_animeStateSum.move->counter, m_animeStateSum.move->time);

		// �ʒu���X�V
		m_animPos = (m_animeStateSum.move->oldPos * (1.0f - rate)) + (m_animeStateSum.move->targetPos * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.move->counter >= m_animeStateSum.move->time) {
			CMemory::Release(&m_animeStateSum.move);
		}
	}

	// [[[ ��] ]]]
	if (m_animeStateSum.spin != NULL) {

		// �����𒲂ׂ�
		const float rate = CEase::Easing(m_animeStateSum.spin->rotEase, m_animeStateSum.spin->counter, m_animeStateSum.spin->time);

		// �������X�V
		m_animRot = (m_animeStateSum.spin->oldRot * (1.0f - rate)) + (m_animeStateSum.spin->targetRot * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.spin->counter >= m_animeStateSum.spin->time) {
			CMemory::Release(&m_animeStateSum.spin);
		}
	}

	// [[[ �g�k ]]]
	if (m_animeStateSum.scaling != NULL) {

		// �����𒲂ׂ�
		const float rate = CEase::Easing(m_animeStateSum.scaling->scaleEase, m_animeStateSum.scaling->counter, m_animeStateSum.scaling->time);

		// �g��{�����X�V
		m_animScale = (m_animeStateSum.scaling->oldScale * (1.0f - rate)) + (m_animeStateSum.scaling->targetScale * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.scaling->counter >= m_animeStateSum.scaling->time) {
			CMemory::Release(&m_animeStateSum.scaling);
		}
	}
}

//========================================
// ���[�V������������
//========================================
void CDoll3D::CBoneState::PrepareMotion(const CMotion3D::BoneMotionData& boneMotionData) {

	// ���������
	CMemory::Release(&m_animeStateSum.move);
	CMemory::Release(&m_animeStateSum.spin);
	CMemory::Release(&m_animeStateSum.scaling);

	// �ړ����Ȃ����[�V�����̎��A�ʒu�ύX���Ă���̂ł���΁A
	if (!boneMotionData.isMove && m_animPos != INITPOS3D) {
		
		// �ړ����̃��������m�ۂ��A
		CMemory::Alloc(&m_animeStateSum.move);

		// �����ʒu�Ɉړ�������
		m_animeStateSum.move->oldPos    = m_animPos;
		m_animeStateSum.move->targetPos = INITPOS3D;
		m_animeStateSum.move->time      = PAUSE_RESET_TIME;
		m_animeStateSum.move->posEase   = CEase::TYPE::LINEAR;
	}

	// ��]���Ȃ����[�V�����̎��A�����ύX���Ă���̂ł���΁A
	if (!boneMotionData.isSpin && m_animRot != INITROT3D) {
		
		// ��]���̃��������m�ۂ��A
		CMemory::Alloc(&m_animeStateSum.spin);

		// ���������ɉ�]������
		m_animeStateSum.spin->oldRot    = m_animRot;
		m_animeStateSum.spin->targetRot = INITROT3D;
		m_animeStateSum.spin->time      = PAUSE_RESET_TIME;
		m_animeStateSum.spin->rotEase   = CEase::TYPE::LINEAR;
	}

	// �g�k���Ȃ����[�V�����̎��A�g��{���ύX���Ă���̂ł���΁A
	if (!boneMotionData.isScale && m_animScale != INITSCALE3D) {
		
		// �g�k���̃��������m�ۂ��A
		CMemory::Alloc(&m_animeStateSum.scaling);

		// �����g��{���Ɋg�k������
		m_animeStateSum.scaling->oldScale    = m_animScale;
		m_animeStateSum.scaling->targetScale = INITSCALE3D;
		m_animeStateSum.scaling->time        = PAUSE_RESET_TIME;
		m_animeStateSum.scaling->scaleEase   = CEase::TYPE::LINEAR;
	}
}