//========================================
// 
// エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]エフェクト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Effect3D::_RNC_Effect3D() {

}

//========================================
// デストラクタ
//========================================
_RNC_Effect3D::~_RNC_Effect3D() {

}

//========================================
// シリンダーエフェクト生成処理
//========================================
CEffect3D_Cylinder* _RNC_Effect3D::CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const _RNC_Ease::TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack) {

	return (new CEffect3D_Cylinder(priority, pos, rot, life, easeType, alphaBlendType))
		->SetDivisionNum(divNum)
		->SetCircleFront(circleFront)
		->SetCircleBack(circleBack);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]エフェクト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 初期化処理
//========================================
void _RNC_Effect3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Effect3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Effect3D::Update(void) {

}