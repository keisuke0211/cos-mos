//========================================
// 
// �R�C��UI�̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
//****************************************
// �N���X��`
//****************************************
// �f���̏��\����
class CCoinUI {
public:

	//========== [[[ �֐��錾 ]]]
	CCoinUI();
	virtual        ~CCoinUI();
	virtual void   Init(D3DXVECTOR3 pos);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	static CCoinUI *Create(D3DXVECTOR3 pos);

protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	D3DXVECTOR3 m_pos;			//�ʒu
	Scale2D		m_scale;		//�g�k
	int			m_TexIdx[2];	//�e�N�X�`���ԍ��@
	int			m_num;
};
