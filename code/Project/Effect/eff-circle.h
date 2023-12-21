//========================================
// 
// 円形エフェクト処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
// 円形エフェクトクラス
class CEffCircle : public CObject {
public:
	//========== [[[ 関数宣言 ]]]
	CEffCircle(const int& life, const Pos3D& pos, const Color& col, const float& startScale, const float& endScale);
	~CEffCircle();
	void Update(void);

private:
	//========== [[[  ]]]
	int   m_life;
	int   m_maxLife;
	Pos3D m_pos;
	Color m_col;
	float m_startScale;
	float m_endScale;
};