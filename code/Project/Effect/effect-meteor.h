//========================================
// 
// 覐΃G�t�F�N�g����
// Author:KOMURO HIROMU
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"
//========================================
// �}�N����`
//========================================
#define MAX_INFO	(20)		// ���̍ő�
#define MAX_MODEL	(3)			// ���f���̍ő�
//****************************************
// �N���X
//****************************************
class CEffect_Meteor : public CObject {
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 move;		// �ړ���
		Scale3D scale;			// �傫��
		Color col;				// ���_�J���[
		int nCountMax;
	};

	// *** �֐� ***
	CEffect_Meteor();
	~CEffect_Meteor();

	/* ������	*/HRESULT Init(void);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	// -- �ݒ� ---------------------------------------------
	/* �ʒu		*/void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }


private:

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����
	static int s_nModelIdx[MAX_MODEL];	// ���f��

	// *** �ϐ� ***
	Info m_Info[MAX_INFO];		// ���ʏ��
	D3DXVECTOR3 m_pos;		// �ʒu
	int m_nCount;

};
