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
	virtual void   Init(D3DXVECTOR3 pos,D3DXVECTOR3 rot, Scale2D scale,bool bframe);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScale(Scale2D scale) { m_scale = scale; }
	void SetbFrame(bool bframe) { m_bFrame = bframe; }

	static CCoinUI *Create(D3DXVECTOR3 pos,Scale2D scale,bool bframe,D3DXVECTOR3 rot = INITD3DXVECTOR3);

protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//�ʒu
	Scale2D		m_scale;		//�g�k
	int			m_TexIdx[2];	//�e�N�X�`���ԍ��@
	int			m_num;
	bool		m_bFrame;		//�t���[�������邩�H
};
