//========================================
// 
// �|���S��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �|���S��3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CPolygon3D::CPolygon3D() {

}

//========================================
// �f�X�g���N�^
//========================================
CPolygon3D::~CPolygon3D() {

}

//========================================
// ����������
//========================================
void CPolygon3D::Init(void) {

}

//========================================
// �I������
//========================================
void CPolygon3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CPolygon3D::Update(void) {

}

//========================================
// �ݒu����
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::Put(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutPolygon3D(priority, mtx, isOnScreen);
}

//========================================
// �ݒu����(�ʒu�ƌ����w��)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::Put(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen) {

	return Put(priority, CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �`����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
VertexBuffer CPolygon3D::CDrawInfo::ms_vtxBuff = NULL;
UShort CPolygon3D::CDrawInfo::ms_allocPower = 0;
UShort CPolygon3D::CDrawInfo::ms_allocNum   = 0;
UShort CPolygon3D::CDrawInfo::ms_idxCount   = 0;

//========================================
// [�ÓI] ���_�o�b�t�@������������
//========================================
void CPolygon3D::CDrawInfo::InitCreateVertexBuffer(void) {

	ms_allocPower = CDrawMgr::POLYGON3D_ALLOC_BASE_POWER;
	ms_allocNum   = pow(2, ms_allocPower);
	CreateVertexBuffer(ms_allocNum);
}

//========================================
// [�ÓI] ���_�o�b�t�@��������
//========================================
void CPolygon3D::CDrawInfo::CreateVertexBuffer(const UShort& num) {

	// ���_�o�b�t�@�̐���
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(
		sizeof(Vertex3D) * 4 * num,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&ms_vtxBuff,
		NULL);
}

//========================================
// [�ÓI] ���_�o�b�t�@�������
//========================================
void CPolygon3D::CDrawInfo::ReleaseVertexBuffer(void) {

	// ���_�o�b�t�@�̔j��
	if (ms_vtxBuff != NULL) {
		ms_vtxBuff->Release();
		ms_vtxBuff = NULL;
	}
}

//========================================
// �R���X�g���N�^
//========================================
CPolygon3D::CDrawInfo::CDrawInfo() {

	m_idx            = 0;
	m_mtx            = INITMATRIX;
	m_tex            = NULL;
	m_texType        = Polygon2DAnd3D::TEX_TYPE::NONE;
	m_isZTest        = true;
	m_isLighting     = true;
	m_isBillboard    = false;
	m_cullingMode    = CDrawState::CULLING_MODE::FRONT_SIDE;
	m_alphaBlendMode = CDrawState::ALPHA_BLEND_MODE::NORMAL;
	m_distance       = 0.0f;
	for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// �f�X�g���N�^
//========================================
CPolygon3D::CDrawInfo::~CDrawInfo() {

	CMemory::Release<void>(&m_tex);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �o�^���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CPolygon3D::CRegistInfo::CRegistInfo() {

	m_setVtxPosInfo = NULL;
	m_setVtxNorInfo = NULL;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
CPolygon3D::CRegistInfo::~CRegistInfo() {

	ClearParameter();
}

//========================================
// �p�����[�^�[�N���A����
//========================================
void CPolygon3D::CRegistInfo::ClearParameter(void) {

	m_idx				= 0;
	CMemory::Release(&m_setVtxPosInfo);
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NONE;
	CMemory::Release(&m_setVtxNorInfo);
	for (int cntVtx = 0; cntVtx < 4; m_vtxCols[cntVtx] = INITCOLOR, cntVtx++);
	m_setTexInfoSum.ClearParameter();
	m_isZtest			= true;
	m_isLighting		= true;
	m_isBillboard		= false;
	m_cullingMode	    = CDrawState::CULLING_MODE::FRONT_SIDE;
	m_alphaBlendMode	= CDrawState::ALPHA_BLEND_MODE::NORMAL;
}

//========================================
// �`����ɕϊ�
//========================================
CPolygon3D::CDrawInfo* CPolygon3D::CRegistInfo::ConvToDrawInfo(void) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE)
		return NULL;

	// �`����̃������m��
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// ��������
	AssignToDrawInfo(*drawInfo);

	// ������
	drawInfo->m_idx            = m_idx;
	drawInfo->m_mtx            = m_mtx;
	drawInfo->m_isZTest        = m_isZtest;
	drawInfo->m_isLighting     = m_isLighting;
	drawInfo->m_isBillboard    = m_isBillboard;
	drawInfo->m_cullingMode    = m_cullingMode;
	drawInfo->m_alphaBlendMode = m_alphaBlendMode;

	//----------------------------------------
	// ���_���̐ݒ�
	//----------------------------------------
	// [[[ �ʒu ]]]
	switch (m_setVtxPosInfoType) {
	case SET_VTX_POS_INFO_TYPE::NORMAL: {
		SetVtxPosInfo* setVtxPosInfo = (SetVtxPosInfo*)m_setVtxPosInfo;

		drawInfo->m_vtxs[0].pos = setVtxPosInfo->vtxPoses[0];
		drawInfo->m_vtxs[1].pos = setVtxPosInfo->vtxPoses[1];
		drawInfo->m_vtxs[2].pos = setVtxPosInfo->vtxPoses[2];
		drawInfo->m_vtxs[3].pos = setVtxPosInfo->vtxPoses[3];
	}break;
	case SET_VTX_POS_INFO_TYPE::SIZE: {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		const float widthHalf  = setSizeInfo->width  * 0.5f;
		const float heightHalf = setSizeInfo->height * 0.5f;

		drawInfo->m_vtxs[0].pos = D3DXVECTOR3(-widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[1].pos = D3DXVECTOR3( widthHalf,  heightHalf, 0.0f);
		drawInfo->m_vtxs[2].pos = D3DXVECTOR3(-widthHalf, -heightHalf, 0.0f);
		drawInfo->m_vtxs[3].pos = D3DXVECTOR3( widthHalf, -heightHalf, 0.0f);
	}break;
	}

	// [[[ �@�� ]]]
	if (m_setVtxNorInfo == NULL) {
		drawInfo->m_vtxs[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		drawInfo->m_vtxs[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	else {
		drawInfo->m_vtxs[0].nor = m_setVtxNorInfo->vtxNors[0];
		drawInfo->m_vtxs[1].nor = m_setVtxNorInfo->vtxNors[1];
		drawInfo->m_vtxs[2].nor = m_setVtxNorInfo->vtxNors[2];
		drawInfo->m_vtxs[3].nor = m_setVtxNorInfo->vtxNors[3];
	}

	// [[[ �F ]]]
	drawInfo->m_vtxs[0].col = D3DCOLOR_RGBA(m_vtxCols[0].r, m_vtxCols[0].g, m_vtxCols[0].b, m_vtxCols[0].a);
	drawInfo->m_vtxs[1].col = D3DCOLOR_RGBA(m_vtxCols[1].r, m_vtxCols[1].g, m_vtxCols[1].b, m_vtxCols[1].a);
	drawInfo->m_vtxs[2].col = D3DCOLOR_RGBA(m_vtxCols[2].r, m_vtxCols[2].g, m_vtxCols[2].b, m_vtxCols[2].a);
	drawInfo->m_vtxs[3].col = D3DCOLOR_RGBA(m_vtxCols[3].r, m_vtxCols[3].g, m_vtxCols[3].b, m_vtxCols[3].a);

	// [[[ UV���W ]]]
	m_setTexInfoSum.AssignTexInfo(
		drawInfo->m_tex,
		drawInfo->m_texType,
		drawInfo->m_vtxs[0].tex,
		drawInfo->m_vtxs[1].tex,
		drawInfo->m_vtxs[2].tex,
		drawInfo->m_vtxs[3].tex);

	return drawInfo;
}

//========================================
// �ԍ���ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetIdx(const short& idx) {

	if (this == NULL)
		return NULL;

	m_idx = idx;

	return this;
}

//========================================
// �}�g���b�N�X��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// �N���b�s���O�J�����ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetClippingCamera(CCamera& camera) {

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
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetClippingCamera(const short& ID) {

	if (this == NULL)
		return NULL;

	m_clippingID = ID;

	return this;
}

//========================================
// ���_�ʒu��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetVtxPos(const Pos3D pos0, const Pos3D pos1, const Pos3D pos2, const Pos3D pos3) {

	if (this == NULL)
		return NULL;

	CMemory::Alloc((SetVtxPosInfo**)&m_setVtxPosInfo);
	SetVtxPosInfo* setTexInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
	setTexInfo->vtxPoses[0] = pos0;
	setTexInfo->vtxPoses[1] = pos1;
	setTexInfo->vtxPoses[2] = pos2;
	setTexInfo->vtxPoses[3] = pos3;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NORMAL;

	return this;
}

//========================================
// �傫����ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (this == NULL)
		return NULL;
	
	CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
	SetSizeInfo* setTexInfo = (SetSizeInfo*)m_setVtxPosInfo;
	setTexInfo->width  = width;
	setTexInfo->height = height;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;

	return this;
}

//========================================
// �傫����ݒ�(�T�C�Y)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetSize(const Size2D& size) {

	return SetSize(size.x, size.y);
}

//========================================
// �@����ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetVtxNor(const Vector3D nor0, const Vector3D nor1, const Vector3D nor2, const Vector3D nor3) {

	if (this == NULL)
		return NULL;

	CMemory::Alloc(&m_setVtxNorInfo);
	m_setVtxNorInfo->vtxNors[0] = nor0;
	m_setVtxNorInfo->vtxNors[1] = nor1;
	m_setVtxNorInfo->vtxNors[2] = nor2;
	m_setVtxNorInfo->vtxNors[3] = nor3;

	return this;
}

//========================================
// �F��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_vtxCols[0] = 
	m_vtxCols[1] = 
	m_vtxCols[2] = 
	m_vtxCols[3] = col;

	return this;
}
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetVtxCol(const Color col0, const Color col1, const Color col2, const Color col3) {

	if (this == NULL)
		return NULL;

	m_vtxCols[0] = col0;
	m_vtxCols[1] = col1;
	m_vtxCols[2] = col2;
	m_vtxCols[3] = col3;

	return this;
}

//========================================
// �e�N�X�`����ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL; 

	m_setTexInfoSum.SetTex(texIdx, ptn, ptnX, ptnY, ptnPos);
	
	return this;
}

//========================================
// �e�N�X�`����ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTex(CCamera* camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(camera, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(texIdx, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexUV(CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(camera, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// �e�N�X�`����X���]�ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexMirrorX(isMirror);

	return this;
}

//========================================
// Z�e�X�g��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;
	
	return this;
}

//========================================
// ���C�e�B���O��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;
	
	return this;
}

//========================================
// �r���{�[�h��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetBillboard(const bool& isBillboard) {

	if (this == NULL)
		return NULL;

	m_isBillboard = isBillboard;
	
	return this;
}

//========================================
// �J�����O���[�h��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetCullingMode(const CDrawState::CULLING_MODE& cullingMode) {

	if (this == NULL)
		return NULL;

	m_cullingMode = cullingMode;

	return this;
}

//========================================
// �A���t�@�u�����h���[�h��ݒ�
//========================================
CPolygon3D::CRegistInfo* CPolygon3D::CRegistInfo::SetAlphaBlendMode(const CDrawState::ALPHA_BLEND_MODE& alphaBlendMode) {

	if (this == NULL)
		return NULL;

	m_alphaBlendMode = alphaBlendMode;

	return this;
}