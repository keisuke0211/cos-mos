//========================================
// 
// 標準エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D_cylinder.h"

//****************************************
// クラス定義
//****************************************
// 標準エフェクト3Dクラス
class CStandardEffect3D {
public:
	//========== [[[ 関数宣言 ]]]
	CStandardEffect3D();
	~CStandardEffect3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetPriority(const UShort& priority) { m_priority = priority; }
	void CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force);

private:
	//========== [[[ 変数宣言 ]]]
	UShort m_priority;
};