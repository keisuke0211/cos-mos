//========================================
// 
// エフェクト3D(シリンダー)の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object.h"
#include "../../Draw/draw-state.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3Dクラス
class CEffect3D_Cylinder : CObject {
public:
	//========== [[[ 関数宣言 ]]]
	CEffect3D_Cylinder();
	~CEffect3D_Cylinder();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ 変数宣言 ]]]
	short                        m_texIdx;
	Pos2D                        m_texPoses[4];
	bool                         m_isZTest;
	bool                         m_isLighting;
	bool                         m_isBillboard;
	CDrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
};