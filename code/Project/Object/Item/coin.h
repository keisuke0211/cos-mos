//========================================
// 
// �R�C������
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"
//****************************************
// �N���X��`
//****************************************
// �R�C���N���X
class CCoin : public CStageObject {
public:
	//========== [[[ �֐��錾 ]]]
	CCoin();
	~CCoin();
	void Uninit(void) {}
	void Update(void);

	static void AddNumAll(void) { s_NumAll += s_Num; s_Num = 0;}

private:

	int m_ModelIdx;

	static int s_Num;		//�X�e�[�W���Ŏ擾�����R�C��
	static int s_NumAll;	//���܂ł̃R�C������
};