//================================================================================================
// 
// �Y����[pile.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"

//=======================================
// �R���X�g���N�^
//=======================================
CPile::CPile()
{
	m_BodyModelID = NONEDATA;
}

//=======================================
// �f�X�g���N�^
//=======================================
CPile::~CPile()
{

}

//=======================================
// ����������
//=======================================
void CPile::Init(void)
{
	m_BodyModelID = RNLib::Model().Load("data\\MODEL\\Block.x");
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");
}

//=======================================
// �I������
//=======================================
void CPile::Uninit(void)
{

}

//=======================================
// �X�V����
//=======================================
void CPile::Update(void)
{

	//���f���z�u
	SetModel();
}

//===============================
//���f���z�u
//===============================
void CPile::SetModel(void)
{
	//�{�̃��f��
	RNLib::Model().Put(m_pos, INITD3DXVECTOR3, m_BodyModelID, false);

	//�����W
	Pos3D PilePos = m_pos;

	//��ԏ�̊�����z�u���邽�߁A�z�u���鍂�����v�Z
	{
		//�����̔����i�؂�̂āj
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//�{�̃T�C�Y�̔���  �{  �T�C�Y �~ ��̊���  �{  ���̂߂荞�ݗ�
		PilePos.y = SIZE_OF_1_SQUARE * 0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//�����f��
		RNLib::Model().Put(PilePos, INITD3DXVECTOR3, m_TrunkModelID);

		//�z�u���W��������
		PilePos.y = SIZE_OF_1_SQUARE;
	}
}

//===============================
//�ݒ菈��
//�����ɕK�v�����i�Œ�3�j��z�u���W���w�肷��
//-------------------------------
//�����P  pos�F�z�u���W
//�����Q  NumTrunk�F���̐��i�Œ�R�j
//�����R  TrunkHeight�F���̂߂荞�ݍ��W�i�߂荞�݂Ȃ� = 0.0f�j
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//�������Œᐔ��������Ă�����C��
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//���ݒ�
	m_pos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;

	//���f���z�u
	SetModel();
}