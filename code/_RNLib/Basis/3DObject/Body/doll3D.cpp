//========================================
// 
// �h�[��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include "../../../RNmode.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�h�[��3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CDoll3D::CDoll3D(const UShort& priority, const short& setUpIdx) {

	// ���X�g�ɒǉ�
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
// �f�X�g���N�^
//========================================
CDoll3D::~CDoll3D() {

	// ���X�g����폜
	RNLib::Doll3DMgr().SubList(this);

	// �{�[����Ԃ̃��������
	RNLib::Memory().Release(&m_boneStates);
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
		_RNC_SetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

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
		RNLib::Memory().Release(&m_boneStates);
		return;
	}

	// �Z�b�g�A�b�v�f�[�^�擾
	const _RNC_SetUp3D::CData& setUp = RNLib::SetUp3D().GetData(m_setUpIdx);

	// �{�[����ԃ������m��
	if (&setUp == NULL) {
		RNLib::Memory().Release(&m_boneStates);
	}
	else {
		// ���i����0���z���Ă��� > �������m��
		// ���i����0�ȉ�         > ���������
		setUp.m_boneDataNum > 0 ?
			RNLib::Memory().Alloc(&m_boneStates, setUp.m_boneDataNum) :
			RNLib::Memory().Release(&m_boneStates);

		for (int cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

			// �h���񂪑��݂��鎞�A�h���Ԃ𐶐�
			if (setUp.m_boneDatas[cntBone].swaying != NULL) {
				m_boneStates[cntBone].CreateSwayingState();
			}
			
			// �e�ԍ����݂�̎��A�����ԍ��̃{�[�����������A��������e�̃|�C���^��ݒ肷��
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

	// ���[�V������������
	PrepareMotion();
}

//========================================
// ���[�V�����ݒ菈��
//========================================
void CDoll3D::SetMotion(const short& motionIdx) {

	// �ԍ�����
	m_motionInfo.idx    =
	m_motionInfo.oldIdx = motionIdx;

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
//==========| [����J]�h�[��3D�N���X
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
	const _RNC_Motion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);
	if (&motionData == NULL)
		return;

	// ���[�V�����J�E���^�[��i�߂�
	if (++m_motionInfo.counter >= motionData.loopTime)
		m_motionInfo.counter = motionData.isLoop ? 0 : motionData.loopTime;
}

//========================================
// �{�[���̍X�V����
//========================================
void CDoll3D::UpdateBone(_RNC_SetUp3D::CData& setUp) {

	// �Z�b�g�A�b�v�f�[�^�����݂��Ȃ����A�������I������
	if (&setUp == NULL)
		return;

	// ���i��0�ȉ��̎��A�������I������
	if (setUp.m_boneDataNum <= 0)
		return;

	// �{�̃}�g���b�N�X
	Matrix selfMtx = RNLib::Matrix().ConvPosRotScaleToMtx(m_pos, m_rot, m_scale);

	// ���_���
	_RNC_Model::Vertex3DInfo** vtxInfo = NULL;
	ULong* vtxNum = NULL;
	RNLib::Memory().Alloc(&vtxInfo, setUp.m_boneDataNum);
	RNLib::Memory().Alloc(&vtxNum, setUp.m_boneDataNum);

	// ���f���̕`��
	for (short cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {

		CBoneState& boneState = m_boneStates[cntBone];
		Pos3D       resultPos = INITPOS3D;
		Rot3D       resultRot = INITROT3D;

		// �{�[����Ԃ̍X�V����
		boneState.Update(m_motionInfo.counter, setUp.m_boneDatas[cntBone]);

		// ���[���h�}�g���b�N�X�𒲂ׂ�
		const Matrix worldMtx = FindBoneWorldMtx(cntBone, m_boneStates, setUp.m_boneDatas, selfMtx);

		// ���_�����擾
		vtxNum[cntBone] = RNLib::Model().GetData(setUp.m_boneDatas[cntBone].modelIdx).m_vtxNum;
		vtxInfo[cntBone] = NULL;
		RNLib::Model().StoreVtxInfo(worldMtx, setUp.m_boneDatas[cntBone].modelIdx, &vtxInfo[cntBone]);

		// �\���t���O���^�̎��A
		if (m_isShow) {

			// ���f���̐ݒu����
			RNLib::Model().Put(m_priority, setUp.m_boneDatas[cntBone].modelIdx, worldMtx)
				->SetCol(m_col)
				->SetBrightnessOfEmissive(m_brightnessOfEmission)
				->SetClippingCamera(m_clippingID);

			// ���_�ԍ��̕`��
			if (RNLib::Doll3DMgr().GetEditDoll() == this &&
				RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx() &&
				RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx() == cntBone &&
				RNLib::Doll3DMgr().GetEditCamera() != NULL) {
				DrawModelVtxIdx(vtxInfo[cntBone], vtxNum[cntBone]);
			}
		}
	}

	// �\���t���O���^�̎��A
	if (m_isShow) {

		// �t�F�C�X��`��
		DrawFace(setUp, vtxInfo, vtxNum);
	}

	// ���_�������
	RNLib::Memory().ReleaseDouble(&vtxInfo, setUp.m_boneDataNum);
	RNLib::Memory().Release(&vtxNum);
}

//========================================
// ���f���̒��_�ԍ���`��
//========================================
void CDoll3D::DrawModelVtxIdx(_RNC_Model::Vertex3DInfo*& vtxInfo, ULong& vtxNum) {

	// ���_�ԍ��`�搔
	ULong drawVtxIdxNum = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum();
	if (drawVtxIdxNum > vtxNum)
		drawVtxIdxNum = vtxNum;

	// 0���z���Ă���
	if (drawVtxIdxNum > 0) {

		// ���_�ԍ����X�g�ƃJ�����܂ł̋������X�g���쐬
		ULong* vtxIdxs = NULL;
		float* vtxDists = NULL;
		RNLib::Memory().Alloc(&vtxIdxs, vtxNum);
		RNLib::Memory().Alloc(&vtxDists, vtxNum);
		const Pos3D& cameraPos = RNLib::Doll3DMgr().GetEditCamera()->GetPosV();
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
			vtxIdxs[cntVtx] = cntVtx;
			vtxDists[cntVtx] = RNLib::Geometry().FindDistance(cameraPos, vtxInfo[cntVtx].pos);
		}

		// �o�u���\�[�g���g�p���Ē��_�ԍ����X�g���\�[�g
		for (ULong cntVtx = 0; cntVtx < vtxNum - 1; ++cntVtx) {
			for (ULong cntVtx2 = 0; cntVtx2 < vtxNum - cntVtx - 1; ++cntVtx2) {
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

		// ���_�ԍ����X�g�ƃJ�����܂ł̋������X�g���쐬
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
// �t�F�C�X��`��
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
// �{�[���̃��[���h�}�g���b�N�X�𒲂ׂ�
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

	// �Ǐ]����
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

	{// �h���Ԃł��鎞�A�h��̉��Z�ʒu�����Z
		CBoneState::SwayingState*& swayingState = boneState[idx].GetSwayingState();
		if (swayingState != NULL)
			resultPos += swayingState->addPos;
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
	worldMtx = RNLib::Matrix().MultiplyMtx(RNLib::Matrix().ConvPosRotScaleToMtx(resultPos, resultRot, resultScale), parentMtx);

	// ���[���h�}�g���b�N�X�ݒ�
	boneState[idx].SetWorldMtx(worldMtx);

	return worldMtx;
}

//========================================
// ���[�V�����̏�������
//========================================
void CDoll3D::PrepareMotion(void) {

	// �Z�b�g�A�b�v�f�[�^�����݂��Ȃ����A�I��
	const _RNC_SetUp3D::CData& setUpData = RNLib::SetUp3D().GetData(m_setUpIdx);
	if (&setUpData == NULL)
		return;

	// �Z�b�g�A�b�v�f�[�^�����݂��Ȃ����A�I��
	const _RNC_Motion3D::CData& motionData = RNLib::Motion3D().GetData(m_motionInfo.idx);
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
//==========| [���J]�{�[����ԃN���X
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
	RNLib::Memory().Release(&m_swayingState);

	// �A�j���̏������
	RNLib::Memory().Release(&m_animeStateSum.move);
	RNLib::Memory().Release(&m_animeStateSum.spin);
	RNLib::Memory().Release(&m_animeStateSum.scaling);
}

//========================================
// �X�V����
//========================================
void CDoll3D::CBoneState::Update(const short& motionCounter, const _RNC_SetUp3D::BoneData& boneData) {

	// �h����
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
	// �R�}���h�ǂݎ��
	//----------------------------------------
	if (m_motionData != NULL){
		for (int cntCommand = 0; cntCommand < m_motionData->commandDataNum; cntCommand++) {

			// �J�E���^�[�����s���Ԃƈ�v���Ă��Ȃ����A�܂�Ԃ�
			if (motionCounter != m_motionData->commandDatas[cntCommand].time)
				continue;

			// �R�}���h�ɉ���������
			switch (m_motionData->commandDatas[cntCommand].command) {
				// [[[ �ړ� ]]]
			case _RNC_Motion3D::COMMAND::MOVE: {
				_RNC_Motion3D::CommandData_Move& commandData = *(_RNC_Motion3D::CommandData_Move*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// �ړ��ɂ����鎞�Ԃ�0�̎��A
					// �ʒu�𒼐ڑ��
					m_animPos = commandData.targetPos;
				}
				else
				{// �ړ��ɂ����鎞�Ԃ�0�łȂ����A
					// �ړ��A�j����ԃ��������m��
					RNLib::Memory().Alloc(&m_animeStateSum.move);

					// �ړ��A�j����Ԃ�ݒ�
					m_animeStateSum.move->posEase   = commandData.easeType;
					m_animeStateSum.move->oldPos    = m_animPos;
					m_animeStateSum.move->targetPos = commandData.targetPos;
					m_animeStateSum.move->time      = commandData.time;
				}
			}break;
				// [[[ ��] ]]]
			case _RNC_Motion3D::COMMAND::SPIN: {
				_RNC_Motion3D::CommandData_Spin& commandData = *(_RNC_Motion3D::CommandData_Spin*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// ��]�ɂ����鎞�Ԃ�0�̎��A
					// �����𒼐ڑ��
					m_animRot = commandData.targetRot;
				}
				else
				{// ��]�ɂ����鎞�Ԃ�0�łȂ����A
					// ��]�A�j����ԃ��������m��
					RNLib::Memory().Alloc(&m_animeStateSum.spin);

					// ��]�A�j����Ԃ�ݒ�
					m_animeStateSum.spin->rotEase   = commandData.easeType;
					m_animeStateSum.spin->oldRot    = m_animRot;
					m_animeStateSum.spin->targetRot = commandData.targetRot;
					m_animeStateSum.spin->time      = commandData.time;
				}
			}break;
				// [[[ �g�k ]]]
			case _RNC_Motion3D::COMMAND::SCALING: {
				_RNC_Motion3D::CommandData_Scaling& commandData = *(_RNC_Motion3D::CommandData_Scaling*)m_motionData->commandDatas[cntCommand].data;

				if (commandData.time == 0)
				{// �g�k�ɂ����鎞�Ԃ�0�̎��A
					// �g��{���𒼐ڑ��
					m_animScale = commandData.targetScale;
				}
				else
				{// �g�k�ɂ����鎞�Ԃ�0�łȂ����A
					// �g�k�A�j����ԃ��������m��
					RNLib::Memory().Alloc(&m_animeStateSum.scaling);

					// �g�k�A�j����Ԃ�ݒ�
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
	// �A�j����ԍX�V����
	//----------------------------------------
	// [[[ �ړ� ]]]
	if (m_animeStateSum.move != NULL) {

		// �����𒲂ׂ�
		const float rate = RNLib::Ease().Easing(m_animeStateSum.move->posEase, m_animeStateSum.move->counter, m_animeStateSum.move->time);

		// �ʒu���X�V
		m_animPos = (m_animeStateSum.move->oldPos * (1.0f - rate)) + (m_animeStateSum.move->targetPos * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.move->counter >= m_animeStateSum.move->time)
			RNLib::Memory().Release(&m_animeStateSum.move);
	}

	// [[[ ��] ]]]
	if (m_animeStateSum.spin != NULL) {

		// �����𒲂ׂ�
		const float rate = RNLib::Ease().Easing(m_animeStateSum.spin->rotEase, m_animeStateSum.spin->counter, m_animeStateSum.spin->time);

		// �������X�V
		m_animRot = (m_animeStateSum.spin->oldRot * (1.0f - rate)) + (m_animeStateSum.spin->targetRot * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.spin->counter >= m_animeStateSum.spin->time)
			RNLib::Memory().Release(&m_animeStateSum.spin);
	}

	// [[[ �g�k ]]]
	if (m_animeStateSum.scaling != NULL) {

		// �����𒲂ׂ�
		const float rate = RNLib::Ease().Easing(m_animeStateSum.scaling->scaleEase, m_animeStateSum.scaling->counter, m_animeStateSum.scaling->time);

		// �g��{�����X�V
		m_animScale = (m_animeStateSum.scaling->oldScale * (1.0f - rate)) + (m_animeStateSum.scaling->targetScale * rate);

		// �J�E���^�[���B���A���������
		if (++m_animeStateSum.scaling->counter >= m_animeStateSum.scaling->time)
			RNLib::Memory().Release(&m_animeStateSum.scaling);
	}
}

//========================================
// ���[�V������������
//========================================
void CDoll3D::CBoneState::PrepareMotion(const _RNC_Motion3D::BoneMotionData& boneMotionData) {

	// ���������
	RNLib::Memory().Release(&m_animeStateSum.move);
	RNLib::Memory().Release(&m_animeStateSum.spin);
	RNLib::Memory().Release(&m_animeStateSum.scaling);

	// �ړ����Ȃ����[�V�����̎��A�ʒu�ύX���Ă���̂ł���΁A
	if (!boneMotionData.isMove && m_animPos != INITPOS3D) {
		
		// �ړ����̃��������m�ۂ��A
		RNLib::Memory().Alloc(&m_animeStateSum.move);

		// �����ʒu�Ɉړ�������
		m_animeStateSum.move->oldPos    = m_animPos;
		m_animeStateSum.move->targetPos = INITPOS3D;
		m_animeStateSum.move->time      = PAUSE_RESET_TIME;
		m_animeStateSum.move->posEase   = _RNC_Ease::TYPE::LINEAR;
	}

	// ��]���Ȃ����[�V�����̎��A�����ύX���Ă���̂ł���΁A
	if (!boneMotionData.isSpin && m_animRot != INITROT3D) {
		
		// ��]���̃��������m�ۂ��A
		RNLib::Memory().Alloc(&m_animeStateSum.spin);

		// ���������ɉ�]������
		m_animeStateSum.spin->oldRot    = m_animRot;
		m_animeStateSum.spin->targetRot = INITROT3D;
		m_animeStateSum.spin->time      = PAUSE_RESET_TIME;
		m_animeStateSum.spin->rotEase   = _RNC_Ease::TYPE::LINEAR;
	}

	// �g�k���Ȃ����[�V�����̎��A�g��{���ύX���Ă���̂ł���΁A
	if (!boneMotionData.isScale && m_animScale != INITSCALE3D) {
		
		// �g�k���̃��������m�ۂ��A
		RNLib::Memory().Alloc(&m_animeStateSum.scaling);

		// �����g��{���Ɋg�k������
		m_animeStateSum.scaling->oldScale    = m_animScale;
		m_animeStateSum.scaling->targetScale = INITSCALE3D;
		m_animeStateSum.scaling->time        = PAUSE_RESET_TIME;
		m_animeStateSum.scaling->scaleEase   = _RNC_Ease::TYPE::LINEAR;
	}
}