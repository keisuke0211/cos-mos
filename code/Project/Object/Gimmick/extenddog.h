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

	static const int MAX_COUNT = 40;	// �ő�J�E���g��
	static const int HIP_POS = 7;		// �K�̈ʒu

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = 0,		//����Ă��Ȃ�
		RETURN,			//�߂�
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
	void   SetHead(D3DXVECTOR3 pos) { m_HeadPos = pos; }		//���̈ʒu�ݒ�
	void   SetHip(D3DXVECTOR3 pos) { m_HipPos = pos; }			//���K�̈ʒu�ݒ�
	void   SetHeadHeight(int height) { m_nHeight = height; }	//���̍���
	void   SetReturn(bool bretrun) { m_bInversion = bretrun; }	//���]�̐ݒ�
	STATE  GetState(void) { return m_state; }					//��ގ擾
	D3DXVECTOR3 GetHeadPos(void) { return m_HeadPos; }
	D3DXVECTOR3 GetHeadPosOid(void) { return m_HeadPosOid; }
	D3DXVECTOR3 GetBodyPos(void) { return m_BodyPos; }
	D3DXVECTOR3 GetHipPos(void) { return m_HipPos; }

protected:

private:

	//========== [[[ �ϐ��錾 ]]]
	STATE m_state;				// ���
	int m_modelIdx[6];			// ���f���ԍ�
	D3DXVECTOR3 m_scale;		// �傫��
	bool		m_bLand;		// ���n�����H
	bool		m_bShrink;		// �k��?
	bool		m_bInversion;	// ���]?
	int			m_nCntShrink;	// �k�ރJ�E���g
	float		m_fcurrenty;	// ���݂�y���W
	int			m_nHeight;		// ����

	D3DXVECTOR3 m_HeadPos;		// ���̈ʒu
	D3DXVECTOR3 m_HeadPosOid;	// ���̉ߋ��ʒu
	D3DXVECTOR3 m_BodyPos;		// �̂̈ʒu
	D3DXVECTOR3 m_HipPos;		// �K�̈ʒu
};