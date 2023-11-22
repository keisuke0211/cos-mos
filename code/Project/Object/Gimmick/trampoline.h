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
	//�g�����|�����̏�ԗ�
	enum class STATE {
		NONE = -1,	//����Ă��Ȃ�
		UP_LAND,	//��ɒ��n
		DOWN_LAND,	//���ɒ��n
		MAX,
	};

	//�p�[�c����
	enum Parts
	{
		Parts_BASE = 0,	//�y��
		Parts_MASH,		//�L�m�R
		Parts_SPRING,	//�΂�
		Parts_EYE,		//�ڋ�
		Parts_MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CTrampoline();
	virtual        ~CTrampoline();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void   SetState(STATE state) { m_state = state; }	//��ސݒ�
	STATE  GetState(void) { return m_state; }			//��ގ擾
	void SetCount(int nCount) { m_nSpringCounter = nCount; }

	void SetSpringForce(float fForce) { m_fSpringForce = fForce; }
	float GetSpringForce(void) { return m_fSpringForce; }

	//---------------------------------------------
	//�o�E���h�ݒ�
	//Author:HIRASAWA SHION
	//---------------------------
	//�����P�@state�F �ǂ����̕����ɒ��n������
	//�����Q�@fForce�F�ǂꂾ���o�E���h�����邩�i�ō��x���W�����j
	//---------------------------------------------
	void SetBound(STATE state, float fForce) { SetState(state); SetSpringForce(fForce); SetCount(MAX_COUNT); }

private:
	//========== [[[ �֐��錾 ]]]
	void PutModel(void);

	//========== [[[ �ϐ��錾 ]]]
	STATE       m_state;		// ���
	D3DXVECTOR3 *m_pSpringPos;	// �΂˂̈ʒu
	float       m_fSpringForce;
	float		m_fJamp;		// ����
	int         m_modelIdx[Parts_MAX];	// ���f���ԍ�
	int			m_nSpringCounter;		// �΂˂̃J�E���^�[
};