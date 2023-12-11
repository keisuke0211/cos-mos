//========================================
// 
// �}�e���A�����b�V���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�}�e���A�����b�V���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CMatMesh::CMatMesh() {

	m_meshes   = NULL;
	m_meshNums = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CMatMesh::~CMatMesh() {

}

//========================================
// ����������
//========================================
void CMatMesh::Init(const UShort& priorityMax) {

	CMemory::Alloc(&m_meshes, priorityMax);
	CMemory::Alloc(&m_meshNums, priorityMax);
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		m_meshes[cnt] = NULL;
		m_meshNums[cnt] = 0;
	}
}

//========================================
// �I������
//========================================
void CMatMesh::Uninit(void) {

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void CMatMesh::Update(void) {

}

//========================================
// �`�揈��
//========================================
void CMatMesh::Draw(Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen) {

	if (m_meshNums == NULL)
		return;

	// ���[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &INITMATRIX);

	// ���_�t�H�[�}�b�g�̐ݒ�
	device->SetFVF(FVF_VERTEX_3D);

	for (int cntMesh = 0; cntMesh < m_meshNums[priority]; cntMesh++) {

		if (m_meshes[priority][cntMesh] == NULL)
			continue;

		if (m_meshes[priority][cntMesh]->m_isOnScreen != isOnScreen)
			continue;

		// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
		if (m_meshes[priority][cntMesh]->m_clippingID != NONEDATA || isCameraClipping)
			if (m_meshes[priority][cntMesh]->m_clippingID != cameraID)
				continue;

		m_meshes[priority][cntMesh]->Draw(device);
	}
}

//========================================
// �������
//========================================
void CMatMesh::Release(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++)
		CMemory::ReleaseDouble(&m_meshes[cnt], m_meshNums[cnt]);
	CMemory::Release(&m_meshes);
	CMemory::Release(&m_meshNums);
}

//========================================
// �폜����
//========================================
void CMatMesh::Delete(void) {

	const UShort& priorityMax = RNLib::DrawMgr().GetPriorityMax();
	for (int cnt = 0; cnt < priorityMax; cnt++) {
		for (int cntMesh = 0; cntMesh < m_meshNums[cnt]; cntMesh++) {
			CMemory::Release(&m_meshes[cnt][cntMesh]);
		}
		m_meshNums[cnt] = 0;
	}
}

//========================================
// ���b�V���ݒ菈��
//========================================
void CMatMesh::SetMesh(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen) {

	for (int cntMesh = 0; cntMesh < m_meshNums[priority]; cntMesh++) {
		CMesh& mesh = *m_meshes[priority][cntMesh];

		if (mesh.m_texIdx == texIdx && mesh.m_isOnScreen == isOnScreen) {
			if (mesh.SetMesh(mtx, modelIdx, col)) 
			{// ���b�V���̐ݒ�ɐ�������
				return;
			}
		}
	}

	// ~~~ �������b�V������������ ~~~
	
	// ���b�V�����Ċm��
	const UShort oldNum = m_meshNums[priority];
	CMemory::ReAllocDouble(&m_meshes[priority], oldNum, ++m_meshNums[priority]);

	// ���b�V����ݒ�
	m_meshes[priority][oldNum]->m_texIdx = texIdx;
	m_meshes[priority][oldNum]->m_isOnScreen = isOnScreen;
	m_meshes[priority][oldNum]->SetMesh(mtx, modelIdx, col);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���b�V���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CMatMesh::CMesh::CMesh() {

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
CMatMesh::CMesh::~CMesh() {

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
void CMatMesh::CMesh::Draw(Device& device) {

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
bool CMatMesh::CMesh::SetMesh(const Matrix& mtx, const short& modelIdx, const Color& col) {

	// �f�o�C�X���擾
	Device& device = RNLib::Window().GetD3DDevice();

	// �ߋ��̐��Ƃ��ĕۑ�
	const UInt vtxNumOld = m_vtxNum;
	const UInt idxNumOld = m_idxNum;

	// ���f���f�[�^���擾
	CModel::CData& modelData = RNLib::Model().GetData(modelIdx);

	// �ǉ����钸�_���ƃC���f�b�N�X�����擾
	CModel::Vertex3DInfo* addVtxes = NULL;
	UInt addVtxNum = 0;
	RNLib::Model().StoreVtxInfo(mtx, modelIdx, &addVtxNum, &addVtxes);
	UShort*& addIdxes  = modelData.m_idxes;
	UInt&    addIdxNum = modelData.m_idxNum;

	// ���_�ƃC���f�b�N�X�������Z
	m_vtxNum += addVtxNum;
	m_idxNum += addIdxNum + (m_idxBuff != NULL) * 3;	// ���k�ރ|���S���Ɏg�p���镪���Z

	if (m_vtxNum > USHRT_MAX || m_idxNum > USHRT_MAX) {

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

		// ���_�����ڂ��ւ�
		if (m_vtxBuff != NULL)
			for (UInt cntVtx = 0; cntVtx < vtxNumOld; newVtxes[cntVtx] = vtxes[cntVtx], cntVtx++);

		{// �V�������_���̐ݒ�
			UInt vtxCount = 0;
			for (UInt cntVtx = vtxNumOld; cntVtx < m_vtxNum; cntVtx++) {
				newVtxes[cntVtx].pos = addVtxes[vtxCount].worldPos;
				newVtxes[cntVtx].nor = addVtxes[vtxCount].worldNor;
				newVtxes[cntVtx].tex = addVtxes[vtxCount].texPos;
				newVtxes[cntVtx].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
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