//========================================
// 
// エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]エフェクト3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CEffect3D::CEffect3D() {

}

//========================================
// デストラクタ
//========================================
CEffect3D::~CEffect3D() {

}


//========================================
// 初期化処理
//========================================
void CEffect3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CEffect3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CEffect3D::Update(void) {

}

//========================================
// シリンダーエフェクト生成処理
//========================================
CEffect3D_Cylinder* CEffect3D::CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const CEase::TYPE& easeType, const CDrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack) {

	return (new CEffect3D_Cylinder(priority, pos, rot, life, easeType, alphaBlendType))
		->SetDivisionNum(divNum)
		->SetCircleFront(circleFront)
		->SetCircleBack(circleBack);
}