//========================================
// 
// 標準エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]標準エフェクト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_StandardEffect3D::_RNC_StandardEffect3D() {

	m_priority = 0;
}

//========================================
// デストラクタ
//========================================
_RNC_StandardEffect3D::~_RNC_StandardEffect3D() {

}

//========================================
// 着地時の土煙生成処理
//========================================
void _RNC_StandardEffect3D::CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force) {

	CEffect3D_Cylinder::Circle circleFront = {};
	circleFront.startRadius = 0.0f;
	circleFront.endRadius   = force * 1.25f;
	circleFront.startDepth  = -force / 1.5f;
	circleFront.endDepth    = 0.0f;
	circleFront.col         = col;

	CEffect3D_Cylinder::Circle circleBack = {};
	circleBack.startRadius = 0.0f;
	circleBack.endRadius   = force;
	circleBack.startDepth  = 0.0f;
	circleBack.endDepth    = 0.0f;
	circleBack.col         = col;

	RNLib::Effect3D().CreateCylinder(m_priority, pos, rot + Rot3D(D3DX_PI_HALF, 0.0f, RNLib::Number().GetRandomFloat(1.0f) * D3DX_PI_DOUBLE), 30, _RNC_Ease::TYPE::OUT_SINE, _RNC_DrawState::ALPHA_BLEND_MODE::NORMAL, 8, circleFront, circleBack)
		->SetSpinZ(-0.01f + RNLib::Number().GetRandomFloat(1.0f) * 0.02f)
		->SetTexIdx(RNLib::DefaultData().GetTextureIdx(_RNC_DefaultData::TEXTURE::SMOKE))
		->SetTexXNum(4);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]標準エフェクト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 初期化処理
//========================================
void _RNC_StandardEffect3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_StandardEffect3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_StandardEffect3D::Update(void) {

}