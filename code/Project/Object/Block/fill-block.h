//========================================
// 
// �u���b�N(���� ����)
// Author:KEISUKE OTONO
// 
//========================================
// *** fill-block.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CFillBlock : public CStageObject {
public:
	// �����߃u���b�N���
	enum class BLOCKTYPE
	{
		TYPE_1x1 = 0,
		TYPE_1x2,
		TYPE_1x3,
		TYPE_2x1,
		TYPE_2x2,
		TYPE_3x1,
		TYPE_3x3,
		TYPE_4x4,
		TYPE_MAX,
	};

	// ���ʏ��
	struct Info
	{
		BLOCKTYPE Type;// ���
		short nModelIdx;// ���f���ԍ�
	};

	// *** �֐� ***
	CFillBlock();
	~CFillBlock();
	/* ������	*/HRESULT Init(BLOCKTYPE type);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);

	// -- �ݒ� ---------------------------------------------
	/* ���f���ԍ�	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	// -- �擾 ---------------------------------------------

private:
	//�T�C�Y�̎��
	enum class SIZE
	{
		WIDTH = 0,	//��
		HEIGHT,		//����
		MAX
	};

	// *** �ϐ� ***
	Info m_Info;		// ���ʏ��
	static const char* s_TypePath[(int)CFillBlock::BLOCKTYPE::TYPE_MAX];
	static const float s_Size[(int)CFillBlock::BLOCKTYPE::TYPE_MAX][(int)CFillBlock::SIZE::MAX];
};