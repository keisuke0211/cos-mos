//========================================
// 
// �R�C������
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"
#include "../../stage.h"

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

	//�擾������
	static int GetNum(void) { return s_Num; }
	//�v���C���Ɏ擾��������
	static int GetNumAll(void) { return s_NumAll; }

	//�S�̂𑍊����Ď擾�������i�Z�[�u�f�[�^����ǂݎ���Ă��łɉ���ς݂̐����܂�
	static int GetWholeAll(void) { return s_Num + s_NumAll + Stage::GetCoinAll(); }

private:

	int m_nID;
	int m_ModelIdx;
	Color m_color;

	static int s_Num;    //�X�e�[�W���Ŏ擾�����R�C��
	static int s_NumAll; //���܂ł̃R�C������
	static int s_CorrNumAll; //�����݂̃X�e�[�W�̃R�C������
};