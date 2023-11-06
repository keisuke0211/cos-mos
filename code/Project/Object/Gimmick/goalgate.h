//========================================
// 
// �S�[���Q�[�g�̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// ���[�h�̏��\����
class CGoalGate : public CStageObject {
public:

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		SCALE_UP = 0,		//�傫���Ȃ�
		SCALE_DOWN,			//�������Ȃ�
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	//�ݒ�
	void   SetGoal(bool bGoal) { m_bGoal = bGoal; }				//�S�[��

	//�擾
	bool   GetGoal(void) { return m_bGoal; }					//�S�[��

protected:

private:

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;			//���
	int m_modelIdx;			//���f���ԍ�
	int m_TexIdx[2];		//�e�N�X�`���ԍ�
	bool m_bGoal;			//�S�[��������
	Scale3D m_scale;	//�g�k
	int			m_nCnt;		//�J�E���g
};
