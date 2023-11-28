//========================================
// 
// �~�X�̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "../Object/stage-object.h"

//****************************************
// �N���X��`
//****************************************
// �~�X�̏��\����
class CMiss : public CStageObject{
public:

	CMiss();
	virtual		~CMiss();

	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void Delete(void) { CStageObject::Delete(); }	// �폜����
	static CMiss *Create();	// ��������

protected:

private:
	int m_TexIdx;		// �e�N�X�`��
	int m_nMagCnt;		// �{���J�E���g
	Scale2D m_Scale;	// �X�P�[��

};