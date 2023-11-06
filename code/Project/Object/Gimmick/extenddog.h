//========================================
// 
// �L�т錢�̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// ���[�h�̏��\����
class CExtenddog : public CStageObject {
public:

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = 0,		//����Ă��Ȃ�
		UP_LAND,		//��ɒ��n
		DOWN_LAND,		//���ɒ��n
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CExtenddog();
	virtual        ~CExtenddog();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void   SetState(STATE state) { m_state = state; }			//��ސݒ�
	void   SetScale(D3DXVECTOR3 scale) { m_scale = scale; }		//�g�k�ݒ�
	void   SetShrink(bool shrink) { m_bShrink = shrink; }		//�L�k�ݒ�
	void   SetHead(float headposx) { m_fHeadposx = headposx; }	//���̈ʒu�ݒ�
	void   SetHip(float hipposx) { m_fHipposx = hipposx; }		//���K�̈ʒu�ݒ�
	STATE  GetState(void) { return m_state; }					//��ގ擾

protected:

private:

	//========== [[[ �֐��錾 ]]]
	void   Collision(void);	//�����蔻�菈��

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;			// ���
	int m_modelIdx[5];		// ���f���ԍ�
	D3DXVECTOR3 m_scale;	// �傫��
	bool		m_bLand;	// ���n�����H
	bool		m_bShrink;	// �k��?
	int			m_nCntShrink;		// �k�ރJ�E���g
	float		m_fcurrenty;		// ���݂�y���W
	float		m_fHeadposx;		// ���̈ʒu
	float		m_fHipposx;			// ���K�̈ʒu
};