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
		NONE = 0,	//�����Ȃ�
		GROW,		//�傫���Ȃ�
		SMALL,		//�������Ȃ�
		DISAPP,		//������
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

protected:

private:

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;			//���
	int m_modelIdx;			//���f���ԍ�
	int m_TexIdx[2];		//�e�N�X�`���ԍ�
	Scale3D m_scale;	//�g�k
	int			m_nCnt;		//�J�E���g
};
