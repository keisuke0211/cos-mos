//========================================
// 
// �}�O�}�u���b�N����
// Author:RYUKI FUJIWARA
// 
//========================================
// *** magma-block.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CMagmaBlock : public CStageObject {
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 move;		// �ړ���
		int nType;				// ���
		short nModelIdx;		// ���f���ԍ�
		int nID;				// ID
		int nCnt;				// �J�E���g
	};

	// *** �֐� ***
	CMagmaBlock();
	~CMagmaBlock();

	/* ������	*/HRESULT Init(void);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	// -- �ݒ� ---------------------------------------------
	/* ���f���ԍ�	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	// -- ���� ---------------------------------------------

private:

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����

	// *** �ϐ� ***
	Info m_Info;		// ���ʏ��

};
