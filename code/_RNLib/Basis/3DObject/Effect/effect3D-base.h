//========================================
// 
// �G�t�F�N�g3D���N���X�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../../RNmain.h"
#include "../../Mechanical/object.h"
#include "../../Draw/draw-state.h"
#include "../../Calculation/ease.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D���N���X
class CEffect3D_Base : CObject {
public:
	//========== [[[ �֐��錾 ]]]
	CEffect3D_Base         (const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const _RNC_Ease::TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType);
	virtual ~CEffect3D_Base();
	virtual void Update    (void) = 0;

protected:
	//========== [[[ �֐��錾 ]]]
	float CountLife(void);

	//========== [[[ �ϐ��錾 ]]]
	UShort                           m_priority;
	Pos3D                            m_pos;
	Rot3D                            m_rot;
	short                            m_life;
	short                            m_lifeMax;
	_RNC_Ease::TYPE                  m_easeType;
	_RNC_DrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
};