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

	//========== [[[ �񋓌^��` ]]]
	enum class RAINBOW {
		RED = 0,	//��
		PURPLE,		//��
		BLUE,		//��
		LIGHT_BLUE,	//��
		GREEN,		//��
		YELLOW,		//��
		MAX		
	};

	//========== [[[ �֐��錾 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void SetEntry(bool bEntry);

	static void EntrySub(void) { m_numEntry--; }

protected:

private:

	void StateUpdate(void);
	void ColUpdate(void);
	void CountRate(float *CountRateX, float *CountRateY);

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;					//���
	RAINBOW m_Rainbow;				//�F���
	Color m_col;					//�F
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
