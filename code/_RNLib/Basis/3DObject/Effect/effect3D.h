//========================================
// 
// エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D_cylinder.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3Dクラス
class _RNC_Effect3D {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Effect3D                     ();
	~_RNC_Effect3D                    ();
	CEffect3D_Cylinder* CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const EASE_TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack);

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_3DObject;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};