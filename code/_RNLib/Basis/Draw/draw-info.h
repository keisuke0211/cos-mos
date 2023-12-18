//========================================
// 
// 基底用描画情報クラスの定義
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "camera.h"

//----------------------------------------
// クラス定義
//----------------------------------------
// 基底用描画情報クラス
class CDrawInfoBase {
public:
	//===== [[[ 関数宣言 ]]]
	CDrawInfoBase         ();
	virtual ~CDrawInfoBase();
	virtual void Draw     (Device& device, const Matrix& viewMtx) {}

	//===== [[[ 変数定義 ]]]
	short m_clippingID;
};
