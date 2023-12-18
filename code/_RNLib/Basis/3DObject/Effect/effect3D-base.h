//========================================
// 
// エフェクト3D基底クラスの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../../RNmain.h"
#include "../../Mechanical/object.h"
#include "../../Draw/draw-state.h"
#include "../../Calculation/ease.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3D基底クラス
class CEffect3D_Base : CObject {
public:
	//========== [[[ 関数宣言 ]]]
	CEffect3D_Base         (const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const _RNC_Ease::TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType);
	virtual ~CEffect3D_Base();
	virtual void Update    (void) = 0;

protected:
	//========== [[[ 関数宣言 ]]]
	float CountLife(void);

	//========== [[[ 変数宣言 ]]]
	UShort                           m_priority;
	Pos3D                            m_pos;
	Rot3D                            m_rot;
	short                            m_life;
	short                            m_lifeMax;
	_RNC_Ease::TYPE                  m_easeType;
	_RNC_DrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
};