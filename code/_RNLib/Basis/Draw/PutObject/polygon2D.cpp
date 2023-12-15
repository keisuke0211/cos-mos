//========================================
// 
// �|���S��2D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�|���S��2D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]���_2D�̍��W�ݒ�
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos2D& pos, const Angle& angle, const float& width, const float& height) {

	// �Ίp���̒����ƌ���
	const float length          = sqrtf((width * width) + (height * height)) * 0.5f;
	const float vtxAngle        = atan2f(width, height);
	const float inverseVtxAngle = D3DX_PI - vtxAngle;

	// ���_���W��ݒ�
	{
		float resultAngle = angle - inverseVtxAngle;
		vtxs[0].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[0].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[0].pos.z = 0.0f;
		resultAngle = angle + inverseVtxAngle;
		vtxs[1].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[1].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[1].pos.z = 0.0f;
		resultAngle = angle - vtxAngle;
		vtxs[2].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[2].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[2].pos.z = 0.0f;
		resultAngle = angle + vtxAngle;
		vtxs[3].pos.x = pos.x + sinf(resultAngle) * length;
		vtxs[3].pos.y = pos.y + cosf(resultAngle) * length;
		vtxs[3].pos.z = 0.0f;
	}

	// �𑜓x��K�p
	ApplyResolution(vtxs);
}

//========================================
// [�ÓI]���_2D�̍��W�ݒ�
//========================================
void CPolygon2D::SetVtxPos(Vertex2D* vtxs, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {
	
	vtxs[0].pos = Pos3D(pos0.x, pos0.y, 0.0f);
	vtxs[1].pos = Pos3D(pos1.x, pos1.y, 0.0f);
	vtxs[2].pos = Pos3D(pos2.x, pos2.y, 0.0f);
	vtxs[3].pos = Pos3D(pos3.x, pos3.y, 0.0f);

	// �𑜓x��K�p
	ApplyResolution(vtxs);
}

//========================================
// [�ÓI]���_2D�̍��W�ݒ�
//========================================
void CPolygon2D::SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos2D& pos, const float& width, const float& height) {

	vtxs[0].pos = Pos3D(pos.x        , pos.y         , 0.0f);
	vtxs[1].pos = Pos3D(pos.x + width, pos.y         , 0.0f);
	vtxs[2].pos = Pos3D(pos.x        , pos.y + height, 0.0f);
	vtxs[3].pos = Pos3D(pos.x + width, pos.y + height, 0.0f);

	// �𑜓x��K�p
	ApplyResolution(vtxs);
}

//========================================
// [�ÓI]���_2D�ɉ𑜓x��K�p
//========================================
void CPolygon2D::ApplyResolution(Vertex2D* vtxs) {

	float resolution = RNLib::Window().GetResolution();
	vtxs[0].pos *= resolution;
	vtxs[1].pos *= resolution;
	vtxs[2].pos *= resolution;
	vtxs[3].pos *= resolution;
}

//========================================
// [�ÓI]���_2D�̍��W�ϊ��p�W���ݒ�
//========================================
void CPolygon2D::SetVtxRHW(Vertex2D *pVtx) {

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// [�ÓI]���_2D�̐F�ݒ�
//========================================
void CPolygon2D::SetVtxCol(Vertex2D *pVtx, const Color& col) {

	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// [�ÓI]���_2D�̃e�N�X�`�����W�ݒ�
//========================================
void CPolygon2D::SetVtxTex_Cut(Vertex2D* vtxs, const Pos2D& cutPos, const float& size) {

	SetVtxTex_Cut(vtxs, cutPos, size, size);
}

//========================================
// [�ÓI]���_2D�̃e�N�X�`�����W�ݒ�
//========================================
void CPolygon2D::SetVtxTex(Vertex2D* vtxs) {

	vtxs[0].tex = Pos2D(0.0f, 0.0f);
	vtxs[1].tex = Pos2D(1.0f, 0.0f);
	vtxs[2].tex = Pos2D(0.0f, 1.0f);
	vtxs[3].tex = Pos2D(1.0f, 1.0f);
}

//========================================
// [�ÓI]���_2D�̃e�N�X�`�����W�ݒ�
//========================================
void CPolygon2D::SetVtxTex_Cut(Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height) {

	const float widthHalf  = width  * 0.5f;
	const float heightHalf = height * 0.5f;
	const float left       = cutPos.x - widthHalf;
	const float right      = cutPos.x + widthHalf;
	const float top        = cutPos.y - heightHalf;
	const float bottom     = cutPos.y + heightHalf;

	vtxs[0].tex = Pos2D(left , top   );
	vtxs[1].tex = Pos2D(right, top   );
	vtxs[2].tex = Pos2D(left , bottom);
	vtxs[3].tex = Pos2D(right, bottom);
}

//========================================
// �R���X�g���N�^
//========================================
CPolygon2D::CPolygon2D() {

}

//========================================
// �f�X�g���N�^
//========================================
CPolygon2D::~CPolygon2D() {

}

//========================================
// ����������
//========================================
void CPolygon2D::Init(void) {

}

//========================================
// �I������
//========================================
void CPolygon2D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CPolygon2D::Update(void) {

}

//========================================
// �ݒu����
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const Pos2D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen)
		->SetPos(pos)
		->SetAngle(angle);
}

//========================================
// �ݒu����
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const Pos3D& pos, const Angle& angle, const bool& isOnScreen) {

	return Put(priority, Pos2D(pos.x, pos.y), angle, isOnScreen);
}

//========================================
// �ݒu����
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::Put(const UShort& priority, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�`����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
VertexBuffer CPolygon2D::CDrawInfo::ms_vtxBuff = NULL;
UShort CPolygon2D::CDrawInfo::ms_allocPower = 0;
UShort CPolygon2D::CDrawInfo::ms_allocNum   = 0;
UShort CPolygon2D::CDrawInfo::ms_idxCount   = 0;

//========================================
// [�ÓI] ���_�o�b�t�@������������
//========================================
void CPolygon2D::CDrawInfo::InitCreateVertexBuffer(void) {

	ms_allocPower = CDrawMgr::POLYGON2D_ALLOC_BASE_POWER;
	ms_allocNum   = pow(2, ms_allocPower);
	CreateVertexBuffer(ms_allocNum);
}

//========================================
// [�ÓI]���_�o�b�t�@��������
//========================================
void CPolygon2D::CDrawInfo::CreateVertexBuffer(const UShort& num) {

	// ���_�o�b�t�@�̐���
	RNLib::Window().GetD3DDevice()->CreateVertexBuffer(sizeof(Vertex2D) * 4 * num, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &ms_vtxBuff, NULL);
}

//========================================
// [�ÓI]���_�o�b�t�@�������
//========================================
void CPolygon2D::CDrawInfo::ReleaseVertexBuffer(void) {

	// ���_�o�b�t�@�̔j��
	if (ms_vtxBuff != NULL) {
		ms_vtxBuff->Release();
		ms_vtxBuff = NULL;
	}
}

//========================================
// �R���X�g���N�^
//========================================
CPolygon2D::CDrawInfo::CDrawInfo() {

	m_idx               = 0;
	m_tex               = NULL;
	m_texType           = Polygon2DAnd3D::TEX_TYPE::NONE;
	m_interpolationMode = CDrawState::INTERPOLATION_MODE::NONE;
	for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
		m_vtxs[cntVtx] = {};
	}
}

//========================================
// �f�X�g���N�^
//========================================
CPolygon2D::CDrawInfo::~CDrawInfo() {

	CMemory::Release(&m_tex);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�o�^���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CPolygon2D::CRegistInfo::CRegistInfo() {

	m_setVtxPosInfo = NULL;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
CPolygon2D::CRegistInfo::~CRegistInfo() {

	ClearParameter();
}

//========================================
// �p�����[�^�[�̃N���A����
//========================================
void CPolygon2D::CRegistInfo::ClearParameter(void) {

	CRegistInfoBase::ClearParameter();
	m_idx               = 0;
	CMemory::Release(&m_setVtxPosInfo);
	m_pos               = INITPOS2D;
	m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NONE;
	m_interpolationMode = CDrawState::INTERPOLATION_MODE::NONE;
	for (int cntVtx = 0; cntVtx < 4; m_vtxCols[cntVtx] = INITCOLOR, cntVtx++);
	m_setTexInfoSum.ClearParameter();
}

//========================================
// �`����ɕϊ�
//========================================
CPolygon2D::CDrawInfo* CPolygon2D::CRegistInfo::ConvToDrawInfo(void) {

	// �`����̃������m��
	CDrawInfo* drawInfo = NULL;
	CMemory::Alloc(&drawInfo);

	// ��������
	AssignToDrawInfo(*drawInfo);

	// ������
	drawInfo->m_idx               = m_idx;
	drawInfo->m_interpolationMode = m_interpolationMode;

	//----------------------------------------
	// ���_���̐ݒ�
	//----------------------------------------
	// [[[ �ʒu ]]]
	switch (m_setVtxPosInfoType) {
	case SET_VTX_POS_INFO_TYPE::NORMAL: {
		SetVtxPosInfo* setVtxPosInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
		CPolygon2D::SetVtxPos(drawInfo->m_vtxs, 
			setVtxPosInfo->vtxPoses[0],
			setVtxPosInfo->vtxPoses[1],
			setVtxPosInfo->vtxPoses[2],
			setVtxPosInfo->vtxPoses[3]);
	}break;
	case SET_VTX_POS_INFO_TYPE::SIZE: {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		CPolygon2D::SetVtxPos(drawInfo->m_vtxs, Pos2D(m_pos.x, m_pos.y), setSizeInfo->angle, setSizeInfo->width, setSizeInfo->height);
	}break;
	}

	// [[[ ���W�ϊ��W�� ]]]
	RNLib::Polygon2D().SetVtxRHW(drawInfo->m_vtxs);

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetIdx(const short& idx) {

	m_idx = idx;

	return this;
}

//========================================
// �ʒu��ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetPos(const Pos2D& pos) {

	m_pos = pos;

	return this;
}

//========================================
// �p�x��ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetAngle(const Angle& angle) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::SIZE) {
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->angle = angle;
	}
	else if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->width  = 0.0f;
		setSizeInfo->height = 0.0f;
		setSizeInfo->angle  = angle;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;
	}
	else {
		assert(false);	// �����ɕʌ`���Œ��_��񂪐ݒ肳��Ă���
	}

	return this;
}

//========================================
// ���_�ʒu��ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetVtxPos(const Pos2D pos0, const Pos2D pos1, const Pos2D pos2, const Pos2D pos3) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetVtxPosInfo**)&m_setVtxPosInfo);
		SetVtxPosInfo* setTexInfo = (SetVtxPosInfo*)m_setVtxPosInfo;
		setTexInfo->vtxPoses[0] = pos0;
		setTexInfo->vtxPoses[1] = pos1;
		setTexInfo->vtxPoses[2] = pos2;
		setTexInfo->vtxPoses[3] = pos3;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::NORMAL;
	}
	else {
		assert(false);	// �����ɕʌ`���Œ��_��񂪐ݒ肳��Ă���
	}

	return this;
}

//========================================
// �傫����ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::SIZE) {
		SetSizeInfo* setTexInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setTexInfo->width  = width;
		setTexInfo->height = height;
	}
	else if (m_setVtxPosInfoType == SET_VTX_POS_INFO_TYPE::NONE) {
		CMemory::Alloc((SetSizeInfo**)&m_setVtxPosInfo);
		SetSizeInfo* setSizeInfo = (SetSizeInfo*)m_setVtxPosInfo;
		setSizeInfo->width  = width;
		setSizeInfo->height = height;
		setSizeInfo->angle  = 0.0f;
		m_setVtxPosInfoType = SET_VTX_POS_INFO_TYPE::SIZE;
	}
	else {
		assert(false);	// �����ɕʌ`���Œ��_��񂪐ݒ肳��Ă���
	}
	
	return this;
}

//========================================
// �F��ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_vtxCols[0] =
	m_vtxCols[1] =
	m_vtxCols[2] =
	m_vtxCols[3] = col;

	return this;
}
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetVtxCol(const Color col0, const Color col1, const Color col2, const Color col3) {

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
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(texIdx, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// �e�N�X�`����ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTex(CCamera* camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTex(camera, ptn, ptnX, ptnY, ptnPos);

	return this;
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(texIdx, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexUV(CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexUV(camera, pos0, pos1, pos2, pos3);

	return this;
}

//========================================
// �e�N�X�`����X���]�ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetTexMirrorX(const bool& isMirror) {

	if (this == NULL)
		return NULL;

	m_setTexInfoSum.SetTexMirrorX(isMirror);

	return this;
}

//========================================
// ��ԃ��[�h��ݒ�
//========================================
CPolygon2D::CRegistInfo* CPolygon2D::CRegistInfo::SetInterpolationMode(const CDrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}