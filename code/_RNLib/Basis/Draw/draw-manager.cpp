//========================================
// 
// �`�揈��
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�`��}�l�[�W���[�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
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
// �R���X�g���N�^
//========================================
CDrawMgr::CDrawMgr() {

	m_reAllocCount = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CDrawMgr::~CDrawMgr() {

}

//========================================
// ����������
//========================================
void CDrawMgr::Init(void) {

	// �o�^���̏����������m��
	ms_resistInfoSum.InitAlloc();
	ms_resistInfoSumScreen.InitAlloc();

	// ���_�o�b�t�@�̏�������
	CPolygon2D::CDrawInfo::InitCreateVertexBuffer();
	CPolygon3D::CDrawInfo::InitCreateVertexBuffer();
}

//========================================
// �I������
//========================================
void CDrawMgr::Uninit(void) {

	// ���C�����[�v�X���b�h���a�藣��
	ms_mainLoopTh.detach();

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void CDrawMgr::Update(void) {

}

//========================================
// �������
//========================================
void CDrawMgr::Release(void) {

	// �`��������
	ms_drawInfoSum.Release();
	ms_drawInfoSumOvr.Release();
	ms_drawInfoSumScreen.Release();
	ms_drawInfoSumScreenOvr.Release();
	CMemory::Release(&ms_drawInfoSum.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumOvr.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumScreen.ms_drawInfos);
	CMemory::Release(&ms_drawInfoSumScreenOvr.ms_drawInfos);

	// �J�����̕`�揇�������
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++)
		CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList);
	CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos);
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum; cnt++)
		CMemory::Release(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt].orderList);
	CMemory::Release(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos);

	// �o�^�������
	ms_resistInfoSum.Release();
	ms_resistInfoSumScreen.Release();

	// ���_�o�b�t�@���������
	CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
	CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
}

//========================================
// �`��J�n����
//========================================
bool CDrawMgr::StartDraw(void) {

	if (ms_processState == PROCESS_STATE::REGIST_ACCEPT)
	{// [ �o�^��t ]�̎��A
		{// <<< �J�����̕`�揇�����쐬 >>>
			// �J�����̐����擾���A���̕��`�揇�����m��
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum = RNLib::CameraMgr().GetNum() + 1;	// ��+1�̓X�N���[���̕�
			CMemory::Alloc(&ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum);

			// ����ݒ肵�Ă���
			CCamera* camera = NULL;
			int cameraCount = 0;
			while (RNLib::CameraMgr().ListLoop((CObject**)&camera)) {
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].ID        = camera->GetID();
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].orderList = NULL;
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].posV      = camera->GetPosV();
				ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].nor       = camera->GetNor();
				cameraCount++;
			}
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].ID        = NONEDATA;	// ���X�N���[���Ƃ��Ĉ���
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].orderList = NULL;
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].posV      = INITPOS3D;
			ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cameraCount].nor       = Vector3D(0.0f, 0.0f, 1.0f);
		}

		// [ �o�^���K�p�҂� ]�ɂ���
		ms_processState = PROCESS_STATE::REGIST_INFO_APPLY_WAIT;
	}
	else if ((ms_processState == PROCESS_STATE::DRAW_INFO_SWAP_WAIT && !RNSystem::GetSpace3DStop()) || RNSystem::GetSceneSwap())
	{// [ �`�����ւ��҂� ]�̎��A
		// �`������㏑������
		ms_drawInfoSum.Overwrite(&ms_drawInfoSumOvr);
		ms_drawInfoSumScreen.Overwrite(&ms_drawInfoSumScreenOvr);

		{// �J�����̕`�揇�����㏑������
			// �Â��������
			for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++)
				CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList);
			CMemory::Release(&ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos);

			// �V�������ŏ㏑��
			ms_cameraDrawOrderInfoSum = ms_cameraDrawOrderInfoSumOvr;
			ms_cameraDrawOrderInfoSumOvr = {};
		}

		{// �s�v�������j���ׁ̈A�Ċm��
			ms_resistInfoSum.ReAlloc();
			ms_resistInfoSumScreen.ReAlloc();

			// �|���S��2D
			if (CPolygon2D::CDrawInfo::m_allocPower > POLYGON2D_ALLOC_BASE_POWER) {

				for (int cntAlloc = POLYGON2D_ALLOC_BASE_POWER; cntAlloc < CPolygon2D::CDrawInfo::m_allocPower; cntAlloc++) {
					const UShort allocLine = pow(2, cntAlloc);

					if (CPolygon2D::CDrawInfo::m_idxCount < allocLine)
					{// �m�ۃ��C�������������Ȃ����A
						// ���̊m�ۃ��C�����m�ۂ�����
						CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
						CPolygon2D::CDrawInfo::CreateVertexBuffer(allocLine);

						// �m�ۂ�����/�ׂ��搔��ۑ�
						CPolygon2D::CDrawInfo::m_allocNum   = allocLine;
						CPolygon2D::CDrawInfo::m_allocPower = cntAlloc;

						break;
					}
				}
			}

			// �|���S��3D
			if (CPolygon3D::CDrawInfo::m_allocPower > POLYGON3D_ALLOC_BASE_POWER) {

				for (int cntAlloc = POLYGON3D_ALLOC_BASE_POWER; cntAlloc < CPolygon3D::CDrawInfo::m_allocPower; cntAlloc++) {
					const UShort allocLine = pow(2, cntAlloc);

					if (CPolygon3D::CDrawInfo::m_idxCount < allocLine)
					{// �m�ۃ��C�������������Ȃ����A
						// ���̊m�ۃ��C�����m�ۂ�����
						CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
						CPolygon3D::CDrawInfo::CreateVertexBuffer(allocLine);

						// �m�ۂ�����/�ׂ��搔��ۑ�
						CPolygon3D::CDrawInfo::m_allocNum   = allocLine;
						CPolygon3D::CDrawInfo::m_allocPower = cntAlloc;

						break;
					}
				}
			}
		}

		// �ԍ��J�E���g������
		CPolygon2D::CDrawInfo::m_idxCount = 0;
		CPolygon3D::CDrawInfo::m_idxCount = 0;

		// ���_������
		AssignVertexInfo();

		// [ �o�^��t ]�ɂ���
		ms_processState = PROCESS_STATE::REGIST_ACCEPT;

		return true;
	}

	return false;
}

//========================================
// �`�揈��
//========================================
void CDrawMgr::Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	// �J������ID�ɑΉ�����`�揇���X�g���擾
	UShort* orderList = NULL;
	for (int cnt = 0; cnt < ms_cameraDrawOrderInfoSum.cameraDrawOrderInfoNum; cnt++) {
		if (cameraID == ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].ID) {
			orderList = ms_cameraDrawOrderInfoSum.cameraDrawOrderInfos[cnt].orderList;
			break;
		}
	}
	// ���擾�ł��Ȃ���ΏI��
	if (orderList == NULL)
		return;

	// �r���[�}�g���b�N�X���擾
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// �`�悵�Ă���
	if (isOnScreen) {
		for (int cntDrawInfo = 0; cntDrawInfo < ms_drawInfoSumScreen.ms_drawInfoNum; cntDrawInfo++) {
			if (ms_drawInfoSumScreen.ms_drawInfos[orderList[cntDrawInfo]] != NULL) {

				// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
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
				
				// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
				if (ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != NONEDATA || isCameraClipping)
					if (ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->m_clippingID != cameraID)
						continue;

				ms_drawInfoSum.ms_drawInfos[orderList[cntDrawInfo]]->Draw(device, viewMtx);
			}
		}
	}
}

//========================================
// �ݒu����(�|���S��2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::PutPolygon2D(const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen) {

	// �ԍ��J�E���g���ő吔�ɒB�������A���_�o�b�t�@���Đ�������
	if (CPolygon2D::CDrawInfo::m_idxCount == CPolygon2D::CDrawInfo::m_allocNum) {
		CPolygon2D::CDrawInfo::m_allocPower++;
		CPolygon2D::CDrawInfo::m_allocNum = pow(2, CPolygon2D::CDrawInfo::m_allocPower);
		CPolygon2D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon2D::CDrawInfo::CreateVertexBuffer(CPolygon2D::CDrawInfo::m_allocNum);
	}

	// �o�^���
	CPolygon2D::CRegistInfo* registInfo = NULL;

	// �X�N���[����t���O�ɉ����ēo�^
	if (isOnScreen) registInfo = RegistPolygon2D(ms_resistInfoSumScreen);
	else            registInfo = RegistPolygon2D(ms_resistInfoSum);

	// ������
	if (registInfo != NULL) {
		registInfo->SetIdx(CPolygon2D::CDrawInfo::m_idxCount++);
		registInfo->SetPos(pos);
		registInfo->SetAngle(angle);
	}

	return registInfo;
}

//========================================
// �ݒu����(�|���S��3D)
//========================================
CPolygon3D::CRegistInfo* CDrawMgr::PutPolygon3D(const Matrix& mtx, const bool& isOnScreen) {
	
	// �ԍ��J�E���g���ő吔�ɒB�������A���_�o�b�t�@���Đ�������
	if (CPolygon3D::CDrawInfo::m_idxCount == CPolygon3D::CDrawInfo::m_allocNum) {
		CPolygon3D::CDrawInfo::m_allocPower++;
		CPolygon3D::CDrawInfo::m_allocNum = pow(2, CPolygon3D::CDrawInfo::m_allocPower);
		CPolygon3D::CDrawInfo::ReleaseVertexBuffer();
		CPolygon3D::CDrawInfo::CreateVertexBuffer(CPolygon3D::CDrawInfo::m_allocNum);
	}

	// �o�^���
	CPolygon3D::CRegistInfo* registInfo = NULL;

	// �X�N���[����t���O�ɉ����ēo�^
	if (isOnScreen) registInfo = RegistPolygon3D(ms_resistInfoSumScreen);
	else            registInfo = RegistPolygon3D(ms_resistInfoSum);

	// ������
	if (registInfo != NULL) {
		registInfo->SetIdx(CPolygon3D::CDrawInfo::m_idxCount++);
		registInfo->SetMtx(mtx);
	}

	return registInfo;
}

//========================================
// �ݒu����(�e�L�X�g2D)
//========================================
CText2D::CRegistInfo* CDrawMgr::PutText2D(const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen) {

	// �o�^���
	CText2D::CRegistInfo* registInfo = NULL;

	// �X�N���[����t���O�ɉ����ēo�^
	if (isOnScreen) registInfo = RegistText2D(ms_resistInfoSumScreen);
	else            registInfo = RegistText2D(ms_resistInfoSum);

	// ������
	if (registInfo != NULL) {
		registInfo->SetPos(pos);
		registInfo->SetAngle(angle);
	}

	return registInfo;
}

//========================================
// �ݒu����(�e�L�X�g3D)
//========================================
CText3D::CRegistInfo* CDrawMgr::PutText3D(const Matrix& mtx, const bool& isOnScreen) {

	// �o�^���
	CText3D::CRegistInfo* registInfo = NULL;

	// �X�N���[����t���O�ɉ����ēo�^
	if (isOnScreen) registInfo = RegistText3D(ms_resistInfoSumScreen);
	else            registInfo = RegistText3D(ms_resistInfoSum);

	// ������
	if (registInfo != NULL) {
		registInfo->SetMtx(mtx);
	}

	return registInfo;
}

//========================================
// �ݒu����(���f��)
//========================================
CModel::CRegistInfo* CDrawMgr::PutModel(const Matrix& mtx, const bool& isOnScreen) {

	// �o�^��t���łȂ����A�I��
	if (ms_processState != PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	// �o�^���
	CModel::CRegistInfo* registInfo = NULL;

	// �X�N���[����t���O�ɉ����ēo�^
	if (isOnScreen) registInfo = RegistModel(ms_resistInfoSumScreen);
	else            registInfo = RegistModel(ms_resistInfoSum);

	// ������
	if (registInfo != NULL) {
		registInfo->SetMtx(mtx);
	}

	return registInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�`��}�l�[�W���[�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ���_���������
//========================================
void CDrawMgr::AssignVertexInfo(void) {

	// ���_2D���ɕϊ�
	if (CPolygon2D::CDrawInfo::m_vtxBuff != NULL) {

		// ���_�o�b�t�@�����b�N
		Vertex2D* vtxs = NULL;
		CPolygon2D::CDrawInfo::m_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSum);
		ConvDrawInfoToVertex2DInfo(vtxs, ms_drawInfoSumScreen);

		// ���_�o�b�t�@���A�����b�N
		CPolygon2D::CDrawInfo::m_vtxBuff->Unlock();
	}

	// ���_3D���ɕϊ�
	if (CPolygon3D::CDrawInfo::m_vtxBuff != NULL) {

		// ���_�o�b�t�@�����b�N
		Vertex3D* vtxs = NULL;
		CPolygon3D::CDrawInfo::m_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSum);
		ConvDrawInfoToVertex3DInfo(vtxs, ms_drawInfoSumScreen);

		// ���_�o�b�t�@���A�����b�N
		CPolygon3D::CDrawInfo::m_vtxBuff->Unlock();
	}
}


//========================================
// �`����𒸓_2D���ɕϊ�
//========================================
void CDrawMgr::ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.ms_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.ms_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON2D)
		{// �J�E���g�̕`���񂪃|���S��2D�̎��A
			// ���_������
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
// �`����𒸓_3D���ɕϊ�
//========================================
void CDrawMgr::ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.ms_drawInfoNum; cntDrawInfo++) {

		if (drawInfoSum.ms_drawInfos[cntDrawInfo]->m_type == CDrawInfoBase::TYPE::POLYGON3D)
		{// �J�E���g�̕`���񂪃|���S��2D�̎��A
			// ���_������
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
// �o�^����(�|���S��2D)
//========================================
CPolygon2D::CRegistInfo* CDrawMgr::RegistPolygon2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon2DRegistInfoNum == resistInfo.m_polygon2DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_polygon2DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_polygon2DRegistInfoAllocNum;
		resistInfo.m_polygon2DRegistInfoAllocNum = pow(2, resistInfo.m_polygon2DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		CMemory::ReAlloc(&resistInfo.m_polygon2DRegistInfos, oldAllocNum, resistInfo.m_polygon2DRegistInfoAllocNum);
	}

	resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum].ClearParameter();
	return &resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum++];
}

//========================================
// �o�^����(�|���S��3D)
//========================================
CPolygon3D::CRegistInfo* CDrawMgr::RegistPolygon3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon3DRegistInfoNum == resistInfo.m_polygon3DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_polygon3DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_polygon3DRegistInfoAllocNum;
		resistInfo.m_polygon3DRegistInfoAllocNum = pow(2, resistInfo.m_polygon3DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		CMemory::ReAlloc(&resistInfo.m_polygon3DRegistInfos, oldAllocNum, resistInfo.m_polygon3DRegistInfoAllocNum);
	}

	resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum].ClearParameter();
	return &resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum++];
}

//========================================
// �o�^����(�e�L�X�g2D)
//========================================
CText2D::CRegistInfo* CDrawMgr::RegistText2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text2DRegistInfoNum == resistInfo.m_text2DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_text2DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_text2DRegistInfoAllocNum;
		resistInfo.m_text2DRegistInfoAllocNum = pow(2, resistInfo.m_text2DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		CMemory::ReAlloc(&resistInfo.m_text2DRegistInfos, oldAllocNum, resistInfo.m_text2DRegistInfoAllocNum);
	}

	resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum].ClearParameter();
	return &resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum++];
}

//========================================
// �o�^����(�e�L�X�g3D)
//========================================
CText3D::CRegistInfo* CDrawMgr::RegistText3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text3DRegistInfoNum == resistInfo.m_text3DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_text3DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_text3DRegistInfoAllocNum;
		resistInfo.m_text3DRegistInfoAllocNum = pow(2, resistInfo.m_text3DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		CMemory::ReAlloc(&resistInfo.m_text3DRegistInfos, oldAllocNum, resistInfo.m_text3DRegistInfoAllocNum);
	}

	resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum].ClearParameter();
	return &resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum++];
}

//========================================
// �o�^����(���f��)
//========================================
CModel::CRegistInfo* CDrawMgr::RegistModel(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_modelRegistInfoNum == resistInfo.m_modelRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_modelRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_modelRegistInfoAllocNum;
		resistInfo.m_modelRegistInfoAllocNum = pow(2, resistInfo.m_modelRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		CMemory::ReAlloc(&resistInfo.m_modelRegistInfos, oldAllocNum, resistInfo.m_modelRegistInfoAllocNum);
	}

	resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum].ClearParameter();
	return &resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum++];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�`��}�l�[�W���[�N���X�̃X���b�h�p�����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]���C�����[�v
//========================================
void CDrawMgr::MainLoop(void) {
	while (true) {
		if (ms_processState == PROCESS_STATE::REGIST_INFO_APPLY_WAIT)
		{// [ �o�^���K�p�҂� ]�̎��A
			// �o�^�������ɐݒu����
			PutBasedRegistInfo(ms_resistInfoSum, false);
			PutBasedRegistInfo(ms_resistInfoSumScreen, true);

			// �o�^����`����ɕϊ�����
			ConvRegistInfoToDrawInfo(ms_resistInfoSum, ms_drawInfoSumOvr);
			ConvRegistInfoToDrawInfo(ms_resistInfoSumScreen, ms_drawInfoSumScreenOvr);

			{// �`������\�[�g����
				// �J����
				int cnt = 0;
				for (; cnt < ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfoNum - 1; cnt++)
					SortDrawInfo(ms_drawInfoSumOvr, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt]);

				// �X�N���[��
				SortDrawInfo(ms_drawInfoSumScreenOvr, ms_cameraDrawOrderInfoSumOvr.cameraDrawOrderInfos[cnt]);
			}

			// [ �`�������ւ��҂� ]�ɂ���
			ms_processState = PROCESS_STATE::DRAW_INFO_SWAP_WAIT;
		}

		// ���S�͂ŉ񂳂Ȃ��悤��0�b�X���[�v
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

//========================================
// [�ÓI]�o�^�������ɐݒu����
//========================================
void CDrawMgr::PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const bool& isOnScreen) {

	//----------------------------------------
	// �e�L�X�g3D����|���S��3D��ݒu
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text3DRegistInfoNum; cnt++) {
		resistInfoSum.m_text3DRegistInfos[cnt].PutPolygon3D(isOnScreen);
	}

	// ���
	resistInfoSum.m_text3DRegistInfoNum = 0;

	//----------------------------------------
	// �e�L�X�g2D����|���S��2D��ݒu
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text2DRegistInfoNum; cnt++) {
		resistInfoSum.m_text2DRegistInfos[cnt].PutPolygon2D(isOnScreen);
	}

	// ���
	resistInfoSum.m_text2DRegistInfoNum = 0;
}

//========================================
// [�ÓI]�o�^����`����ɕϊ�����
//========================================
void CDrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum) {

	int cntDrawInfo = 0;

	{// �`����̃������m��
		drawInfoSum.ms_drawInfoNum =
			resistInfoSum.m_polygon2DRegistInfoNum +	// �|���S��2D
			resistInfoSum.m_polygon3DRegistInfoNum +	// �|���S��3D
			resistInfoSum.m_modelRegistInfoNum;			// ���f��
		CMemory::Alloc<CDrawInfoBase*>(&drawInfoSum.ms_drawInfos, drawInfoSum.ms_drawInfoNum, true);
	}

	//----------------------------------------
	// �|���S��3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon3DRegistInfos[cnt].ConvToDrawInfo();
	}

	// ���
	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// ���f��
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_modelRegistInfos[cnt].ConvToDrawInfo();
	}

	// ���
	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// �|���S��2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++, cntDrawInfo++) {
		drawInfoSum.ms_drawInfos[cntDrawInfo] = resistInfoSum.m_polygon2DRegistInfos[cnt].ConvToDrawInfo();
	}

	// ���
	resistInfoSum.m_polygon2DRegistInfoNum = 0;
}

//========================================
// [�ÓI]�`����\�[�g����
//========================================
void CDrawMgr::SortDrawInfo(CDrawInfoSum& drawInfoSum, CameraDrawOrderInfo& cameraDrawOrderInfo) {

	// �`����̐���0�ȉ��Ȃ�I��
	if (drawInfoSum.ms_drawInfoNum <= 0)
		return;

	// �`�揇���X�g��`���񐔕��������m��
	CMemory::Alloc(&cameraDrawOrderInfo.orderList, drawInfoSum.ms_drawInfoNum, true);

	for (int cnt = 0; cnt < drawInfoSum.ms_drawInfoNum; cnt++) {

		// �`�揇���X�g�̒l��������
		cameraDrawOrderInfo.orderList[cnt] = cnt;

		// �\�[�g�p�ɃJ�����܂ł̋����𒲂ׂ�
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
	// ���[�J���֐��̒�`
	//----------------------------------------
	struct LocalFunc {
		// [[[ �D�悷�邩���ׂ�(�|���S��3D�ƃ��f��) ]]]
		static bool FindPrioritizePolygon3DAndModel(CPolygon3D::CDrawInfo* base, CModel::CDrawInfo* target) {

			if (base->m_isZTest && !target->m_isZTest) return true;		// ���Z�e�X�g��ON�Ŕ�r�Ώۂ̂�Z�e�X�g��OFF
			if (!base->m_isZTest && target->m_isZTest) return false;	// ���Z�e�X�g��OFF�Ŕ�r�Ώۂ�Z�e�X�g��ON
			if (base->m_distance > target->m_distance) return true;		// ��̕�������������

			return false;
		}

		// [[[ �D�悷�邩���ׂ� ]]]
		// ���Ԃ��͕̂`���D�悷�邩�Ȃ̂Œ���
		static bool FindPrioritize(CDrawInfoBase& base, CDrawInfoBase& target) {

			// �Е������݂��Ȃ�
			if (&base == NULL || &target == NULL)
				return false;
			
			// ��̕����D��x���Ⴂ
			if (base.m_priority < target.m_priority) 
				return true;

			// ��̕����D��x������
			if (base.m_priority > target.m_priority)
				return false;

			if (base.m_type == CDrawInfoBase::TYPE::POLYGON2D) {
				//----------------------------------------
				// [[[ Base:�|���S��2D ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D)
				{// [[[ Target::�|���S��2D ]]]
					CPolygon2D::CDrawInfo* castedBase   = (CPolygon2D::CDrawInfo*)&base;
					CPolygon2D::CDrawInfo* castedTarget = (CPolygon2D::CDrawInfo*)&target;

					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// ���Z�e�X�g��ON�Ŕ�r�Ώۂ̂�Z�e�X�g��OFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// ���Z�e�X�g��OFF�Ŕ�r�Ώۂ�Z�e�X�g��ON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// ��̕�������������
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::�|���S��3D ]]]
					return false;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:���f�� ]]]
					return false;
				}
				else assert(false);
			}
			else if (base.m_type == CDrawInfoBase::TYPE::POLYGON3D) {
				//----------------------------------------
				// [[[ Base:�|���S��3D ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D) 
				{// [[[ Target::�|���S��2D ]]]
					return true;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::�|���S��3D ]]]
					CPolygon3D::CDrawInfo* castedBase   = (CPolygon3D::CDrawInfo*)&base;
					CPolygon3D::CDrawInfo* castedTarget = (CPolygon3D::CDrawInfo*)&target;

					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// ���Z�e�X�g��ON�Ŕ�r�Ώۂ̂�Z�e�X�g��OFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// ���Z�e�X�g��OFF�Ŕ�r�Ώۂ�Z�e�X�g��ON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// ��̕�������������
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:���f�� ]]]
					return FindPrioritizePolygon3DAndModel((CPolygon3D::CDrawInfo*)&base, (CModel::CDrawInfo*)&target);
				}
				else assert(false);
			}
			else if (base.m_type == CDrawInfoBase::TYPE::MODEL) {
				//----------------------------------------
				// [[[ Base:���f�� ]]]
				//----------------------------------------
				if (target.m_type == CDrawInfoBase::TYPE::POLYGON2D)
				{// [[[ Target::�|���S��2D ]]]
					return true;
				}
				else if (target.m_type == CDrawInfoBase::TYPE::POLYGON3D)
				{// [[[ Target::�|���S��3D ]]]
					return !FindPrioritizePolygon3DAndModel((CPolygon3D::CDrawInfo*)&target, (CModel::CDrawInfo*)&base);
				}
				else if (target.m_type == CDrawInfoBase::TYPE::MODEL)
				{// [[[ Target:���f�� ]]]
					CModel::CDrawInfo* castedBase = (CModel::CDrawInfo*)&base;
					CModel::CDrawInfo* castedTarget = (CModel::CDrawInfo*)&target;
					
					if (castedBase->m_isZTest && !castedTarget->m_isZTest) return true;		// ���Z�e�X�g��ON�Ŕ�r�Ώۂ̂�Z�e�X�g��OFF
					if (!castedBase->m_isZTest && castedTarget->m_isZTest) return false;	// ���Z�e�X�g��OFF�Ŕ�r�Ώۂ�Z�e�X�g��ON
					if (castedBase->m_distance > castedTarget->m_distance) return true;		// ��̕�������������
				}
				else assert(false);
			}
			else assert(false);

			return false;
		}
	};

	//----------------------------------------
	// �\�[�g����
	//      cnt1
	// [ 0 ][ 1 ][ 2 ][ 3 ][ 4 ][ �c ]
	// �����̏ꍇ2�����r���Ă���
	//----------------------------------------
	int cnt1Num = drawInfoSum.ms_drawInfoNum - 1;
	for (int cnt1 = 0; cnt1 < cnt1Num; cnt1++) {

		// �ŗD��̃J�E���g�𒲂ׂ�
		int cntTop = NONEDATA;
		for (int cnt2 = cnt1 + 1; cnt2 < drawInfoSum.ms_drawInfoNum; cnt2++) {
			CDrawInfoBase& drawInfoCnt2 = *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cnt2]];

			//----------------------------------------
			// Cnt2�̕`���� �� ���̍ŗD��`���� ���
			// �D��x���������ǂ������肷��
			//----------------------------------------
			bool isTopPriorityCnt2 = false;

			// �ŗD��̃J�E���g��NONE(�܂�ŏ��̃J�E���g)�̎��ACnt2�̕����D��x�������Ƃ���
			if (cntTop == NONEDATA) {
				isTopPriorityCnt2 = true;
			}
			else {
				isTopPriorityCnt2 = LocalFunc::FindPrioritize(drawInfoCnt2, *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cntTop]]);
			}
			
			// ����̌��ʁc
			// Cnt2�̕`���� �̕����D��x�������������A
			// �ŗD��̃J�E���g���㏑������
			if (isTopPriorityCnt2)
				cntTop = cnt2;
		}

		// Cnt1�`���� �̕��� �ŗD��`���� ���D��x���Ⴂ���A����ւ���
		if (!LocalFunc::FindPrioritize(*drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cnt1]], *drawInfoSum.ms_drawInfos[cameraDrawOrderInfo.orderList[cntTop]])) {
			UShort temp = cameraDrawOrderInfo.orderList[cnt1];
			cameraDrawOrderInfo.orderList[cnt1] = cameraDrawOrderInfo.orderList[cntTop];
			cameraDrawOrderInfo.orderList[cntTop] = temp;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�`���񑍊��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CDrawMgr::CDrawInfoSum::CDrawInfoSum() {

	ms_drawInfos   = NULL;
	ms_drawInfoNum = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CDrawMgr::CDrawInfoSum::~CDrawInfoSum() {

}

//========================================
// �������
//========================================
void CDrawMgr::CDrawInfoSum::Release(void) {

	// �`��������
	if (ms_drawInfos != NULL) {
		for (int cnt = 0; cnt < ms_drawInfoNum; cnt++) {
			CMemory::Release(&ms_drawInfos[cnt]);
		}
		CMemory::Release(&ms_drawInfos);
	}
}

//========================================
// �㏑������
//========================================
void CDrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

	// �������
	Release();

	// �㏑��
	ms_drawInfos         = pOvr->ms_drawInfos;
	ms_drawInfoNum       = pOvr->ms_drawInfoNum;
	pOvr->ms_drawInfos   = NULL;
	pOvr->ms_drawInfoNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�o�^����񑍊��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//========================================
CDrawMgr::CRegistInfoSum::~CRegistInfoSum() {

}

//========================================
// �����m�ۏ���
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
// �Ċm�ۏ���
//========================================
void CDrawMgr::CRegistInfoSum::ReAlloc() {

	//----------------------------------------
	// �|���S��2D
	//----------------------------------------
	if (m_polygon2DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_polygon2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon2DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				CMemory::ReAlloc(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_polygon2DRegistInfoAllocNum   = allocLine;
				m_polygon2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �|���S��3D
	//----------------------------------------
	if (m_polygon3DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_polygon3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon3DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				CMemory::ReAlloc(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_polygon3DRegistInfoAllocNum   = allocLine;
				m_polygon3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �e�L�X�g2D
	//----------------------------------------
	if (m_text2DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_text2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text2DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				CMemory::ReAlloc(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_text2DRegistInfoAllocNum   = allocLine;
				m_text2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �e�L�X�g3D
	//----------------------------------------
	if (m_text3DRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_text3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text3DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				CMemory::ReAlloc(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_text3DRegistInfoAllocNum   = allocLine;
				m_text3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// ���f��
	//----------------------------------------
	if (m_modelRegistInfoAllocPower > REGIST_ALLOC_BASE_POWER) {

		for (int cntAlloc = REGIST_ALLOC_BASE_POWER; cntAlloc < m_modelRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_modelRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				CMemory::ReAlloc(&m_modelRegistInfos, m_modelRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_modelRegistInfoAllocNum   = allocLine;
				m_modelRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}
}

//========================================
// �������
//========================================
void CDrawMgr::CRegistInfoSum::Release() {

	CMemory::Release(&m_polygon2DRegistInfos);
	CMemory::Release(&m_polygon3DRegistInfos);
	CMemory::Release(&m_text2DRegistInfos);
	CMemory::Release(&m_text3DRegistInfos);
	CMemory::Release(&m_modelRegistInfos);
}