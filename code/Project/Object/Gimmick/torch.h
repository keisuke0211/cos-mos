//========================================
// 
// �����̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once
#ifndef _TORCH_H_
#define _TORCH_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// �N���X��`
//****************************************
// �����̏��\����
class CTorch : public CStageObject {
public:
	//========== [[[ �֐��錾 ]]]
	CTorch();
	virtual        ~CTorch();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

private:

	//========== [[[ �ϐ��錾 ]]]
	float m_fBlink;		// �_��
	int	  m_nBlinkAnim;	// �_�ŃA�j���[�V����
	int ModelIdx;
};

#endif