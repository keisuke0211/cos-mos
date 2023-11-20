//========================================
// 
// �u���b�N (���� ����)
// Author:KEISUKE OTONO
// Arrange:HIRASAWA SHION
// 
//========================================
// *** fill-block.cpp ***
//========================================
#include "fill-block.h"
#include "../../main.h"

//�p�X�ǂݍ���
const char *CFillBlock::s_TypePath[(int)BLOCKTYPE::TYPE_MAX] = {
	"data\\MODEL\\fill-block_1x1.x",
	"data\\MODEL\\fill-block_1x2.x",
	"data\\MODEL\\fill-block_1x3.x",
	"data\\MODEL\\fill-block_2x1.x",
	"data\\MODEL\\fill-block_2x2.x",
	"data\\MODEL\\fill-block_3x1.x",
	"data\\MODEL\\fill-block_3x3.x",
	"data\\MODEL\\fill-block_4x4.x",
};

const float CFillBlock::s_Size[(int)CFillBlock::BLOCKTYPE::TYPE_MAX][(int)CFillBlock::SIZE::MAX] = {
	{ SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE },			//1x1
	{ SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 2 },		//1x2
	{ SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 3 },		//1x3
	{ SIZE_OF_1_SQUARE * 2, SIZE_OF_1_SQUARE },		//2x1
	{ SIZE_OF_1_SQUARE * 2, SIZE_OF_1_SQUARE * 2 },	//2x2
	{ SIZE_OF_1_SQUARE * 3, SIZE_OF_1_SQUARE },		//3x1
	{ SIZE_OF_1_SQUARE * 3, SIZE_OF_1_SQUARE * 3 },	//3x3
	{ SIZE_OF_1_SQUARE * 4, SIZE_OF_1_SQUARE * 4 },	//4x4
};

//========================================
// �R���X�g���N�^
//========================================
CFillBlock::CFillBlock(void)
{
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::FILLBLOCK;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_Info.Type = BLOCKTYPE::TYPE_1x1;
}

//========================================
// �f�X�g���N�^
//========================================
CFillBlock::~CFillBlock()
{

}

//========================================
// ������
//========================================
HRESULT CFillBlock::Init(BLOCKTYPE type)
{
	m_Info.Type = type;

	//�^�C�v�𐔎��ɕϊ�
	const int nType = (int)type;

	//���f���ԍ��ǂݍ���
	m_Info.nModelIdx = RNLib::Model().Load(s_TypePath[nType]);
	m_width = s_Size[nType][(int)SIZE::WIDTH];
	m_height = s_Size[nType][(int)SIZE::HEIGHT];

	return S_OK;
}

//========================================
// �I��
//========================================
void CFillBlock::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CFillBlock::Update(void)
{
	// �ߋ��̈ʒu
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), false)
		->SetCol(m_color);
}