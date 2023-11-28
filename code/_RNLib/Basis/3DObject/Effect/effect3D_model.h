//========================================
// 
// エフェクト3D(モデル)の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D-base.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3D(モデル)クラス
class CEffect3D_Model : CEffect3D_Base {
public:
	//========== [[[ 関数宣言 ]]]
	CEffect3D_Model(const short& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const CEase::TYPE& easeType, const CDrawState::ALPHA_BLEND_MODE& alphaBlendType);
	~CEffect3D_Model();
	void Update(void);

private:
	//========== [[[ 変数宣言 ]]]

};