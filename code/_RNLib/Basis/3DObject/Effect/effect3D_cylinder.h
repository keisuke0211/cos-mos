//========================================
// 
// �G�t�F�N�g3D(�V�����_�[)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object.h"
#include "../../Draw/draw-state.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D�N���X
class CEffect3D_Cylinder : CObject {
public:
	//========== [[[ �֐��錾 ]]]
	CEffect3D_Cylinder();
	~CEffect3D_Cylinder();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ �ϐ��錾 ]]]
	short                        m_texIdx;
	Pos2D                        m_texPoses[4];
	bool                         m_isZTest;
	bool                         m_isLighting;
	bool                         m_isBillboard;
	CDrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
};