//========================================
// 
// 標準エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]標準エフェクト3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CStandardEffect3D::CStandardEffect3D() {

	m_priority = 0;
}

//========================================
// デストラクタ
//========================================
CStandardEffect3D::~CStandardEffect3D() {

}


//========================================
// 初期化処理
//========================================
void CStandardEffect3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CStandardEffect3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CStandardEffect3D::Update(void) {

}

//========================================
// 着地時の土煙生成処理
//========================================
void CStandardEffect3D::CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force) {

	CEffect3D_Cylinder::Circle circleFront = {};
	circleFront.startRadius = 0.0f;
	circleFront.endRadius   = force * 1.25f;
	circleFront.startDepth  = -force / 2.5f;
	circleFront.endDepth    = -force / 1.5f;
	circleFront.col         = col;

	CEffect3D_Cylinder::Circle circleBack = {};
	circleBack.startRadius = 0.0f;
	circleBack.endRadius   = force;
	circleBack.startDepth  = 0.0f;
	circleBack.endDepth    = 0.0f;
	circleBack.col         = col;

	RNLib::Effect3D().CreateCylinder(m_priority, pos, rot + Rot3D(D3DX_PI_HALF, 0.0f, fRand() * D3DX_PI_DOUBLE), 30, CEase::TYPE::OUT_SINE, CDrawState::ALPHA_BLEND_MODE::NORMAL, 8, circleFront, circleBack)
		->SetSpinZ(-0.01f + fRand() * 0.02f)
		->SetTexIdx(RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::SMOKE))
		->SetTexXNum(4);
}