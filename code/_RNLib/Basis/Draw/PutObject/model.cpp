//========================================
// 
// ���f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include "../../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���f���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Model::_RNC_Model() {

	m_datas = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Model::~_RNC_Model() {
	
}

//========================================
// ����������
//========================================
void _RNC_Model::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_Model::Uninit(void) {

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void _RNC_Model::Update(void) {

}

//========================================
// �������
//========================================
void _RNC_Model::Release(void) {

	// �f�[�^�̉��
	RNLib::Memory().ReleaseDouble(&m_datas, m_num);
}

//========================================
// �ǂݍ��ݏ���
//========================================
short _RNC_Model::Load(const char* loadPath, short idx) {

	const UShort oldNum = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx)) 
	{// �ǂݍ��ݐ���
		// �f�[�^�̃������Ċm��
		RNLib::Memory().ReAllocDouble(&m_datas, oldNum, m_num);

		// �f�[�^�̔j��(�ԍ��w��̏ꍇ)
		if (idxOld != NONEDATA)
			m_datas[idx]->Release();

		// X�t�@�C���̓ǂݍ���
		Device device(RNLib::Window().GetD3DDevice());
		if (FAILED(D3DXLoadMeshFromX(loadPath, D3DXMESH_SYSTEMMEM, device, NULL, &m_datas[idx]->m_matBuff, NULL, (DWORD*)&m_datas[idx]->m_matNum, &m_datas[idx]->m_mesh))) 
		{// �ǂݍ��ݎ��s
			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(String("���f���̓ǂݍ��݂Ɏ��s���܂����B\n%s", loadPath));

			// �f�[�^�̃��������Z�b�g
			RNLib::Memory().ReAllocDouble(&m_datas, m_num, oldNum);

			// �ǂݍ��ݍσp�X�̃��������Z�b�g
			ReAllocLoadPath(oldNum);

			// �������Z�b�g
			m_num = oldNum;

			return NONEDATA;
		}
		else
		{// �ǂݍ��݂ɐ����������A
			const DWORD fvf       = m_datas[idx]->m_mesh->GetFVF();
			const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);

			// ���_�����擾
			m_datas[idx]->m_vtxNum = m_datas[idx]->m_mesh->GetNumVertices();

			// ���_�f�[�^���쐬
			RNLib::Memory().Alloc(&m_datas[idx]->m_vtxDatas, m_datas[idx]->m_vtxNum);
			for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
				m_datas[idx]->m_vtxDatas[cntVtx] = {};
				RNLib::Memory().Alloc(&m_datas[idx]->m_vtxDatas[cntVtx].isMats, m_datas[idx]->m_matNum);
				for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++)
					m_datas[idx]->m_vtxDatas[cntVtx].isMats[cntMat] = false;
			}

			// �}�e���A���f�[�^�𐶐�
			RNLib::Memory().Alloc(&m_datas[idx]->m_matDatas, m_datas[idx]->m_matNum);
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
				m_datas[idx]->m_matDatas[cntMat] = {};
			}

			// �ʂ̐����擾
			m_datas[idx]->m_faceNum = m_datas[idx]->m_mesh->GetNumFaces();

			// ���_�̉������\���̂��`
			struct VertexOutLine {
				Pos3D totalVec = INITPOS3D;
				ULong count = 0;
			};

			// ���_�̉������𐶐�
			VertexOutLine* vertexOutLines = NULL;
			RNLib::Memory().Alloc(&vertexOutLines, m_datas[idx]->m_vtxNum);
			for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

			{// ���_�̉��������Z�o
				// ���_�o�b�t�@�����b�N
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// �@�������ɉ��Z
				for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// ���a�̍ő�𒲂ׂ�
						const float dist = RNLib::Geometry().FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// �d�Ȃ��Ă��钸�_�ʒu�ɗ֊s�̉��Z���������Z
					for (ULong cntVtx2 = 0; cntVtx2 < m_datas[idx]->m_vtxNum; cntVtx2++) {

						// ���̃J�E���g�̒��_�Ɠ����Ȃ�A�܂�Ԃ�
						if (cntVtx == cntVtx2)
							continue;

						// ���_���d�Ȃ��Ă��鎞�A���Z�ʒu�̒l�����Z
						Pos3D* pos2 = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx2));
						if (*pos == *pos2) {
							vertexOutLines[cntVtx2].totalVec += *nor;
							vertexOutLines[cntVtx2].count++;
						}
					}

					// ���J�E���g���_�̉��Z�ʒu�̒l�����Z
					vertexOutLines[cntVtx].totalVec += *nor;
					vertexOutLines[cntVtx].count++;
				}

				// ���_�o�b�t�@���A�����b�N
				m_datas[idx]->m_mesh->UnlockVertexBuffer();
			}

			{// �C���f�b�N�X������
				// �C���f�b�N�X�o�b�t�@�����b�N
				WORD* idxBuff = NULL;
				m_datas[idx]->m_mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idxBuff);

				// �A�g���r���[�g�o�b�t�@�����b�N
				DWORD* attributeBuffer = NULL;
				m_datas[idx]->m_mesh->LockAttributeBuffer(0, &attributeBuffer);

				// �C���f�b�N�X�����擾
				m_datas[idx]->m_idxNum = m_datas[idx]->m_faceNum * 3;

				// �C���f�b�N�X���𐶐�
				RNLib::Memory().Alloc(&m_datas[idx]->m_idxes, m_datas[idx]->m_idxNum);

				// �ʖ��̔ԍ��𐶐�
				RNLib::Memory().Alloc(&m_datas[idx]->m_faceMatIdxes, m_datas[idx]->m_faceNum);

				for (ULong cntFace = 0; cntFace < m_datas[idx]->m_faceNum; cntFace++) {

					{// �C���f�b�N�X������
						const ULong startIdxIdx = cntFace * 3;
						for (int cnt = 0; cnt < 3; cnt++) {
							const ULong idxIdx = startIdxIdx + cnt;
							m_datas[idx]->m_idxes[idxIdx] = idxBuff[idxIdx];

							// ���̒��_�̃}�e���A���ɑ����Ă��邩�t���O
							m_datas[idx]->m_vtxDatas[idxBuff[idxIdx]].isMats[attributeBuffer[cntFace]] = true;
						}
					}

					// �}�e���A���̃C���f�b�N�X�������Z
					m_datas[idx]->m_matDatas[attributeBuffer[cntFace]].idxNum += 3;

					// �ʂ̃}�e���A���ԍ�
					m_datas[idx]->m_faceMatIdxes[cntFace] = attributeBuffer[cntFace];
				}

				for (UShort cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {

					// �}�e���A���̒��_�ԍ���
					ULong* matVtxIdxes = NULL;
					RNLib::Memory().Alloc(&matVtxIdxes, m_datas[idx]->m_vtxNum); {
						ULong matVtxCount = 0;
						for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {

							if (m_datas[idx]->m_vtxDatas[cntVtx].isMats[cntMat]) 
							{// �}�e���A�����Y�����钸�_�ł��鎞�A
								// �}�e���A�����̒��_�������Z
								m_datas[idx]->m_matDatas[cntMat].vtxNum++;

								// �}�e���A���̒��_�ԍ���ɒl����
								matVtxIdxes[cntVtx] = matVtxCount++;
							}
							else {
								// �}�e���A���̒��_�ԍ���ɒl��������
								matVtxIdxes[cntVtx] = 0;
							}
						}
					}

					// �}�e���A�����̃C���f�b�N�X���𐶐�
					RNLib::Memory().Alloc(&m_datas[idx]->m_matDatas[cntMat].idxes, m_datas[idx]->m_matDatas[cntMat].idxNum);

					// �}�e���A�����̃C���f�b�N�X���������Ă���
					ULong matFaceCount = 0;
					for (ULong cntFace = 0; cntFace < m_datas[idx]->m_faceNum; cntFace++) {
						if (cntMat == attributeBuffer[cntFace]) {
							const ULong faceStartIdx    = cntFace      * 3;
							const ULong matFaceStartIdx = matFaceCount * 3;
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx    ] = (ULong)matVtxIdxes[idxBuff[faceStartIdx    ]];
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx + 1] = (ULong)matVtxIdxes[idxBuff[faceStartIdx + 1]];
							m_datas[idx]->m_matDatas[cntMat].idxes[matFaceStartIdx + 2] = (ULong)matVtxIdxes[idxBuff[faceStartIdx + 2]];
							matFaceCount++;
						}
					}

					RNLib::Memory().Release(&matVtxIdxes);
				}

				// �C���f�b�N�X�o�b�t�@���A�����b�N
				m_datas[idx]->m_mesh->UnlockIndexBuffer();

				// �A�g���r���[�g�o�b�t�@���A�����b�N
				m_datas[idx]->m_mesh->UnlockAttributeBuffer();
			}

			// �֊s���b�V���𐶐�����
			RNLib::Memory().Alloc(&m_datas[idx]->m_outLineMeshs, RNSettings::GetInfo().modelOutLineAddDistanceDelimiter);

			for (UShort cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {

				// ���b�V���𕡐�����
				D3DXCreateMeshFVF(m_datas[idx]->m_faceNum, m_datas[idx]->m_vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);
				m_datas[idx]->m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);

				// ���_�o�b�t�@�����b�N
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMeshs[cntOutLine]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// ���_�ʒu�ɉ��Z�ʒu�����Z
				for (ULong cntVtx = 0; cntVtx < m_datas[idx]->m_vtxNum; cntVtx++) {
					Pos3D* pos = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					D3DXVec3Normalize(&vertexOutLines[cntVtx].totalVec, &vertexOutLines[cntVtx].totalVec);
					*pos += vertexOutLines[cntVtx].totalVec * RNSettings::GetInfo().modelOutLineAddDistanceInterval * (cntOutLine + 1);
				}

				// ���_�o�b�t�@���A�����b�N
				m_datas[idx]->m_outLineMeshs[cntOutLine]->UnlockVertexBuffer();
			}

			// ���_�̉�������j��
			RNLib::Memory().Release(&vertexOutLines);

			// �}�e���A�����ɑ΂���|�C���^���擾
			const D3DXMATERIAL* mats = (D3DXMATERIAL*)m_datas[idx]->m_matBuff->GetBufferPointer();

			// �e�N�X�`���̃������m��
			RNLib::Memory().Alloc(&m_datas[idx]->m_texIdxes, m_datas[idx]->m_matNum);
			RNLib::Memory().Alloc(&m_datas[idx]->m_texes, m_datas[idx]->m_matNum);

			// �e�N�X�`���̓ǂݍ���
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
				m_datas[idx]->m_matDatas[cntMat].col.Set(mats[cntMat].MatD3D.Diffuse);
				m_datas[idx]->m_texIdxes[cntMat] = (mats[cntMat].pTextureFilename != NULL) ? RNLib::Texture().Load(mats[cntMat].pTextureFilename)          : NONEDATA;
				m_datas[idx]->m_texes   [cntMat] = m_datas[idx]->m_texIdxes[cntMat] >= 0   ? RNLib::Texture().GetTexture(m_datas[idx]->m_texIdxes[cntMat]) : NULL;
			}
		}
	}

	return idx;
}

//========================================
// ���_�����i�[
//========================================
void _RNC_Model::StoreVtxInfo(const Matrix& modelMtx, const short& modelIdx, Vertex3DInfo** vtxInfos, const short& matIdx) {

	if (modelIdx == NONEDATA) {
		*vtxInfos = NULL;
		return;
	}

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	const DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_datas[modelIdx]->m_mesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	BYTE* vtxBuff = NULL;
	m_datas[modelIdx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

	// ���_���𐶐�
	RNLib::Memory().Alloc(vtxInfos, matIdx == NONEDATA ? m_datas[modelIdx]->m_vtxNum : m_datas[modelIdx]->m_matDatas[matIdx].vtxNum);

	// �e�N�X�`�����W�̃I�t�Z�b�g���v�Z
	const DWORD texCoordOffset = dwSizeFVF - sizeof(float) * 2;  // 2��2�����̃e�N�X�`�����W�̗v�f��

	// ���_������
	UInt vtxCount = 0;
	if (matIdx == NONEDATA) {
		for (ULong cntVtx = 0; cntVtx < m_datas[modelIdx]->m_vtxNum; cntVtx++) {
			ExecutionStoreVtxInfo(&(*vtxInfos)[cntVtx], modelMtx, cntVtx, vtxBuff, dwSizeFVF, texCoordOffset);
		}
	}
	else {
		ULong matVtxCount = 0;
		for (ULong cntVtx = 0; cntVtx < m_datas[modelIdx]->m_vtxNum; cntVtx++) {
			if (m_datas[modelIdx]->m_vtxDatas[cntVtx].isMats[matIdx]) {
				ExecutionStoreVtxInfo(&(*vtxInfos)[matVtxCount], modelMtx, cntVtx, vtxBuff, dwSizeFVF, texCoordOffset);
				matVtxCount++;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// �ݒu����
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// ���f���ԍ��������̎��A
	if (modelIdx == NONEDATA)
		return NULL;

	return RNLib::DrawMgr().PutModel(priority, mtx, isOnScreen)
		->SetModel(modelIdx);
}

//========================================
// �ݒu����(�ʒu�ƌ����Ŏw��)
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {
	
	return Put(priority, modelIdx, RNLib::Matrix().ConvPosRotToMtx(pos, rot), isOnScreen);
}

//========================================
// �ݒu����(�ʒu�ƌ����Ɗg��{���Ŏw��)
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen) {

	return Put(priority, modelIdx, RNLib::Matrix().ConvPosRotScaleToMtx(pos, rot, scale), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]���f���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ���_���̊i�[���s����
//========================================
void _RNC_Model::ExecutionStoreVtxInfo(Vertex3DInfo* vtx, const Matrix& modelMtx, const ULong& cntVtx, BYTE*& vtxBuff, const DWORD& dwSizeFVF, const DWORD& texCoordOffset) {

	vtx->pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
	vtx->nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

	// ���[���h�}�g���b�N�X���Z�o
	Matrix worldMtx = RNLib::Matrix().MultiplyMtx(RNLib::Matrix().ConvPosNorToMtx(vtx->pos, vtx->nor), modelMtx);

	vtx->worldPos = RNLib::Matrix().ConvMtxToPos(worldMtx);
	vtx->rot      = RNLib::Geometry().FindVecRot(vtx->nor);
	vtx->worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
	vtx->worldRot = RNLib::Geometry().FindVecRot(vtx->worldNor);
	vtx->texPos   = *(Pos2D*)(vtxBuff + (dwSizeFVF * cntVtx) + texCoordOffset);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �f�[�^�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Model::CData::CData() {

	m_texIdxes	   = NULL;
	m_texes		   = NULL;
	m_vtxDatas	   = NULL;
	m_vtxNum	   = 0;
	m_idxes		   = NULL;
	m_idxNum	   = 0;
	m_matBuff	   = NULL;
	m_matDatas	   = NULL;
	m_matNum	   = 0;
	m_faceMatIdxes = NULL;
	m_faceNum	   = 0;
	m_mesh		   = 0;
	m_outLineMeshs = NULL;
	m_radiusMax	   = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Model::CData::~CData() {

	Release();
}

//========================================
// �������
//========================================
void _RNC_Model::CData::Release(void) {

	// �e�N�X�`���̔j��
	RNLib::Memory().Release(&m_texIdxes);
	RNLib::Memory().Release(&m_texes);

	// ���_�f�[�^�̔j��
	for (ULong cnt = 0; cnt < m_vtxNum; cnt++) {
		RNLib::Memory().Release(&m_vtxDatas[cnt].isMats);
	}
	RNLib::Memory().Release(&m_vtxDatas);

	// �C���f�b�N�X�̔j��
	RNLib::Memory().Release(&m_idxes);

	// �}�e���A���f�[�^�𐶐�
	for (ULong cnt = 0; cnt < m_matNum; cnt++) {
		RNLib::Memory().Release(&m_matDatas[cnt].idxes);
	}
	RNLib::Memory().Release(&m_matDatas);

	// �ʖ��̃}�e���A���ԍ��̔j��
	RNLib::Memory().Release(&m_faceMatIdxes);

	// ���b�V���̔j��
	if (m_mesh != NULL) {
		m_mesh->Release();
		m_mesh = NULL;
	}

	// �֊s�����b�V���̔j��
	if (m_outLineMeshs != NULL) {
		for (int cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {
			if (m_outLineMeshs[cntOutLine] != NULL) {
				m_outLineMeshs[cntOutLine]->Release();
				m_outLineMeshs[cntOutLine] = NULL;
			}
		}
	}
	RNLib::Memory().Release(&m_outLineMeshs);

	// �}�e���A���̔j��
	if (m_matBuff != NULL) {
		m_matBuff->Release();
		m_matBuff = NULL;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �`����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
Material _RNC_Model::CDrawInfo::ms_outLineMat = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	0.0f
};

//========================================
// �R���X�g���N�^
//========================================
_RNC_Model::CDrawInfo::CDrawInfo() {

	m_mtx                  = INITMATRIX;
	m_mats                 = NULL;
	m_texes                = NULL;
	m_matNum               = 0;
	m_mesh                 = NULL;
	m_outLineMesh          = NULL;
	m_isScaling            = false;
	m_isZTest              = true;
	m_interpolationMode    = _RNC_DrawState::INTERPOLATION_MODE::NONE;
	m_isLighting           = true;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Model::CDrawInfo::~CDrawInfo() {

	RNLib::Memory().Release(&m_mats);

	// �g��{���ɕύX�����������A�������
	if (m_isScaling) {
		if (m_mesh != NULL) {
			m_mesh->Release();
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�o�^���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Model::CRegistInfo::CRegistInfo() {

	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Model::CRegistInfo::~CRegistInfo() {

}

//========================================
// �p�����[�^�[�N���A����
//========================================
void _RNC_Model::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_mtx                  = INITMATRIX;
	m_col                  = COLOR_WHITE;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = true;
	m_outLineIdx           = NONEDATA;
	m_brightnessOfEmissive = 1.0f;
	m_interpolationMode    = _RNC_DrawState::INTERPOLATION_MODE::NONE;
}

//========================================
// �`����ɕϊ�
//========================================
_RNC_Model::CDrawInfo* _RNC_Model::CRegistInfo::ConvToDrawInfo(Device& device) {

	// �`����̃������m��
	CDrawInfo* drawInfo = NULL;
	RNLib::Memory().Alloc(&drawInfo);

	// ��������
	AssignToDrawInfo(*drawInfo);

	const _RNC_Model::CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// ������
	drawInfo->m_mtx               = m_mtx;
	drawInfo->m_texes             = modelData.m_texes;
	drawInfo->m_matNum            = modelData.m_matNum;
	drawInfo->m_isZTest           = m_isZTest;
	drawInfo->m_isLighting        = m_isLighting;
	drawInfo->m_interpolationMode = m_interpolationMode;
	drawInfo->m_radiusMax         = modelData.m_radiusMax;{

		// �֊s���b�V�������Z�o
		if (m_outLineIdx == NONEDATA) {
			drawInfo->m_outLineMesh = NULL;
		}
		else {
			if (m_outLineIdx >= RNSettings::GetInfo().modelOutLineAddDistanceDelimiter) {
				m_outLineIdx = RNSettings::GetInfo().modelOutLineAddDistanceDelimiter - 1;
			}
			drawInfo->m_outLineMesh = modelData.m_outLineMeshs[m_outLineIdx];
			drawInfo->m_radiusMax += RNSettings::GetInfo().modelOutLineAddDistanceInterval * m_outLineIdx;
		}

		const Scale3D scale = RNLib::Matrix().ConvMtxToScale(m_mtx);
		float scaleMax = scale.x;
		if (scaleMax < scale.y)
			scaleMax = scale.y;
		if (scaleMax < scale.z)
			scaleMax = scale.z;

		drawInfo->m_radiusMax *= scaleMax;
	}

	//----------------------------------------
	// �}�e���A�������Z�o
	//----------------------------------------
	{
		D3DXMATERIAL* mats= (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();
		RNLib::Memory().Alloc(&drawInfo->m_mats, drawInfo->m_matNum);
		for (int cntMat = 0; cntMat < drawInfo->m_matNum; cntMat++) {
			drawInfo->m_mats[cntMat] = mats[cntMat].MatD3D;
			
			float brightness = 1.0f;
			if (drawInfo->m_mats[cntMat].Emissive.r +
				drawInfo->m_mats[cntMat].Emissive.g +
				drawInfo->m_mats[cntMat].Emissive.b > 0.0f) {
				brightness = m_brightnessOfEmissive;
			}

			// �}�e���A���̍ގ��p�����[�^��ݒ�
			const float r = ((float)m_col.r / 255) * brightness;
			const float g = ((float)m_col.g / 255) * brightness;
			const float b = ((float)m_col.b / 255) * brightness;
			const float a = ((float)m_col.a / 255);
			drawInfo->m_mats[cntMat].Diffuse.r *= r;
			drawInfo->m_mats[cntMat].Diffuse.g *= g;
			drawInfo->m_mats[cntMat].Diffuse.b *= b;
			drawInfo->m_mats[cntMat].Diffuse.a *= a;
			drawInfo->m_mats[cntMat].Emissive.r *= r;
			drawInfo->m_mats[cntMat].Emissive.g *= g;
			drawInfo->m_mats[cntMat].Emissive.b *= b;
			drawInfo->m_mats[cntMat].Emissive.a *= a;
		}
	}

	//----------------------------------------
	// ���b�V�������Z�o
	//----------------------------------------
	// �}�g���b�N�X�̊g��{�����擾���A
	const Scale3D scale = RNLib::Matrix().ConvMtxToScale(m_mtx);
	drawInfo->m_isScaling = !RNLib::Number().GetIsEq(scale.x, 1.0f, 0.01f) ? true : !RNLib::Number().GetIsEq(scale.y, 1.0f, 0.01f) ? true : !RNLib::Number().GetIsEq(scale.z, 1.0f, 0.01f);
	if (drawInfo->m_isScaling)
	{// �����g��{���ɕύX�����������A
		const DWORD fvf       = modelData.m_mesh->GetFVF();
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);
		const ULong faceNum   = modelData.m_mesh->GetNumFaces();

		// ���b�V���𕡐�����
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);

		// ���b�V����NULL�ł���΁A
		if (drawInfo->m_mesh == NULL) {
			RNLib::Memory().Release(&drawInfo);
			return NULL;
		}

		// ���_�o�b�t�@�����b�N
		BYTE* vtxBuff = NULL;
		drawInfo->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// �@�������ɉ��Z
		for (ULong cntVtx = 0; cntVtx < modelData.m_vtxNum; cntVtx++) {
			Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

			nor->x *= scale.x;
			nor->y *= scale.y;
			nor->z *= scale.z;
		}

		// ���_�o�b�t�@���A�����b�N
		drawInfo->m_mesh->UnlockVertexBuffer();
	}
	else
	{// �g��{���ɕύX���Ȃ��������A
		// �`�惁�b�V���͂��̂܂܎g�p
		drawInfo->m_mesh = modelData.m_mesh;
	}

	return drawInfo;
}

//========================================
// �N���b�s���O�J�����ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetClippingCamera(CCamera& camera) {

	if (this == NULL)
		return NULL;

	if (&camera == NULL)
		return this;

	m_clippingID = camera.GetID();

	return this;
}

//========================================
// �N���b�s���O�J�����ݒ�(ID�w��)
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetClippingCamera(const short& ID) {

	if (this == NULL)
		return NULL;

	m_clippingID = ID;

	return this;
}

//========================================
// �}�g���b�N�X��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// �F��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// ���f����ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetModel(const short& modelIdx) {

	if (this == NULL)
		return NULL;

	m_modelIdx = modelIdx;

	return this;
}

//========================================
// �e�N�X�`����ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetTex(const short& texIdx) {

	if (this == NULL)
		return NULL;

	m_texIdx = texIdx;

	return this;
}

//========================================
// Z�e�X�g��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZTest = isZTest;

	return this;
}

//========================================
// ���C�e�B���O��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// �֊s����ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetOutLineIdx(const UShort& outLineIdx) {

	if (this == NULL)
		return NULL;

	m_outLineIdx = outLineIdx;

	return this;
}

//========================================
// ���������̖��邳��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetBrightnessOfEmissive(const float& brightnessOfEmissive) {

	if (this == NULL)
		return NULL;

	m_brightnessOfEmissive = brightnessOfEmissive;

	return this;
}

//========================================
// ��ԃ��[�h��ݒ�
//========================================
_RNC_Model::CRegistInfo* _RNC_Model::CRegistInfo::SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}