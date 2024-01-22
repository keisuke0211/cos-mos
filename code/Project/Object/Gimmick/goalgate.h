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
		WHITE,
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
	void LeaveDoor(void) { m_bLeave = true; }
	void SetStartGate(bool bStartGate) { m_bStartGate = bStartGate; }

	bool GetStartGate(void) { return m_bStartGate; }
	static void EntrySub(void) { s_numEntry--; }
	static void ResetEtr(void) { s_numEntry = 0; }

protected:

private:

	void StateUpdate(void);
	void ColUpdate(void);
	void CountRate(float *CountRateX, float *CountRateY,float *CountRateZ);
	void EscapeGuide(void);

	//========== [[[ �ϐ��錾 ]]]
	CDoll3D* m_doll;
	STATE m_state;							//���
	RAINBOW m_Rainbow;						//�F���
	Color m_col;							//�F
	Color m_RainbowCol[(int)RAINBOW::MAX];	//�F
	Scale3D m_scale;						//�g�k
	CCamera m_camera;						//�J����
	static int s_modelIdx;					//���f���ԍ�
	static int s_TexIdx[3];					//�e�N�X�`���ԍ�
	static int s_num;
	static int s_numEntry;

	int		m_nCnt;							//�J�E���g
	int		m_nCntEtrX, m_nCntEtrY;			//XY�̌ʃJ�E���g
	int		m_MotionIdx;
	int		m_MotionOldIdx;

	bool	m_bEntry;						//���������ǂ���
	bool	m_bScale;						//�g�傷�邩�k�����邩
	bool	m_bStartGate;
	bool	m_bCloseGate;
	bool	m_bLeave;						//�S�[������o����

	static const int ESCAPE_GUIDE_POPUP_TIME = 60;
	static int s_nEscapeGuideTexID; // �E�o�A�C�R���̃e�N�X�`��ID
	int m_nEntryNo;      // �G���g���[No
	int m_nEntryCounter; // �����Ă���̎��ԃJ�E���^�[
	UShort m_GuideAlpha; // �K�C�hUI�̃��l
};