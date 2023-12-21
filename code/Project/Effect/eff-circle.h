//========================================
// 
// �~�`�G�t�F�N�g����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// �N���X��`
//****************************************
// �~�`�G�t�F�N�g�N���X
class CEffCircle : public CObject {
public:
	//========== [[[ �֐��錾 ]]]
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