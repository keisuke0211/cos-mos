//========================================
// 
// �S�[���Q�[�g�̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

#define ETR_CNT			(16)	//�ő�J�E���g��

//****************************************
// �N���X��`
//****************************************
// ���[�h�̏��\����
class CGoalGate : public CStageObject {
public:

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = 0,	//�����Ȃ�
		GROW,		//�傫���Ȃ�
		SMALL,		//�������Ȃ�
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void SetEntry(bool bEntry) { 
		m_bEntry = bEntry; 

		if (bEntry == true){
			m_numEntry++;
			m_nCntEtrX = ETR_CNT;
			m_nCntEtrY = ETR_CNT * 0.5;
		}
	}

	static void EntrySub(void) { m_numEntry--; }

protected:

private:

	void StateUpdate(void);
	void CountRate(float *CountRateX, float *CountRateY);

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;					//���
	int m_modelIdx;					//���f���ԍ�
	int m_TexIdx[2];				//�e�N�X�`���ԍ�
	Scale3D m_scale;				//�g�k
	int		m_nCnt;					//�J�E���g
	bool	m_bEntry;				//���������ǂ���
	bool	m_bScale;				//�g�傷�邩�k�����邩
	int		m_nCntEtrX,m_nCntEtrY;	//XY�̌ʃJ�E���g
	static int m_num;
	static int m_numEntry;
};
