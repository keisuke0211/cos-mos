//========================================
// 
// ���P�b�g�p�[�cUI�̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
//****************************************
// �N���X��`
//****************************************
// �f���̏��\����
class CRocketPartsUI{
public:

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = 0,
		OBTAIN,
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CRocketPartsUI();
	virtual        ~CRocketPartsUI();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static CRocketPartsUI *Create(void);

	static void SetRocketstg(bool bRocketStg) { m_bRocketStg = bRocketStg; }
	static bool GetRocketstg(void) { return m_bRocketStg; }

protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	D3DXVECTOR3 m_pos;			// �ʒu
	Scale2D		m_scale;		// �g�k
	STATE		m_state[2];		// ���
	int			m_TexIdx;
	int			m_colorA;
	int			m_num;
	static bool	m_bRocketStg;	//���P�b�g�̂���X�e�[�W�H
};