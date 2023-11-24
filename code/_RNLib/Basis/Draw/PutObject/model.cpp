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
			{// <<< �֊s�����b�V���̐��� & ���a�̍ő�𒲂ׂ� >>>
				// �֊s���p�ɂ�����x�ǂݍ���
				D3DXLoadMeshFromX(loadPath, D3DXMESH_SYSTEMMEM, device, NULL, NULL, NULL, NULL, &m_datas[idx]->m_outLineMesh);

				// ���_�t�H�[�}�b�g�̃T�C�Y���擾
				const DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_datas[idx]->m_outLineMesh->GetFVF());

				// ���_�o�b�t�@�����b�N
				BYTE* vtxBuff = NULL;
				m_datas[idx]->m_outLineMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

				// ���_�����擾
				const int& vtxNum = m_datas[idx]->m_outLineMesh->GetNumVertices();

				// ���_�̉������\���̂��`
				struct VertexOutLine {
					Pos3D  totalVec = INITPOS3D;
					UShort count    = 0;
				};

				// ���_�̉������𐶐�
				VertexOutLine* vertexOutLines = NULL;
				CMemory::Alloc(&vertexOutLines, vtxNum);
				for (int cntVtx = 0; cntVtx < vtxNum; vertexOutLines[cntVtx] = {}, cntVtx++);

				// �@�������ɉ��Z
				for (int cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Vector3D* pos = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

					{// ���a�̍ő�𒲂ׂ�
						const float dist = CGeometry::FindDistance(INITD3DXVECTOR3, *pos);

						if (m_datas[idx]->m_radiusMax < dist)
							m_datas[idx]->m_radiusMax = dist;
					}

					// �d�Ȃ��Ă��钸�_�ʒu�ɗ֊s�̉��Z���������Z
					for (int cntVtx2 = 0; cntVtx2 < vtxNum; cntVtx2++) {

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

				// ���_�ʒu�ɉ��Z�ʒu�����Z
				for (int cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
					Pos3D* pos = (Pos3D*)(vtxBuff + (dwSizeFVF * cntVtx));
					D3DXVec3Normalize(&vertexOutLines[cntVtx].totalVec, &vertexOutLines[cntVtx].totalVec);
					*pos += vertexOutLines[cntVtx].totalVec * RNSettings::GetInfo().modelOutLineAddDistance;
				}

				// ���_�̉�������j��
				RNLib::Memory().Release(&vertexOutLines);

				// ���_�o�b�t�@���A�����b�N
				m_datas[idx]->m_outLineMesh->UnlockVertexBuffer();
			}// <<< >>>

			// �}�e���A�����ɑ΂���|�C���^���擾
			const D3DXMATERIAL* mats((D3DXMATERIAL*)m_datas[idx]->m_matBuff->GetBufferPointer());

			// �e�N�X�`���ԍ��̃������m��
			CMemory::Alloc(&m_datas[idx]->m_texIdxs, m_datas[idx]->m_matNum);

			// �e�N�X�`���̓ǂݍ���
			for (int cntMat = 0; cntMat < m_datas[idx]->m_matNum; cntMat++)
				m_datas[idx]->m_texIdxs[cntMat] = (mats[cntMat].pTextureFilename != NULL) ? RNLib::Texture().Load(mats[cntMat].pTextureFilename) : NONEDATA;
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
		Vertex3DInfo& vtx = (*vtxInfos)[cntVtx];
		vtx.pos = *(Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx));
		vtx.nor = *(Normal3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

		// ���[���h�}�g���b�N�X���Z�o
		Matrix worldMtx = CMatrix::MultiplyMtx(CMatrix::ConvPosNorToMtx(vtx.pos, vtx.nor), modelMtx);

		vtx.worldPos = CMatrix::ConvMtxToPos(worldMtx);
		vtx.rot      = CGeometry::FindVecRot(vtx.nor);
		vtx.worldNor = Normal3D(worldMtx._31, worldMtx._32, worldMtx._33);
		vtx.worldRot = CGeometry::FindVecRot(vtx.worldNor);
	}

	// ���_�o�b�t�@���A�����b�N
	m_datas[modelIdx]->m_mesh->UnlockVertexBuffer();
}

//========================================
// �ݒu����
//========================================
CModel::CRegistInfo* CModel::Put(const UShort& priority, const short& modelIdx, const Matrix& mtx, const bool& isOnScreen) {

	// �o�^��t���łȂ����A�I��
	if (CDrawMgr::GetProcessState() != CDrawMgr::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

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

	m_texIdxs     = NULL;
	m_mesh        = NULL;
	m_outLineMesh = NULL;
	m_matBuff     = NULL;
	m_matNum      = 0;
	m_radiusMax   = 0.0f;
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

	// �e�N�X�`���ԍ��̔j��
	CMemory::Release(&m_texIdxs);

	// ���b�V���̔j��
	if (m_mesh != NULL) {
		m_mesh->Release();
		m_mesh = NULL;
	}

	// �֊s�����b�V���̔j��
	if (m_outLineMesh != NULL) {
		m_outLineMesh->Release();
		m_outLineMesh = NULL;
	}

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
	m_col                  = INITCOLOR;
	m_modelIdx             = NONEDATA;
	m_texIdx               = NONEDATA;
	m_isZTest              = true;
	m_isLighting           = false;
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
	m_distance             = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CModel::CDrawInfo::~CDrawInfo() {

}

//========================================
// �`�揈��
//========================================
void CModel::CDrawInfo::Draw(Device& device, const Matrix& viewMtx) {

	//----------------------------------------
	// ���O����
	//----------------------------------------
	// ���f���f�[�^���擾
	const CData& modelData = RNLib::Model().GetData(m_modelIdx);

	// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &m_mtx);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL* mats = (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();

	// �}�g���b�N�X�̊g��{�����擾���A
	const Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);
	const bool isScaling = !EqualFloat(scale.x,1.0f, 0.01f) ? true : !EqualFloat(scale.y, 1.0f, 0.01f) ? true : !EqualFloat(scale.z, 1.0f, 0.01f);
	LPD3DXMESH drawMesh = NULL;
	if (isScaling)
	{// �����g��{���ɕύX�����������A
		const DWORD fvf    = modelData.m_mesh->GetFVF();
		const ULong vtxNum = modelData.m_mesh->GetNumVertices();

		// ���b�V���𕡐�����
		D3DXCreateMeshFVF(modelData.m_mesh->GetNumFaces(), vtxNum, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, modelData.m_mesh->GetFVF(), device, &drawMesh);
		modelData.m_mesh->CloneMeshFVF(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, fvf, device, &drawMesh);

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		const DWORD dwSizeFVF = D3DXGetFVFVertexSize(fvf);

		// ���_�o�b�t�@�����b�N
		BYTE* vtxBuff = NULL;
		drawMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtxBuff);

		// �@�������ɉ��Z
		for (ULong cntVtx = 0; cntVtx < vtxNum; cntVtx++) {
			Vector3D* nor = (Vector3D*)(vtxBuff + (dwSizeFVF * cntVtx) + D3DXGetFVFVertexSize(D3DFVF_XYZ));

			nor->x *= scale.x;
			nor->y *= scale.y;
			nor->z *= scale.z;
		}

		// ���_�o�b�t�@���A�����b�N
		drawMesh->UnlockVertexBuffer();
	}
	else 
	{// �g��{���ɕύX���Ȃ��������A
		// �`�惁�b�V���͂��̂܂܎g�p
		drawMesh = modelData.m_mesh;
	}

	//----------------------------------------
	// �ꎞ�I�ȕ`�惂�[�h�ݒ���J�n
	//----------------------------------------
	RNLib::DrawStateMgr().StartTemporarySetMode();

	// [[[ Z�e�X�g�̐ݒ� ]]]
	RNLib::DrawStateMgr().SetZTestMode(m_isZTest, device);

	//----------------------------------------
	// �\�ʂ̕`��
	//----------------------------------------
	for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
		
		{// [[[ �}�e���A���̐ݒ� ]]]
			Color setCol = m_col;

			// ���������̖��邳���ݒ肳��Ă��鎞�A
			if (m_brightnessOfEmissive < 1.0f) {

				// �����v�f������ΓK�p
				if (0.0f < mats[cntMat].MatD3D.Emissive.r + mats[cntMat].MatD3D.Emissive.g + mats[cntMat].MatD3D.Emissive.b) {
					setCol = BrightnessToColor(setCol, m_brightnessOfEmissive);
				}
			}
			
			// �}�e���A����ݒ�
			SetMaterial(device, &mats[cntMat].MatD3D, setCol);
		}

		// [[[ �e�N�X�`���̐ݒ� ]]]
		RNLib::Texture().Set(device, (m_texIdx == NONEDATA) ? modelData.m_texIdxs[cntMat] : m_texIdx);

		// �`��
		drawMesh->DrawSubset(cntMat);
	}

	//----------------------------------------
	// ���ʂ̕`��
	//----------------------------------------
	if (m_isOutLine) {

		// �}�e���A���̐ݒ�
		device->SetMaterial(&ms_outLineMat);
		RNLib::DrawStateMgr().SetCullingMode(CDrawState::CULLING_MODE::BACK_SIDE, device);

		for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
			modelData.m_outLineMesh->DrawSubset(cntMat);
		}
	}

	//----------------------------------------
	// �ꎞ�I�ȕ`�惂�[�h�ݒ���I��
	//----------------------------------------
	RNLib::DrawStateMgr().EndTemporarySetMode(device);

	// �g��{���ɕύX�����������A�������
	if (isScaling)
		drawMesh->Release();
}

//========================================
// �}�e���A���ݒ菈��
//========================================
void CModel::CDrawInfo::SetMaterial(Device& device, Material* mat, const Color& col) {

	// �}�e���A���̍ގ��p�����[�^��ۑ�
	const D3DXCOLOR diffuseTemp  = mat->Diffuse;
	const D3DXCOLOR emissiveTemp = mat->Emissive;

	// �}�e���A���̍ގ��p�����[�^��ݒ�
	const float r = (float)col.r / 255;
	const float g = (float)col.g / 255;
	const float b = (float)col.b / 255;
	const float a = (float)col.a / 255;
	mat->Diffuse.r  = diffuseTemp .r * r;
	mat->Diffuse.g  = diffuseTemp .g * g;
	mat->Diffuse.b  = diffuseTemp .b * b;
	mat->Diffuse.a  = diffuseTemp .a * a;
	mat->Emissive.r = emissiveTemp.r * r;
	mat->Emissive.g = emissiveTemp.g * g;
	mat->Emissive.b = emissiveTemp.b * b;
	mat->Emissive.a = emissiveTemp.a * a;

	// �}�e���A���̐ݒ�
	device->SetMaterial(mat);

	// �}�e���A���̍ގ��p�����[�^�����ɖ߂�
	mat->Diffuse  = diffuseTemp;
	mat->Emissive = emissiveTemp;
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
	m_isOutLine            = false;
	m_brightnessOfEmissive = 1.0f;
}

//========================================
// �`����ɕϊ�
//========================================
CModel::CDrawInfo* CModel::CRegistInfo::ConvToDrawInfo(void) {

	// �`����̃������m��
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// ��������
	AssignToDrawInfo(*drawInfo);

	// ������
	drawInfo->m_mtx                  = m_mtx;
	drawInfo->m_col                  = m_col;
	drawInfo->m_modelIdx             = m_modelIdx;
	drawInfo->m_texIdx               = m_texIdx;
	drawInfo->m_isZTest              = m_isZTest;
	drawInfo->m_isLighting           = m_isLighting;
	drawInfo->m_isOutLine            = m_isOutLine;
	drawInfo->m_brightnessOfEmissive = m_brightnessOfEmissive;
	{// �������Z�o
		// �g��{���̍ő���Z�o
		float scaleMax = 0.0f; {
			Scale3D scale = CMatrix::ConvMtxToScale(m_mtx);

			scaleMax = scale.x;
			if (scaleMax < scale.y)
				scaleMax = scale.y;
			if (scaleMax < scale.z)
				scaleMax = scale.z;
		}
		drawInfo->m_distance = 0.0f;//CGeometry::FindDistanceToCameraPlane(CMatrix::ConvMtxToPos(m_mtx), RNLib::Camera3D()) - (RNLib::Model().GetData(m_modelIdx).m_radiusMax * scaleMax);
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
CModel::CRegistInfo* CModel::CRegistInfo::SetOutLine(const bool& isOutLine) {

	if (this == NULL)
		return NULL;

	m_isOutLine = isOutLine;

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