//========================================
// 
// �g�����|�����̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// ���[�h�̏��\����
class CTrampoline : public CStageObject {
public:
	//========== [[[ �萔��` ]]]
	static const int MAX_COUNT = 10;

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = -1,	//����Ă��Ȃ�
		UP_LAND,	//��ɒ��n
		DOWN_LAND,	//���ɒ��n
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CTrampoline();
	virtual        ~CTrampoline();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void   SetState(STATE state) { m_state = state; }			//��ސݒ�
	STATE  GetState(void) { return m_state; }					//��ގ擾
	void SetCount(int nCount) { m_nCnt = nCount; }

	D3DXVECTOR3 GetSpringPos(int nIdx);	// �΂˂̈ʒu�擾
	void SetSpringForce(float fForce) { m_fSpringForce = fForce; }
	float GetSpringForce(void) { return m_fSpringForce; }

private:
	//========== [[[ �֐��錾 ]]]
	void PutModel(void);

	//========== [[[ �ϐ��錾 ]]]
	STATE       m_state;		// ���
	D3DXVECTOR3 *m_pSpringPos;	// �΂˂̈ʒu
	float       m_fSpringForce;
	int         m_modelIdx[4];	// ���f���ԍ�
	float		m_fJamp;		// ����
	int			m_nCnt;			// �J�E���g
};