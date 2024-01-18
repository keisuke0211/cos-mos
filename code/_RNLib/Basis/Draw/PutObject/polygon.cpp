//========================================
// 
// ポリゴンの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]ポリゴンの関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// テクスチャの設定処理
//========================================
void Polygon2DAnd3D::SetTexture(Device& device, void* tex, const TEX_TYPE& texType) {

	if (tex == NULL) {
		device->SetTexture(0, NULL);
	}
	else {
		switch (texType) {
		case TEX_TYPE::IDX: {
			RNLib::Texture().Set(device, *(short*)tex);
			RNLib::DrawStateMgr().SetIsTextureAlpha(device, true);
		}break;
		case TEX_TYPE::CAMERA: {
			CCamera** camera = (CCamera**)tex;
			if (*camera == NULL) {
				device->SetTexture(0, NULL);
			}
			else {
				bool isFound = false; {
					CCamera* otherCamera = NULL;
					while (RNSystem::GetCameraMgr().ListLoop(&otherCamera)) {
						if (*camera == otherCamera) {
							isFound = true;
							break;
						}
					}

					if (isFound) {
						device->SetTexture(0, (*camera)->GetTexture());
					}
					else {
						device->SetTexture(0, NULL);
					}
				}
			}
			RNLib::DrawStateMgr().SetIsTextureAlpha(device, false);
		}break;
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]テクスチャ設定情報総括クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
Polygon2DAnd3D::CSetTexInfoSum::CSetTexInfoSum() {

	m_setTex         = NULL;
	m_setTexType     = TEX_TYPE::NONE;
	m_setTexInfo     = NULL;
	m_setTexInfoType = TEX_INFO_TYPE::NONE;
	m_isTexMirrorX   = false;
	m_isTexMirrorY   = false;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
Polygon2DAnd3D::CSetTexInfoSum::~CSetTexInfoSum() {

	ClearParameter();
}

//========================================
// パラメータークリア処理
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::ClearParameter(void) {

	RNLib::Memory().Release(&m_setTex);
	RNLib::Memory().Release(&m_setTexInfo);
	m_setTexInfoType = TEX_INFO_TYPE::NONE;
	m_isTexMirrorX   = false;
	m_isTexMirrorY   = false;
}

//========================================
// テクスチャ位置に代入
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AssignTexInfo(void*& tex, TEX_TYPE& texType, Pos2D& pos0, Pos2D& pos1, Pos2D& pos2, Pos2D& pos3) {

	// テクスチャを代入
	texType = m_setTexType;
	if (texType == TEX_TYPE::IDX) {
		RNLib::Memory().Alloc((int**)&tex);
		*(int*)tex = m_setTex == NULL ? NONEDATA : *(int*)m_setTex;
	}
	else if (texType == TEX_TYPE::CAMERA) {
		RNLib::Memory().Alloc((CCamera***)&tex);
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
			pos0 = Pos2D(0.0f + setTexInfo->ptnPos.x, 0.0f + setTexInfo->ptnPos.y);
			pos1 = Pos2D(1.0f + setTexInfo->ptnPos.x, 0.0f + setTexInfo->ptnPos.y);
			pos2 = Pos2D(0.0f + setTexInfo->ptnPos.x, 1.0f + setTexInfo->ptnPos.y);
			pos3 = Pos2D(1.0f + setTexInfo->ptnPos.x, 1.0f + setTexInfo->ptnPos.y);
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

	if (m_isTexMirrorX) {
		float XTemp = pos0.x;
		pos0.x = pos1.x;
		pos1.x = XTemp;

		XTemp = pos2.x;
		pos2.x = pos3.x;
		pos3.x = XTemp;
	}

	if (m_isTexMirrorY) {
		float YTemp = pos0.y;
		pos0.y = pos1.y;
		pos1.y = YTemp;

		YTemp = pos2.y;
		pos2.y = pos3.y;
		pos3.y = YTemp;
	}
}

//========================================
// テクスチャを設定
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex(const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	AllocTex(texIdx);
	SetTex_Ptn(ptn, ptnX, ptnY, ptnPos);
}

//========================================
// テクスチャを設定
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex(CCamera*& camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	AllocTex(camera);
	SetTex_Ptn(ptn, ptnX, ptnY, ptnPos);
}

//========================================
// テクスチャを設定(UV)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV(const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	AllocTex(texIdx);
	SetTexUV_Pos(pos0, pos1, pos2, pos3);
}

//========================================
// テクスチャを設定(UV)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV(CCamera*& camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	AllocTex(camera);
	SetTexUV_Pos(pos0, pos1, pos2, pos3);
}

//========================================
// テクスチャのX反転設定
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexMirrorX(const bool& isMirrorX) {

	m_isTexMirrorX = isMirrorX;
}

//========================================
// テクスチャのY反転設定
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexMirrorY(const bool& isMirrorY) {

	m_isTexMirrorY = isMirrorY;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]テクスチャ設定情報総括クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// テクスチャを設定(パターン)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTex_Ptn(const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos) {

	RNLib::Memory().Alloc((Polygon2DAnd3D::SetTexInfo**)&m_setTexInfo);
	Polygon2DAnd3D::SetTexInfo* setTexInfo = (Polygon2DAnd3D::SetTexInfo*)m_setTexInfo;
	setTexInfo->ptn    = ptn;
	setTexInfo->ptnX   = ptnX;
	setTexInfo->ptnY   = ptnY;
	setTexInfo->ptnPos = ptnPos;
	m_setTexInfoType = Polygon2DAnd3D::TEX_INFO_TYPE::NORMAL;
}

//========================================
// テクスチャUVを設定(位置)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::SetTexUV_Pos(const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3) {

	RNLib::Memory().Alloc((Polygon2DAnd3D::SetTexUVInfo**)&m_setTexInfo);
	Polygon2DAnd3D::SetTexUVInfo* setTexInfo = (Polygon2DAnd3D::SetTexUVInfo*)m_setTexInfo;
	setTexInfo->poses[0] = pos0;
	setTexInfo->poses[1] = pos1;
	setTexInfo->poses[2] = pos2;
	setTexInfo->poses[3] = pos3;
	m_setTexInfoType = Polygon2DAnd3D::TEX_INFO_TYPE::UV;
}

//========================================
// テクスチャを確保(番号)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AllocTex(const short& texIdx) {

	RNLib::Memory().Alloc((short**)&m_setTex);
	*(short*)m_setTex = texIdx;
	m_setTexType = TEX_TYPE::IDX;
}

//========================================
// テクスチャを確保(情報)
//========================================
void Polygon2DAnd3D::CSetTexInfoSum::AllocTex(CCamera*& camera) {

	RNLib::Memory().Alloc((CCamera***)&m_setTex);
	*(CCamera**)m_setTex = camera;
	m_setTexType = TEX_TYPE::CAMERA;

	(*(CCamera**)m_setTex)->SetIsDraw(true);
}