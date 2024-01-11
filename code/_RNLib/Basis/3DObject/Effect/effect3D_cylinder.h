//========================================
// 
// �G�t�F�N�g3D(�V�����_�[)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D-base.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D�N���X
class CEffect3D_Cylinder : CEffect3D_Base {
public:
	//========== [[[ �\���̐錾 ]]]
	struct Circle {
		float startRadius = 0.0f;
		float endRadius   = 0.0f;
		float startDepth  = 0.0f;
		float endDepth    = 0.0f;
		Color col         = COLOR_WHITE;
	};

	//========== [[[ �֐��錾 ]]]
	CEffect3D_Cylinder                     (const short& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const EASE_TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType);
	~CEffect3D_Cylinder                    ();
	CEffect3D_Cylinder* SetFadeOutStartRate(const float& fadeOutStartRate)    { m_fadeOutStartRate = fadeOutStartRate; return this; }
	CEffect3D_Cylinder* SetSpinZ           (const float& spinZ)               { m_spinZ			   = spinZ			 ; return this; }
	CEffect3D_Cylinder* SetDivisionNum     (const UShort& divisionNum)        { m_divisionNum	   = divisionNum	 ; return this; }
	CEffect3D_Cylinder* SetTexIdx          (const short& texIdx)              { m_texIdx		   = texIdx			 ; return this; }
	CEffect3D_Cylinder* SetTexXNum         (const short& texXNum)             { m_texXNum          = texXNum         ; return this; }
	CEffect3D_Cylinder* SetCircleFront     (const Circle& circleFront)        { m_circleFront      = circleFront     ; return this; }
	CEffect3D_Cylinder* SetCircleBack      (const Circle& circleBack)         { m_circleBack       = circleBack		 ; return this; }

private:
	//========== [[[ �֐��錾 ]]]
	void Update(void);

	//========== [[[ �ϐ��錾 ]]]
	float  m_fadeOutStartRate;
	float  m_spinZ;
	UShort m_divisionNum;
	short  m_texIdx;
	UShort m_texXNum;
	Circle m_circleFront;
	Circle m_circleBack;
};