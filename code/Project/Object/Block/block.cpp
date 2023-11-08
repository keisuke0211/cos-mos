//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
// *** block.cpp ***
//========================================
#include "block.h"
#include "../../main.h"

//========================================
// �ÓI�ϐ�
//========================================
int CBlock::m_nNumAll = 0;

//�p�X�ǂݍ���
static const char* s_Path[8] = {
	"data\\MODEL\\Block_Chocolate.x",
	"data\\MODEL\\WallDog\\WallDog_House.x",
};

//========================================
// �R���X�g���N�^
//========================================
CBlock::CBlock(void)
{
	Manager::BlockMgr()->AddList(this);

	m_type   = TYPE::BLOCK;
	m_width  = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;
	m_Info.nID = m_nNumAll;
}

//========================================
// �f�X�g���N�^
//========================================
CBlock::~CBlock()
{

}

//========================================
// ������
//========================================
HRESULT CBlock::Init(BLOCK_TYPE type)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;

	//���f���ԍ��ǂݍ���
	m_Info.nModelIdx = RNLib::Model().Load(s_Path[(int)type]);

	return S_OK;
}

//========================================
// �I��
//========================================
void CBlock::Uninit(void)
{
	
}

//========================================
// �X�V
//========================================
void CBlock::Update(void)
{
	// �ߋ��̈ʒu
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_Info.nModelIdx, false)
		->SetOutLine(true)
		->SetCol(m_color);
}

//========================================
// �`��
//========================================
void CBlock::Draw(void)
{

}