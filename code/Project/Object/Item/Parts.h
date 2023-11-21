//========================================
// 
// �p�[�c�̏���
// Author:KOMURO HIROMU�@Hirasawa Shion
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// �N���X��`
//****************************************
// �p�[�c�̏��\����
class CParts : public CStageObject {
public:
	//========== [[[ �֐��錾 ]]]
	CParts();
	virtual        ~CParts();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SetModelIdx(int idx) { ModelIdx = idx; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void GetParts(void) { s_nNumGetParts++; }

	//---------------------------
	//�擾������`�� ON/OFF ��؂�ւ�
	//---------------------------
	void	DispSwitch(bool bGet);

	//---------------------------
	//�`�� ON/OFF ��Ԃ��擾
	//---------------------------
	bool	GetDisp(void) { return m_bDisp; }

	//---------------------------
	//�p�[�c�̑S�̐����擾
	//---------------------------
	static int GetNumAll(void) { return s_nNumAll; }

	//---------------------------
	//���P�b�g�ɏ�鏀����������������
	//---------------------------
	static bool IsReady(void) { return s_nNumAll == s_nNumGetParts ? true : false; }

private:
	//========== [[[ �ϐ��錾 ]]]
	static int s_nNumGetParts;				// �p�[�c�̎擾��
	static int s_nNumAll;					// �p�[�c���������݂��邩
	static const D3DXVECTOR3 PARTS_ADDROT;	// �p�[�c�̉�]�̈ړ���

	int m_nEffctAnimCnt;	// �G�t�F�N�g�A�j���J�E���g 
	int ModelIdx;
	bool m_bDisp;		//�@�擾�������ǂ���
}; 
