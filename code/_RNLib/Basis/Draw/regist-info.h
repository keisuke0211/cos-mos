//========================================
// 
// 基底用登録情報クラスの定義
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "camera.h"
#include "draw-info.h"

//----------------------------------------
// クラス定義
//----------------------------------------
// 基底用描画情報クラス
class CRegistInfoBase {
public:
	//===== [[[ 関数宣言 ]]]
	CRegistInfoBase();
	virtual ~CRegistInfoBase();
	void AssignToDrawInfo(CDrawInfoBase& drawInfo, const CDrawInfoBase::TYPE& type);

	//===== [[[ 変数定義 ]]]
	short m_priority;
	short m_clippingID;
};
