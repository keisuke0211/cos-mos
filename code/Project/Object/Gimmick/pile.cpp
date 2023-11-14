//================================================================================================
// 
// �Y����[pile.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"
#include "../../main.h"

//=======================================
// �R���X�g���N�^
//=======================================
CPile::CPile()
{
	Manager::BlockMgr()->AddList(this);

	//�������
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_BodyModelID = NONEDATA;
	m_TrunkModelID = NONEDATA;
	m_pos = INITVECTOR3D;	//�{�̍��W
	m_NumTrunk = 0;			//���̐��i�Œ�R�j
	m_TrunkHeight = 0.0f;	//�����W
	m_TrunkHeightOld = 0.0f;//�����W
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
	//�O��̍�����ۑ�
	m_TrunkHeightOld = m_TrunkHeight;

	SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + m_TrunkHeight, 0.0f));

	//���f���z�u
	PutModel();
}

//===============================
//���f���z�u
//===============================
void CPile::PutModel(void)
{
	//�{�̃��f��
	RNLib::Model().Put(m_pos, INITD3DXVECTOR3, m_BodyModelID, false);

	//�����W
	Pos3D PilePos = m_pos;

	//��ԏ�̊�����z�u���邽�߁A�z�u���鍂�����v�Z
	{
		//�����̔����i�؂�̂āj
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//�{�̃T�C�Y  �{  �T�C�Y �~ ��̊���  �{  ���̂߂荞�ݗ�
		PilePos.y = SIZE_OF_1_SQUARE * -0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;

		RNLib::Text2D().PutDebugLog(CreateText("�Y�ʒu  Y:%f  �߂荞��:%f", PilePos.y, m_TrunkHeight));
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//�����f��
		RNLib::Model().Put(PilePos, INITD3DXVECTOR3, m_TrunkModelID);

		//�z�u���W��������
		PilePos.y -= SIZE_OF_1_SQUARE;
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

	//�����蔻��̍������Đݒ�
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//���f���z�u
	PutModel();
}

//===============================
//�߂荞�ݗʔ��f����
//===============================
void CPile::CaveInTrunkHeight(float fCaveInHeight)
{
	//�ւ��ݗʔ��f
	fCaveInHeight += m_TrunkHeight;

	//�Y�̍ő�E�Œፂ�����v�Z
	const int nNumHalf = m_NumTrunk / EVENPARITY;
	const float fMaxHeight = SIZE_OF_1_SQUARE * nNumHalf;
	const float fMinHeight = -SIZE_OF_1_SQUARE * nNumHalf;

	//�Y�������Ȃ��悤�ɒ���
	if (m_TrunkHeight < fMinHeight)		 m_TrunkHeight = fMinHeight;
	else if (m_TrunkHeight > fMaxHeight) m_TrunkHeight = fMaxHeight;

	//�߂荞�ݗʂ𔽉f������
	m_TrunkHeight = fCaveInHeight;

	//���f���z�u
	PutModel();
}

//===============================
//�ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;//���ݍ��W���i�[
	ReturnPos.y += m_TrunkHeight; //�߂荞�ݗʂ𔽉f
	return ReturnPos;			  //���W��Ԃ�
}

//===============================
//�O��ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosOldCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;	//���ݍ��W���i�[
	ReturnPos.y += m_TrunkHeightOld;//�O��̂߂荞�ݗʂ𔽉f
	return ReturnPos;				//���W��Ԃ�
}