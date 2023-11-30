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
//==========| [���J]���f���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CModel::CModel() {

	m_datas = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CModel::~CModel() {
	
}

//========================================
// ����������
//========================================
void CModel::Init(void) {

}

//========================================
// �I������
//========================================
void CModel::Uninit(void) {

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void CModel::Update(void) {

}

//========================================
// �������
//========================================
void CModel::Release(void) {

	// �f�[�^�̉��
	CMemory::ReleaseDouble(&m_datas, m_num);
}

//========================================
// �ǂݍ��ݏ���
//========================================
short CModel::Load(const char* loadPath, short idx) {

	const UShort oldNum = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx)) 
	{// �ǂݍ��ݐ���
		// �f�[�^�̃������Ċm��
		CMemory::ReAllocDouble(&m_datas, oldNum, m_num);

		// �f�[�^�̔j��(�ԍ��w��̏ꍇ)
		if (idxOld != NONEDATA)
			m_datas[idx]->Release();

		// X�t�@�C���̓ǂݍ���
		Device device(RNLib::Window().GetD3DDevice());
		if (FAILED(D3DXLoadMeshFromX(loadPath, D3DXMESH_SYSTEMMEM, device, NULL, &m_datas[idx]->m_matBuff, NULL, (DWORD*)&m_datas[idx]->m_matNum, &m_datas[idx]->m_mesh))) 
		{// �ǂݍ��ݎ��s
			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("���f���̓ǂݍ��݂Ɏ��s���܂����B\n%s", loadPath));

			// �f�[�^�̃��������Z�b�g
			CMemory::ReAllocDouble(&m_datas, m_num, oldNum);

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
			const ULong vtxNum    = m_datas[idx]->m_mesh->GetNumVertices();
			const ULong faceNum   = m_datas[idx]->m_mesh->GetNumFaces();

			// ���_�̉������\���̂��`
			struct VertexOutLine {
				Pos3D  totalVec = INITPOS3D;
				UShort count = 0;
			};

			// ���_�̉������𐶐�
			VertexOutLine* vertexOutLines = NULL;
			CMemory::Alloc(&vertexOutLines, vtxNum);
			for (ULong cntVtx = 0; cntVtx < vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

			{// ���_�̉��������Z�o
				// ���_�o�b�t�@�����b�N
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// �@�������ɉ��Z
				for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// ���a�̍ő�𒲂ׂ�
						const float dist = CGeometry::FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// �d�Ȃ��Ă��钸�_�ʒu�ɗ֊s�̉��Z���������Z
					for (ULong cntVtx2 = 0; cntVtx2 < vtxNum; cntVtx2++) {

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

			// �֊s���b�V���𐶐�����
			CMemory::Alloc(&m_datas[idx]->m_outLineMeshs, RNSettings::GetInfo().modelOutLineAddDistanceDelimiter);

			for (UShort cntOutLine = 0; cntOutLine < RNSettings::GetInfo().modelOutLineAddDistanceDelimiter; cntOutLine++) {

				// ���b�V���𕡐�����
				D3DXCreateMeshFVF(faceNum, vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);
				m_datas[idx]->m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &m_datas[idx]->m_outLineMeshs[cntOutLine]);

				// ���_�o�b�t�@�����b�N
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMeshs[cntOutLine]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// ���_�ʒu�ɉ��Z�ʒu�����Z
				for (UInt cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
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
			CMemory::Alloc(&m_datas[idx]->m_texIdxes, m_datas[idx]->m_matNum);
			CMemory::Alloc(&m_datas[idx]->m_texes, m_datas[idx]->m_matNum);

			// �e�N�X�`���̓ǂݍ���
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++) {
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
void CModel::StoreVtxInfo(UInt* vtxNum, Vertex3DInfo** vtxInfos, const short& modelIdx, const Matrix& modelMtx) {

	if (modelIdx == NONEDATA) {
		*vtxNum   = 0;
		*vtxInfos = NULL;
		return;
	}

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	const DWORD dwSizeFVF(D3DXGetFVFVertexSize(m_datas[modelIdx]->m_mesh->GetFVF()));

	// ���_�o�b�t�@�����b�N
	BYTE* vtxBuff = NULL;
	m_datas[modelIdx]->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

	// ���_�����擾
	*vtxNum = m_datas[modelIdx]->m_mesh->GetNumVertices();

	// ���_���𐶐�
	CMemory::Alloc(vtxInfos, *vtxNum);

	for (UInt cntVtx = 0; cntVtx < *vtxNum; cntVtx++) {
		Vertex3DInfo* vtx = &(*vtxInfos)[cntVtx];
		vtx->pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
		vtx->nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

		// ���[���h�}�g���b�N�X���Z�o
		Matrix worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosNorToMtx(vtx->pos, vtx->nor), modelMtx);

		vtx->worldPos = CMatrix::ConvMtxToPos(worldMtx);
		vtx->rot      = CGeometry::FindVecRot(vtx->nor);
		vtx->worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
		vtx->worldRot = CGeometry::FindVecRot(vtx->worldNor);
	}

	// ���_�o�b�t�@���A�����b�N
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// �ݒu����
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// ���f���ԍ��������̎��A
	if (modelIdx == NONEDATA)
		return NULL;

	return RNLib::DrawMgr().PutModel(priority, mtx, isOnScreen)
		->SetModel(modelIdx);
}

//========================================
// �ݒu����(�ʒu�ƌ����Ŏw��)
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {
	
	return Put(priority, modelIdx, CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
}

//========================================
// �ݒu����(�ʒu�ƌ����Ɗg��{���Ŏw��)
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen) {

	return Put(priority, modelIdx, CMatrix::ConvPosRotScaleToMtx(pos, rot, scale), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �f�[�^�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CModel::CData::CData() {

	m_texIdxes     = NULL;
	m_texes        = NULL;
	m_mesh         = NULL;
	m_outLineMeshs = NULL;
	m_matBuff      = NULL;
	m_matNum       = 0;
	m_radiusMax    = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CModel::CData::~CData() {

	Release();
}

//========================================
// �������
//========================================
void CModel::CData::Release(void) {

	// �e�N�X�`���̔j��
	CMemory::Release(&m_texIdxes);
	CMemory::Release(&m_texes);

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
	CMemory::Release(&m_outLineMeshs);

	// �}�e���A���̔j��
	if (m_matBuff != NULL) {
		m_matBuff->Release();
		m_matBuff = NULL;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �`����N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
Material CModel::CDrawInfo::ms_outLineMat = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	0.0f
};

//========================================
// �R���X�g���N�^
//========================================
CModel::CDrawInfo::CDrawInfo() {

	m_mtx                  = INITMATRIX;
	m_mats                 = NULL;
	m_texes                = NULL;
	m_matNum               = 0;
	m_mesh                 = NULL;
	m_outLineMesh          = NULL;
	m_isScaling            = false;
	m_isZTest              = true;
	m_isLighting           = false;
}

//========================================
// �f�X�g���N�^
//========================================
CModel::CDrawInfo::~CDrawInfo() {

	CMemory::Release(&m_mats);

	// �g��{���ɕύX�����������A�������
	if (m_isScaling) {
		if (m_mesh != NULL)
			m_mesh->Release();
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�o�^���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CModel::CRegistInfo::CRegistInfo() {

	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
CModel::CRegistInfo::~CRegistInfo() {

}

//========================================
// �p�����[�^�[�N���A����
//========================================
void CModel::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_mtx                  = INITMATRIX;
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_outLineIdx           = NONEDATA;
	m_brightnessOfEmissive = 1.0f;
}

//========================================
// �`����ɕϊ�
//========================================
CModel::CDrawInfo* CModel::CRegistInfo::ConvToDrawInfo(Device& device) {

	// �`����̃������m��
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// ��������
	AssignToDrawInfo(*drawInfo);

	const CModel::CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// ������
	drawInfo->m_mtx                  = m_mtx;
	drawInfo->m_texes                = modelData.m_texes;
	drawInfo->m_matNum               = modelData.m_matNum;
	drawInfo->m_isZTest              = m_isZTest;
	drawInfo->m_isLighting           = m_isLighting;

	//----------------------------------------
	// �}�e���A�������Z�o
	//----------------------------------------
	{
		D3DXMATERIAL* mats= (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();
		CMemory::Alloc(&drawInfo->m_mats, drawInfo->m_matNum);
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
	// �֊s���b�V�������Z�o
	//----------------------------------------
	if (m_outLineIdx == NONEDATA) {
		drawInfo->m_outLineMesh = NULL;
	}
	else {
		if (m_outLineIdx > RNSettings::GetInfo().modelOutLineAddDistanceDelimiter) {
			m_outLineIdx = RNSettings::GetInfo().modelOutLineAddDistanceDelimiter;
		}
		drawInfo->m_outLineMesh = modelData.m_outLineMeshs[m_outLineIdx];
	}

	//----------------------------------------
	// ���b�V�������Z�o
	//----------------------------------------
	// �}�g���b�N�X�̊g��{�����擾���A
	const Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);
	drawInfo->m_isScaling = !EqualFloat(scale.x, 1.0f, 0.01f) ? true : !EqualFloat(scale.y, 1.0f, 0.01f) ? true : !EqualFloat(scale.z, 1.0f, 0.01f);
	if (drawInfo->m_isScaling)
	{// �����g��{���ɕύX�����������A
		const DWORD fvf       = modelData.m_mesh->GetFVF();
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);
		const ULong vtxNum    = modelData.m_mesh->GetNumVertices();
		const ULong faceNum   = modelData.m_mesh->GetNumFaces();

		// ���b�V���𕡐�����
		D3DXCreateMeshFVF(faceNum, vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawInfo->m_mesh);

		// ���b�V����NULL�ł���΁A
		if (drawInfo->m_mesh == NULL) {
			CMemory::Release(&drawInfo);
			return NULL;
		}

		// ���_�o�b�t�@�����b�N
		BYTE* vtxBuff = NULL;
		drawInfo->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// �@�������ɉ��Z
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
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
CModel::CRegistInfo* CModel::CRegistInfo::SetClippingCamera(CCamera& camera) {

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
CModel::CRegistInfo* CModel::CRegistInfo::SetClippingCamera(const short& ID) {

	if (this == NULL)
		return NULL;

	m_clippingID = ID;

	return this;
}

//========================================
// �}�g���b�N�X��ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// �F��ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// ���f����ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetModel(const short& modelIdx) {

	if (this == NULL)
		return NULL;

	m_modelIdx = modelIdx;

	return this;
}

//========================================
// �e�N�X�`����ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetTex(const short& texIdx) {

	if (this == NULL)
		return NULL;

	m_texIdx = texIdx;

	return this;
}

//========================================
// Z�e�X�g��ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZTest = isZTest;

	return this;
}

//========================================
// ���C�e�B���O��ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// �֊s����ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetOutLineIdx(const UShort& outLineIdx) {

	if (this == NULL)
		return NULL;

	m_outLineIdx = outLineIdx;

	return this;
}

//========================================
// ���������̖��邳��ݒ�
//========================================
CModel::CRegistInfo* CModel::CRegistInfo::SetBrightnessOfEmissive(const float& brightnessOfEmissive) {

	if (this == NULL)
		return NULL;

	m_brightnessOfEmissive = brightnessOfEmissive;

	return this;
}