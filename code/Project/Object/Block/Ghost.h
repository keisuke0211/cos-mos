//========================================
// 
// �H��̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// �H��̏��\����
class CGhost : public CStageObject {
public:

	CGhost();
	virtual		~CGhost();

	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static CGhost *Create(D3DXVECTOR3 pos,int nplayernumber);	// ��������

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�
	void SetPlayer(int playernumber) { m_nPlayerNumber = playernumber; };	// �v���C���[�ԍ�

protected:

private:
	D3DXVECTOR3 m_pos;		// �ʒu
	int m_nPlayerNumber;	// �v���C���[�ԍ�
	int m_TexIdx[2];		// �e�N�X�`��
	int m_nMagCnt;			// �{���J�E���g
	Scale2D m_Scale;		// �X�P�[��

};