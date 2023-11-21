//========================================
// 
// �|���S���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�|���S���̊֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �e�N�X�`���̐ݒ菈��
//========================================
void Polygon2DAnd3D::SetTexture(Device& device, void*& tex, TEX_TYPE& texType) {

	if (tex == NULL) {
		device->SetTexture(0, NULL);
	}
	else {
		switch (texType) {
		case TEX_TYPE::IDX: {
			RNLib::Texture().Set(device, *(short*)tex);
		}break;
		case TEX_TYPE::CAMERA: {
			CCamera** camera = (CCamera**)tex;
			if (*camera == NULL) {
				device->SetTexture(0, NULL);
			}
			else {
				if (RNLib::CameraMgr().CheckDeletedCamera(*camera)) {
					device->SetTexture(0, NULL);
				}
				else {
					device->SetTexture(0, (*camera)->GetTexture());
					RNLib::DrawStateMgr().SetTextureAlphaMode(false, device);	// �e�N�X�`���̓��߂𖳌���
				}
			}
		}break;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�e�N�X�`���ݒ��񑍊��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
Polygon2DAnd3D::CSetTexInfoSum::CSetTexInfoSum() {

	m_setTex         = NULL;
	m_setTexType     = TEX_TYPE::NONE;
	m_setTexInfo     = NULL;
	m_setTexInfoType = TEX_INFO_TYPE::NONE;
	m_isTexMirrorX   = false;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
Polygon2DAnd3D::CSetTexInfoSum::~CSetTexInfoSum() {

	ClearParameter();
}

//========================================
// �p�����[�^�[�N���A����
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::ClearParameter(void) {

	CMemory::Release(&m_setTex);
	CMemory::Release(&m_setTexInfo);
	m_setTexInfoType = TEX_INFO_TYPE::NONE;
	m_isTexMirrorX   = false;
}

//========================================
// �e�N�X�`���ʒu�ɑ��
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AssignTexInfo(void*& tex, TEX_TYPE& texType, Pos2D& pos0, Pos2D& pos1, Pos2D& pos2, Pos2D& pos3) {

	// �e�N�X�`������
	texType = m_setTexType;
	if (texType == TEX_TYPE::IDX) {
		CMemory::Alloc((int**)&tex);
		*(int*)tex = m_setTex == NULL ? NONEDATA : *(int*)m_setTex;
	}
	else if (texType == TEX_TYPE::CAMERA) {
		CMemory::Alloc((CCamera***)&tex);
		*(CCamera**)tex = *(CCamera**)m_setTex;
	}

	switch (m_setTexInfoType) {
	case TEX_INFO_TYPE::NONE: {
		pos0 = Pos2D(0.0f, 0.0f);
		pos1 = Pos2D(1.0f, 0.0f);
		pos2 = Pos2D(0.0f, 1.0f);
		pos3 = Pos2D(1.0f, 1.0f);
	}break;
	case TEX_INFO_TYPE::NORMAL: {
		SetTexInfo* setTexInfo = (SetTexInfo*)m_setTexInfo;

		if ((setTexInfo->ptn == 0 && setTexInfo->ptnX == 1 && setTexInfo->ptnY == 1) || setTexInfo->ptnX <= 0 || setTexInfo->ptnY <= 0) {
			pos0 = Pos2D(0.0f, 0.0f);
			pos1 = Pos2D(1.0f, 0.0f);
			pos2 = Pos2D(0.0f, 1.0f);
			pos3 = Pos2D(1.0f, 1.0f);
		}
		else {
			const float divX   = (1.0f / setTexInfo->ptnX);
			const float divY   = (1.0f / setTexInfo->ptnY);
			const float left   = ((setTexInfo->ptn % setTexInfo->ptnX) * divX) + setTexInfo->ptnPos.x;
			const float top    = (((setTexInfo->ptn / setTexInfo->ptnX) % setTexInfo->ptnY) * divY) + setTexInfo->ptnPos.y;
			const float right  = left + divX;
			const float bottom = top + divY;

			pos0 = Pos2D(left , top   );
			pos1 = Pos2D(right, top   );
			pos2 = Pos2D(left , bottom);
			pos3 = Pos2D(right, bottom);
		}
	}break;
	case TEX_INFO_TYPE::UV: {
		SetTexUVInfo* setTexInfo = (SetTexUVInfo*)m_setTexInfo;

		pos0 = setTexInfo->poses[0];
		pos1 = setTexInfo->poses[1];
		pos2 = setTexInfo->poses[2];
		pos3 = setTexInfo->poses[3];
	}break;
	}
}

//========================================
// �e�N�X�`����ݒ�
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	AllocTex(texIdx);
	SetTex_Ptn(ptn, ptnX, ptnY, ptnPos);
}

//========================================
// �e�N�X�`����ݒ�
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex(CCamera*& camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	AllocTex(camera);
	SetTex_Ptn(ptn, ptnX, ptnY, ptnPos);
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	AllocTex(texIdx);
	SetTexUV_Pos(pos0, pos1, pos2, pos3);
}

//========================================
// �e�N�X�`����ݒ�(UV)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV(CCamera*& camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	AllocTex(camera);
	SetTexUV_Pos(pos0, pos1, pos2, pos3);
}

//========================================
// �e�N�X�`����X���]�ݒ�
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexMirrorX(const bool& isMirror) {

	m_isTexMirrorX = isMirror;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�e�N�X�`���ݒ��񑍊��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �e�N�X�`����ݒ�(�p�^�[��)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex_Ptn(const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	CMemory::Alloc((Polygon2DAnd3D::SetTexInfo**)&m_setTexInfo);
	Polygon2DAnd3D::SetTexInfo* setTexInfo = (Polygon2DAnd3D::SetTexInfo*)m_setTexInfo;
	setTexInfo->ptn    = ptn;
	setTexInfo->ptnX   = ptnX;
	setTexInfo->ptnY   = ptnY;
	setTexInfo->ptnPos = ptnPos;
	m_setTexInfoType = Polygon2DAnd3D::TEX_INFO_TYPE::NORMAL;
}

//========================================
// �e�N�X�`��UV��ݒ�(�ʒu)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV_Pos(const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	CMemory::Alloc((Polygon2DAnd3D::SetTexUVInfo**)&m_setTexInfo);
	Polygon2DAnd3D::SetTexUVInfo* setTexInfo = (Polygon2DAnd3D::SetTexUVInfo*)m_setTexInfo;
	setTexInfo->poses[0] = pos0;
	setTexInfo->poses[1] = pos1;
	setTexInfo->poses[2] = pos2;
	setTexInfo->poses[3] = pos3;
	m_setTexInfoType = Polygon2DAnd3D::TEX_INFO_TYPE::UV;
}

//========================================
// �e�N�X�`�����m��(�ԍ�)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AllocTex(const short& texIdx) {

	CMemory::Alloc((short**)&m_setTex);
	*(short*)m_setTex = texIdx;
	m_setTexType = TEX_TYPE::IDX;
}

//========================================
// �e�N�X�`�����m��(���)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AllocTex(CCamera*& camera) {

	CMemory::Alloc((CCamera***)&m_setTex);
	*(CCamera**)m_setTex = camera;
	m_setTexType = TEX_TYPE::CAMERA;
}