//========================================
// 
// �X�^�e�B�b�N���b�V���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�X�^�e�B�b�N���b�V���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CStaticMesh::CStaticMesh() {

	m_meshes         = NULL;
	m_meshNums       = 0;
	m_meshesScreen   = NULL;
	m_meshNumsScreen = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CStaticMesh::~CStaticMesh() {

}

//========================================
// ����������
//========================================
void CStaticMesh::Init(const UShort& priorityMax) {

	CMemory::Alloc(&m_meshes, priorityMax);
	CMemory::Alloc(&m_meshNums, priorityMax);
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		m_meshes  [cnt] = NULL;
		m_meshNums[cnt] = 0;
	}
	CMemory::Alloc(&m_meshesScreen, CDrawMgr::SCREEN_PRIORITY_MAX);
	CMemory::Alloc(&m_meshNumsScreen, CDrawMgr::SCREEN_PRIORITY_MAX);
	for (int cnt = 0; cnt < CDrawMgr::SCREEN_PRIORITY_MAX; cnt++) {
		m_meshesScreen  [cnt] = NULL;
		m_meshNumsScreen[cnt] = 0;
	}
}

//========================================
// �I������
//========================================
void CStaticMesh::Uninit(void) {

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void CStaticMesh::Update(void) {

}

//========================================
// �`�揈��
//========================================
void CStaticMesh::Draw(Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	// �X�N���[�����ɉ����ĕϐ����Q��
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (meshes[priority] == NULL)
		return;

	if (meshNums[priority] == NULL)
		return;

	for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {

		if (meshes[priority][cntMesh] == NULL)
			continue;

		// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
		if (meshes[priority][cntMesh]->m_clippingID != NONEDATA || isCameraClipping)
			if (meshes[priority][cntMesh]->m_clippingID != cameraID)
				continue;

		meshes[priority][cntMesh]->Draw(device);
	}
}

//========================================
// �������
//========================================
void CStaticMesh::Release(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++)
		CMemory::ReleaseDouble(&m_meshes[cnt], m_meshNums[cnt]);
	CMemory::Release(&m_meshes);
	CMemory::Release(&m_meshNums);
	for (int cnt = 0; cnt < CDrawMgr::SCREEN_PRIORITY_MAX; cnt++)
		CMemory::ReleaseDouble(&m_meshesScreen[cnt], m_meshNumsScreen[cnt]);
	CMemory::Release(&m_meshesScreen);
	CMemory::Release(&m_meshNumsScreen);
}

//========================================
// �폜����
//========================================
void CStaticMesh::Delete(const bool& isOnScreen) {

	// �X�N���[�����ɉ����ĕϐ����Q��
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (meshNums == NULL)
		return;

	const UShort& priorityMax = isOnScreen ? CDrawMgr::SCREEN_PRIORITY_MAX : RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		for (int cntMesh = 0; cntMesh < meshNums[cnt]; cntMesh++) {
			CMemory::Release(&meshes[cnt][cntMesh]);
		}
		meshNums[cnt] = 0;
	}
}

//========================================
// ���f���ݒ菈��
//========================================
void CStaticMesh::SetModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	SetModel(priority, CMatrix::ConvPosRotScaleToMtx(pos, rot, scale), modelIdx, col, isOnScreen);
}
void CStaticMesh::SetModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	SetModel(priority, CMatrix::ConvPosRotToMtx(pos, rot), modelIdx, col, isOnScreen);
}
void CStaticMesh::SetModel(const UShort& priority, const Matrix& mtx, const short& modelIdx, const Color& col, const bool& isOnScreen) {
	
	// �X�N���[�����ɉ����ĕϐ����Q��
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	if (modelIdx == NONEDATA)
		return;

	// ���f���f�[�^���擾
	const CModel::CData& data = RNLib::Model().GetData(modelIdx);

	for (int cntMat = 0; cntMat < data.m_matNum; cntMat++) {

		bool isSet = false;
		for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {
			CMesh& mesh = *meshes[priority][cntMesh];

			if (mesh.m_texIdx == data.m_texIdxes[cntMat]) {
				if (mesh.SetModel(mtx, modelIdx, col, data.m_texIdxes[cntMat], cntMat))
				{// ���b�V���̐ݒ�ɐ�������
					isSet = true;
					break;
				}
			}
		}

		if(!isSet){
			// �������b�V���������������A���b�V����V�K�쐬
			NewCreateMesh(priority, mtx, modelIdx, data.m_texIdxes[cntMat], cntMat, col, isOnScreen);
		}
	}
}

//========================================
// �}�e���A�����f���ݒ菈��
//========================================
void CStaticMesh::SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	SetMaterialModel(priority, CMatrix::ConvPosRotScaleToMtx(pos, rot, scale), modelIdx, texIdx, col, isOnScreen);
}
void CStaticMesh::SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	SetMaterialModel(priority, CMatrix::ConvPosRotToMtx(pos, rot), modelIdx, texIdx, col, isOnScreen);
}
void CStaticMesh::SetMaterialModel(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {
	
	// �X�N���[�����ɉ����ĕϐ����Q��
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	for (int cntMesh = 0; cntMesh < meshNums[priority]; cntMesh++) {
		CMesh& mesh = *meshes[priority][cntMesh];

		if (mesh.m_texIdx == texIdx) {
			if (mesh.SetModel(mtx, modelIdx, col, texIdx, NONEDATA)) 
			{// ���b�V���̐ݒ�ɐ�������
				return;
			}
		}
	}

	// �������b�V���������������A���b�V����V�K�쐬
	NewCreateMesh(priority, mtx, modelIdx, texIdx, NONEDATA, col, isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�X�^�e�B�b�N���b�V���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ���b�V����V�K�쐬
//========================================
void CStaticMesh::NewCreateMesh(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const short& matIdx, const Color& col, const bool& isOnScreen) {

	// �X�N���[�����ɉ����ĕϐ����Q��
	CMesh***& meshes   = isOnScreen ? m_meshesScreen : m_meshes;
	UShort*&  meshNums = isOnScreen ? m_meshNumsScreen : m_meshNums;

	// ���b�V�����Ċm��
	const UShort oldNum = meshNums[priority];
	CMemory::ReAllocDouble(&meshes[priority], oldNum, ++meshNums[priority]);

	// ���b�V����ݒ�
	meshes[priority][oldNum]->m_texIdx = texIdx;
	meshes[priority][oldNum]->SetModel(mtx, modelIdx, col, texIdx, matIdx);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���b�V���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CStaticMesh::CMesh::CMesh() {

	m_texIdx     = NONEDATA;
	m_clippingID = NONEDATA;
	m_vtxBuff    = NULL;
	m_vtxNum     = 0;
	m_idxBuff    = NULL;
	m_idxNum     = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CStaticMesh::CMesh::~CMesh() {

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
// �`�揈��
//========================================
void CStaticMesh::CMesh::Draw(Device& device) {

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(Vertex3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	device->SetIndices(m_idxBuff);
	
	// [[[ �e�N�X�`���̐ݒ� ]]]
	Polygon2DAnd3D::SetTexture(device, (void*)(&m_texIdx), Polygon2DAnd3D::TEX_TYPE::IDX);

	// �|���S���̕`��
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vtxNum, 0, m_idxNum / 3);
}

//========================================
// ���b�V���ݒ菈��
//========================================
bool CStaticMesh::CMesh::SetModel(const Matrix& mtx, const short& modelIdx, const Color& col, const short& texIdx, const short& matIdx) {

	// �f�o�C�X���擾
	Device& device = RNLib::Window().GetD3DDevice();

	// �ߋ��̐��Ƃ��ĕۑ�
	const ULong vtxNumOld = m_vtxNum;
	const ULong idxNumOld = m_idxNum;

	// ���f���f�[�^���擾
	CModel::CData& modelData = RNLib::Model().GetData(modelIdx);
	CModel::CData::MatData matData = {};
	if (matIdx != NONEDATA) {
		matData = modelData.m_matDatas[matIdx];
	}

	// �ǉ����钸�_���ƃC���f�b�N�X�����擾
	CModel::Vertex3DInfo* addVtxes = NULL;
	ULong   addVtxNum = (matIdx == NONEDATA) ? modelData.m_vtxNum : modelData.m_matDatas[matIdx].vtxNum;
	RNLib::Model().StoreVtxInfo(mtx, modelIdx, &addVtxes, matIdx);
	ULong*& addIdxes  = (matIdx == NONEDATA) ? modelData.m_idxes : modelData.m_matDatas[matIdx].idxes;
	ULong&  addIdxNum = (matIdx == NONEDATA) ? modelData.m_idxNum : modelData.m_matDatas[matIdx].idxNum;

	// ���_�ƃC���f�b�N�X�������Z
	m_vtxNum += addVtxNum;
	m_idxNum += addIdxNum + (m_idxBuff != NULL) * 3;	// ���k�ރ|���S���Ɏg�p���镪���Z

	if (m_vtxNum > USHRT_MAX || m_idxNum > USHRT_MAX || m_vtxNum <= 0 || m_idxNum <= 0) {

		// �ǉ����钸�_�������
		CMemory::Release(&addVtxes);

		return false;
	}

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@��V��������
	VertexBuffer newVtxBuff = NULL;
	device->CreateVertexBuffer(sizeof(Vertex3D) * m_vtxNum, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &newVtxBuff, NULL);
	IndexBuffer newIdxBuff = NULL;
	device->CreateIndexBuffer(sizeof(WORD) * m_idxNum, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &newIdxBuff, NULL);
	
	{// [[[ ���̏㏑�� ]]]

		// ���_�o�b�t�@�����b�N
		Vertex3D* vtxes = NULL;
		Vertex3D* newVtxes = NULL;
		if (m_vtxBuff != NULL)
			m_vtxBuff->Lock(0, 0, (void**)&vtxes, 0);
		newVtxBuff->Lock(0, 0, (void**)&newVtxes, 0);

		// ���̒��_�����ڂ��ւ�
		if (m_vtxBuff != NULL) {
			for (UInt cntVtx = 0; cntVtx < vtxNumOld; newVtxes[cntVtx] = vtxes[cntVtx], cntVtx++);
		}

		{// �ǉ����钸�_�����ڂ��ւ�
			UInt vtxCount = 0;
			for (UInt cntVtx = vtxNumOld; cntVtx < m_vtxNum; cntVtx++) {
				newVtxes[cntVtx].pos = addVtxes[vtxCount].worldPos;
				newVtxes[cntVtx].nor = addVtxes[vtxCount].worldNor;
				newVtxes[cntVtx].tex = addVtxes[vtxCount].texPos;
				newVtxes[cntVtx].col = (matIdx == NONEDATA) ? D3DCOLOR_RGBA(col.r, col.g, col.b, col.a) : matData.col.GetMixed(col).ConvD3DCOLOR();
				vtxCount++;
			}
		}

		// �ǉ����钸�_�������
		CMemory::Release(&addVtxes);

		// ���_�o�b�t�@���A�����b�N
		if (m_vtxBuff != NULL)
			m_vtxBuff->Unlock();
		newVtxBuff->Unlock();

		// �C���f�b�N�X�o�b�t�@�����b�N
		WORD* idxes = NULL;
		WORD* newIdxes = NULL;
		if (m_idxBuff != NULL)
			m_idxBuff->Lock(0, 0, (void**)&idxes, 0);
		newIdxBuff->Lock(0, 0, (void**)&newIdxes, 0);

		// �C���f�b�N�X�����ڂ��ւ�
		if (m_idxBuff != NULL)
			for (UInt cntIdx = 0; cntIdx < idxNumOld; newIdxes[cntIdx] = idxes[cntIdx], cntIdx++);

		{// �V�����C���f�b�N�X���̐ݒ�
			UInt idxCount = 0;
			for (UInt cntIdx = idxNumOld + (m_idxBuff != NULL) * 3; cntIdx < m_idxNum; cntIdx++) {
				newIdxes[cntIdx] = vtxNumOld + addIdxes[idxCount];
				idxCount++;
			}
		}

		// �k�ރC���f�b�N�X�̐ݒ�
		if (m_idxBuff != NULL) {
			newIdxes[idxNumOld] = newIdxes[idxNumOld - 1];
			newIdxes[idxNumOld + 1] = newIdxes[idxNumOld + 2];
			newIdxes[idxNumOld + 2] = newIdxes[idxNumOld + 2];
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N
		if (m_idxBuff != NULL)
			m_idxBuff ->Unlock();
		newIdxBuff->Unlock();
	}

	// �Â����_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�����
	if (m_vtxBuff != NULL) {
		m_vtxBuff->Release();
	}
	if (m_idxBuff != NULL) {
		m_idxBuff->Release();
	}

	// �V�������_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@����
	m_vtxBuff = newVtxBuff;
	m_idxBuff = newIdxBuff;

	return true;
}

//========================================
// ���b�V���擾����
//========================================
UShort CStaticMesh::GetMeshNum(void) {

	const UShort priorityMax = RNLib::DrawMgr().GetPriorityMax();

	UShort totalMeshNum = 0;
	for (int cnt = 0; cnt < priorityMax; cnt++)
		totalMeshNum += m_meshNums[cnt];
	for (int cnt = 0; cnt < CDrawMgr::SCREEN_PRIORITY_MAX; cnt++)
		totalMeshNum += m_meshNumsScreen[cnt];

	return totalMeshNum;
}