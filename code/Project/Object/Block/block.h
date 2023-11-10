//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
// *** block.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CBlock : public CStageObject {
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 move;		// �ړ���
		int nType;				// ���
		short nModelIdx;		// ���f���ԍ�
		int nID;				// ID
	};

	// �����߃u���b�N���
	enum class BLOCK_TYPE
	{
		BLOCK_NORMAL = 0,
		BLOCK_NUI,
		BLOCK_MAX
	};

	// *** �֐� ***
	CBlock();
	~CBlock();

	/* ������	*/HRESULT Init(BLOCK_TYPE type);
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